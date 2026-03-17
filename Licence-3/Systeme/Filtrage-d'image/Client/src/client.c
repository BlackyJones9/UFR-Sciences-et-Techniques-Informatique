#include <commun.h>
#include <types.h>

#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define TIMEOUT_RESPONSE 5
#define TIMEOUT_DATA 10 

#define REPR_TAB 4
#define BASE_GAP 0
#define TERMINAL_MARGIN 5
#define INIT_LENGTH_WORD  2
#define OPT_SHORT "-"
#define SHORT_HELP "?"

void print_help();

volatile sig_atomic_t timeout = 0;

void alarm_handler(int sig) {
    (void)sig;
    timeout = 1;
}

int main(int argc, char **argv) {
    int r = EXIT_SUCCESS;
    
    int opt;

    while ((opt = getopt(argc, argv, ":d")) != -1) {
        switch (opt) {
            case '?':
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    filter_request fr;
    char fifo_name[64];
    
    sem_t *mutex = nullptr;
    sem_t *empty = nullptr;
    sem_t *full  = nullptr;

    volatile shared_data *data = nullptr;
    size_t *shm_info = nullptr;

    int fifo_fd = -1;
    int fd_out  = -1;

    unsigned char *img_data = nullptr;
    struct stat st;
    char full_path[PATH_MAX];

    bool fifo_created = false;

    // verify number of arguments
    if (argc < 3) {
        goto error_arguments;
    }

    //get image full path
    if (realpath(argv[1], full_path) == nullptr) {
        perror("realpath");
        goto error;
    }

    if (stat(full_path, &st) == -1) {
        perror("stat");
        goto error;
    }
    
    // Vérification de la taille du fichier
    off_t taille = st.st_size;
    if (st.st_size <= 0 || taille > 100L * 1024 * 1024) {
        fprintf(stderr, "Taille de fichier invalide: %ld\n", st.st_size);
        goto error;
    }

    // get filter
    int filter_id = atoi(argv[2]);
    if (filter_id <= 0 || filter_id > MAX_FILTER) {
        goto error_filter;
    }

    // get extension
    char *extension = full_path + strlen(full_path) - 3;
    if (strcmp(extension, "bmp") != 0) {
        goto error_extension;
    }

    // open semaphore
    mutex = get_semaphore(MUTEX, 1, false);
    if(is_sem_incorrect(mutex)) {
        goto error;
    }
    empty = get_semaphore(EMPTY, SHM_SIZE, false);
    if(is_sem_incorrect(empty, mutex)) {
        goto error;
    }
    full = get_semaphore(FULL, 0, false);
    if(is_sem_incorrect(full, mutex, empty)) {
        goto error;
    }

    // open shared_shm
    data = get_shared_memory(SHM, sizeof(shared_data), O_RDWR, PROT_READ | PROT_WRITE, false, nullptr, nullptr);
    if (data == nullptr) {
        goto error;
    }
    //open shm_info (effective length of shared_shm)
    shm_info = get_shared_memory(SHM_INFO, sizeof(size_t), O_RDONLY, PROT_READ, false, nullptr, nullptr);
    if (shm_info == nullptr) {
        goto error;
    }

    // create request
    fr.pid = getpid();
    snprintf(fr.chemin, sizeof(fr.chemin), "%s", full_path);
    fr.filtre = filter_id;
    char pid[6];
    sprintf(pid, "%d", fr.pid);

    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, nullptr) == -1) {
        perror("sigaction");
        goto error;
    }

    // create fifo_result
    snprintf(fifo_name, sizeof(fifo_name), "%s%s", FIFO_RES, pid);
    if (mkfifo(fifo_name, S_IRUSR | S_IWUSR) == -1) {
        perror("mkfifo");
        goto error;
    }
    fifo_created = true;
    
    // send request if there is space in shm
    P(full);
    P(mutex);
        data->t[data->write] = fr;
        data->write = (data->write + 1) % *shm_info;
    V(mutex);
    V(empty);
    
    alarm(TIMEOUT_RESPONSE);
    fifo_fd = open(fifo_name, O_RDONLY);
    alarm(0);
    if (fifo_fd == -1) {
        if (errno == EINTR && timeout) {
            fprintf(stderr, "Timeout: aucun serveur connecté à la FIFO\n");
        } else {
            perror("open");
        }
        goto error;
    }

    // Lecture du code de success
    int success;
    ssize_t bytes_read = read(fifo_fd, &success, sizeof(success));
    if (bytes_read == -1) {
        if (errno == ETIMEDOUT) {
            fprintf(stderr, "Timeout: le serveur ne répond pas\n");
        } else {
            perror("read");
        }
        goto error;
    } else if (bytes_read == 0) {
        fprintf(stderr, "Le serveur a fermé la connexion prématurément\n");
        goto error;
    } else if (bytes_read != sizeof(success)) {
        fprintf(stderr, "Lecture incomplète du code de succès\n");
        goto error;
    }

    if (success == FILTER_FAILURE) {
        goto error_server;
    }

    // Lecture de l'image
    img_data = malloc((size_t)st.st_size);
    if (img_data == nullptr) {
        perror("malloc");
        goto error;
    }
    
    size_t total_read = 0;
    alarm(TIMEOUT_DATA);
    while (total_read < (size_t)st.st_size) {
        size_t remaining = (size_t)st.st_size - total_read;
        if (img_data == nullptr) {
            goto error;
        }
        ssize_t bytes = read(fifo_fd, img_data + total_read, remaining);
        if (bytes == -1) {
            if (errno == ETIMEDOUT) {
                fprintf(stderr, "Timeout: données incomplètes recu\n");
            } else {
                perror("read");
            }
            free(img_data);
            img_data = nullptr;
            goto error;
        }
        if (bytes == 0) {
            fprintf(stderr, "EOF prématuré\n");
            free(img_data);
            img_data = nullptr;
            goto error;
        }
        total_read += (size_t)bytes;
    }
    close(fifo_fd);
    fifo_fd = -1;
    unlink(fifo_name);
    fifo_created = false;

    // Écrire l'image modifiée
    char new_path[PATH_MAX];
    char *dot = strrchr(full_path, '.');
    if (dot != nullptr) {
        // Il y a une extension
        size_t base_len = (size_t) (dot - full_path);
        snprintf(new_path, sizeof(new_path), "%.*s_filtered%s", 
                (int)base_len, full_path, dot);
    } else {
        // Pas d'extension trouvée
        snprintf(new_path, sizeof(new_path), "%s_filtered", full_path);
    }
    fd_out = open(new_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out == -1) {
        perror("open output");
        free(img_data);
        img_data = nullptr;
        goto error;
    }

    if (write(fd_out, img_data, (size_t)st.st_size) != st.st_size) {
        fprintf(stderr, "Erreur écriture fichier\n");
        free(img_data);
        img_data = nullptr;
        goto error;
    }

    free(img_data);
    img_data = nullptr;  // Éviter le double free
    goto cleanup;

    error_arguments:
        printf("Usage : ./client ../[path_to_image] n ; with n the filter id (see help for filters)\n");
        goto error;
    error_filter:
        printf("The filter id has to be between 1 and %d\n", MAX_FILTER);
        goto error;
    error_extension:
        printf("Image type has to be .bmp\n");
        goto error;
    error_server:
        fprintf(stdout, "Serveur has returned an error\n");
        goto error;
    error:
        r = EXIT_FAILURE;
        fprintf(stdout, "For help. Try: ./client -?\n");
    cleanup:
        if (fd_out != -1) close(fd_out);
        if (fifo_fd != -1) close(fifo_fd);
        if (img_data != nullptr) {
            free(img_data);
            img_data = nullptr;
        }

        if (mutex != nullptr) sem_close(mutex);
        if (empty != nullptr) sem_close(empty);
        if (full != nullptr) sem_close(full);

        if (fifo_created) {
            unlink(fifo_name);
        }
        
    return r;
}

// =================================================
// --- SHOW HELP ---
// =================================================
static void print_str(const char *text, size_t gap) {
  size_t nbrcol = INIT_LENGTH_WORD;
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
    nbrcol = (size_t) w.ws_col;
  }
  size_t taille_ligne = 0;
  size_t taille_mot = 0;
  const char *start_word = text;
  const char *actual_char = text;
  size_t len = strlen(text);
  printf("%*s", REPR_TAB * (int) (gap), " ");
  for (size_t i = 0; i <= len; i++) {
    if (text[i] == ' ' || text[i] == '\0') {
      taille_ligne += taille_mot + 1;
      if (taille_ligne > nbrcol - TERMINAL_MARGIN) {
        printf("\n");
        printf("%*s", REPR_TAB * (int) (gap + 1), " ");
        taille_ligne = taille_mot + REPR_TAB * (gap + 1);
      }
      for (size_t k = 0; k < taille_mot + 1; k++) {
        printf("%c", start_word[k]);
      }
      start_word = actual_char;
      taille_mot = 0;
    } else {
      taille_mot++;
    }
    actual_char++;
  }
  printf("\n");
}

void print_help() {
  const char *usage = "Usage:  client <image.bmp> <filtre>";
  const char *description
    = "Description:  Send the image to the server with filter choice, the "
      "server send back the filtered image";
  const char *info = "Program  Information :";
  print_str(usage, BASE_GAP);
  printf("\n");
  print_str(description, BASE_GAP);

  printf("\n");

  print_str(info, BASE_GAP);
  printf("%*s", REPR_TAB, " ");
  printf("%s%s\n", OPT_SHORT, SHORT_HELP);
  const char *help = "Help:  Print this help message and exit.";
  print_str(help, BASE_GAP + 1);
  printf("\n");

  
  const char *output_ctrl = "List of filters :";
  print_str(output_ctrl, BASE_GAP);
  const char *filter = "1 -> grayscale";
  print_str(filter, BASE_GAP + 1);
  filter = "2 -> redscale";
  print_str(filter, BASE_GAP + 1);
  filter = "3 -> greenscale";
  print_str(filter, BASE_GAP + 1);
  filter = "4 -> bluescale";
  print_str(filter, BASE_GAP + 1);
  filter = "5 -> pinkscale";
  print_str(filter, BASE_GAP + 1);
  filter = "6 -> transparency";
  print_str(filter, BASE_GAP + 1);
  filter = "7 -> negate";
  print_str(filter, BASE_GAP + 1);
  filter = "8 -> random";
  print_str(filter, BASE_GAP + 1);
}
