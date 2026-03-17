#include <commun.h>
#include <worker.h>
#include <ctype.h>

#define CONFIG_FILE "../config.conf"
#define MAX_THREADS 8
#define MIN_THREADS 4
#define WORKER "/semaphore_worker"

typedef struct {
    int max_threads_used;
    int max_request;
    int max_workers;
    int waiting_time;
} server_config_t;

void sigusr1_handler(int sig);
void sigchld_handler(int sig);
void sigterm_handler(int sig);
void init_shared(void *p, void *data);
void init_size_t(void *p, void *data);
int load_server_config(const char *filename, server_config_t *config);
int reload_config(server_config_t *config);
void become_your_demon(void);

size_t *shm_info = nullptr;
volatile shared_data *data = nullptr;
sem_t *empty = nullptr;
sem_t *full = nullptr;
sem_t *mutex = nullptr;
sem_t *sem_worker = nullptr;
static char config_file[PATH_MAX];

volatile sig_atomic_t reload = 0;
volatile sig_atomic_t do_stop = 0;

int main() {
    int r = EXIT_SUCCESS;
    if (realpath(CONFIG_FILE, config_file) == nullptr) {
        perror("realpath");
        exit(EXIT_FAILURE);
    }

    server_config_t config;
    if (load_server_config(config_file, &config) == -1) {
        fprintf(stderr, "Erreur lecture fichier config\n");
        return EXIT_FAILURE;
    }
    
    become_your_demon();

    //open semaphore
    mutex = get_semaphore(MUTEX, 1, true);
    if(is_sem_incorrect(mutex)) {
        goto error;
    }
    empty = get_semaphore(EMPTY, 0, true);
    if(is_sem_incorrect(empty, mutex)) {
        goto error;
    }
    full = get_semaphore(FULL, (unsigned int) config.max_request, true);
    if(is_sem_incorrect(full, mutex, empty)) {
        goto error;
    }
    sem_worker = get_semaphore(WORKER, (unsigned int) config.max_workers, true);
    if(is_sem_incorrect(sem_worker, mutex, empty, full)) {
        return -1;
    }

    //create-open shm
    size_t data1[2] = {0, 0};
    data = get_shared_memory(SHM, sizeof(shared_data), O_CREAT | O_RDWR, PROT_READ | PROT_WRITE, true, init_shared, &data1);

    //create-open shm_info (effective length of shared_shm)
    size_t data2[1] = {(unsigned int) config.max_request};
    shm_info = get_shared_memory(SHM_INFO, sizeof(size_t), O_CREAT | O_RDWR, PROT_READ | PROT_WRITE, true, init_size_t, &data2);

    //get request from shm
    boucle:
    while(true) {
        if (reload == 1) {
            if (reload_config(&config) == -1) {
                goto dispose;
            }
            goto boucle;
        }
        if (do_stop == 1) {
            goto dispose;
        }
        P(sem_worker);
            if (reload == 1) {
                if (reload_config(&config) == -1) {
                    goto dispose;
                }
                goto boucle;
            }
            if (do_stop == 1) {
                goto dispose;
            }
        P(empty);
            if (reload == 1) {
                if (reload_config(&config) == -1) {
                    goto dispose;
                }
                goto boucle;
            }
            if (do_stop == 1) {
                goto dispose;
            }
        P(mutex);
            if (reload == 1) {
                if (reload_config(&config) == -1) {
                    goto dispose;
                }
                goto boucle;
            }
            if (do_stop == 1) {
                goto dispose;
            }
            filter_request fr;
            size_t read = data->read;
            fr.pid = data->t[read].pid;
            snprintf(fr.chemin, sizeof(fr.chemin), "%s", data->t[read].chemin);
            fr.filtre = data->t[read].filtre;
            fr.timeout = (unsigned int) config.waiting_time;
            data->read = (read + 1) % *shm_info;
        V(mutex);
        V(full);
        pid_t pid;
        switch((pid = fork())) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                worker(fr);
                V(sem_worker);
                exit(EXIT_SUCCESS);
        }
    }

    goto dispose;
    error:
        r = EXIT_FAILURE;
    dispose:
        sem_close(mutex);
        sem_unlink(MUTEX);
        sem_close(full);
        sem_unlink(FULL);
        sem_close(empty);
        sem_unlink(EMPTY);
        sem_close(sem_worker);
        sem_unlink(WORKER);

        if (shm_info != nullptr) {
            munmap((void*)shm_info, sizeof(size_t));
        }
        shm_unlink(SHM_INFO);
        if (data != nullptr) {
            munmap((void*)data, sizeof(shared_data));
        }
        shm_unlink(SHM);
    return r;
}


// =================================================================
// FONCTIONS AUXILIAIRES
// =================================================================

// --- Signaux ---
// void sigchld_handler(int sig) {
//     (void)sig;
//     while (waitpid(-1, nullptr, WNOHANG) > 0) { }
// }

void sigusr1_handler(int sig) {
    (void)sig;
    reload = 1;
    sem_post(empty);
    sem_post(sem_worker);
}

void sigterm_handler(int sig) {
    (void)sig;
    do_stop = 1;
    sem_post(empty);
    sem_post(sem_worker);
}

// --- How do I name that ---
void init_shared(void *p, void *data) {
    shared_data *d = p;
    
    size_t *val = (size_t*) data;
    d->read = val[0];
    d->write = val[1];
}

void init_size_t(void *p, void *data) {
    size_t *shm_val = p;
    size_t *val = (size_t*) data;
    *shm_val = val[0];
}

// --- Load config file to struct ---
int load_server_config(const char *filename, server_config_t *config) {
    if (!filename || !config) return -1;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen config file");
        return -1;
    }

    char line[256];  // ligne temporaire locale
    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (isspace(*p)) p++;      // ignorer les espaces initiaux
        if (*p == '#' || *p == '\0') // commentaire ou ligne vide
            continue;

        char *sep = strchr(p, ':');
        if (!sep) continue;           // ligne mal formée
        *sep = '\0';
        char *key = p;
        char *value = sep + 1;

        while (isspace(*value)) value++; // ignorer espaces avant la valeur
        char *newline = strchr(value, '\n');
        if (newline) *newline = '\0';

        if (strcmp(key, "max_threads_used") == 0) {
            config->max_threads_used = atoi(value);
        } else if (strcmp(key, "max_request") == 0) {
            config->max_request = atoi(value);
        } else if (strcmp(key, "max_workers") == 0) {
            config->max_workers = atoi(value);
        } else if (strcmp(key, "waiting_time") == 0) {
            config->waiting_time = atoi(value);
        } else {
            fprintf(stderr, "Unknown config key: %s\n", key);
        }
    }
    if (config->max_request > SHM_SIZE) {
        config->max_request = SHM_SIZE;
    } else if (config->max_request < 0) {
        config->max_request = 1;
    }

    if (config->max_workers < 0) {
        config->max_workers = 1;
    }

    if (config->max_threads_used > MAX_THREADS) {
        config->max_threads_used = MAX_THREADS;
    } else if (config->max_threads_used < 0) {
        config->max_threads_used = MIN_THREADS;
    }

    if (config->waiting_time < 0) {
        config->max_workers = 0;
    }

    fclose(fp);
    return 0;
}

// --- Recreate sem & shm from struct config ---
int reload_config(server_config_t *config) {
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGTERM);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    if (config == nullptr) {
        return -1;
    }

    if (load_server_config(config_file, config) == -1) {
        fprintf(stderr, "SIGUSR1: impossible de recharger la configuration\n");
        return -1;
    }
    if (config->max_request > SHM_SIZE) {
        config->max_request = SHM_SIZE;
    }
    
    if (config->max_request == 0 ||
        config->max_workers == 0 ||
        config->waiting_time == 0) {
        fprintf(stderr, "SIGUSR1: configuration invalide\n");
        return -1;
    }
    
    // Fermer ET détruire les sémaphores
    if (sem_close(mutex) ==-1) {
        return -1;
    }
    if (sem_unlink(MUTEX) == -1) {
        return -1;
    }
    if (sem_close(full) ==-1) {
        return -1;
    }
    if (sem_unlink(FULL) == -1) {
        return -1;
    }
    if (sem_close(empty) ==-1) {
        return -1;
    }
    if (sem_unlink(EMPTY) == -1) {
        return -1;
    }
    if (sem_close(sem_worker) ==-1) {
        return -1;
    }
    if (sem_unlink(WORKER) == -1) {
        return -1;
    }
    // Détruire les shm
    if (munmap((void*)shm_info, sizeof(size_t)) == -1) {
        return -1;
    }
    if (munmap((void*)data, sizeof(shared_data)) == -1) {
        return -1;
    }
    if (shm_unlink(SHM_INFO) == -1) {
        return -1;
    }
    if (shm_unlink(SHM) == -1) {
        return -1;
    }

    //open semaphore
    mutex = get_semaphore(MUTEX, 1, true);
    if(is_sem_incorrect(mutex)) {
        return -1;
    }
    empty = get_semaphore(EMPTY, 0, true);
    if(is_sem_incorrect(empty, mutex)) {
        return -1;
    }
    full = get_semaphore(FULL, (unsigned int) config->max_request, true);
    if(is_sem_incorrect(full, mutex, empty)) {
        return -1;
    }
    sem_worker = get_semaphore(WORKER, (unsigned int) config->max_workers, true);
    if(is_sem_incorrect(sem_worker, mutex, empty, full)) {
        return -1;
    }

    //create-open shm
    size_t data1[2] = {0, 0};
    data = get_shared_memory(SHM, sizeof(shared_data), O_CREAT | O_RDWR, PROT_READ | PROT_WRITE, true, init_shared, &data1);
    if (data == nullptr) {
        return -1;
    }
    //create-open shm_info (effective length of shared_shm)
    size_t data2[1] = {(unsigned int) config->max_request};
    shm_info = get_shared_memory(SHM_INFO, sizeof(size_t), O_CREAT | O_RDWR, PROT_READ | PROT_WRITE, true, init_size_t, &data2);
    if (shm_info == nullptr) {
        return -1;
    }
    reload = 0;
    sigprocmask(SIG_SETMASK, &oldmask, nullptr);
    return 0;
}

void become_your_demon(void) {
    // Premier fork
    switch(fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
            break;  // Enfant continue
        default:
            exit(EXIT_SUCCESS); // Parent quitte
    }
    
    // Nouvelle session
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    
    // Deuxième fork pour éviter de redevenir leader de session
    switch(fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
            break;  // Enfant continue
        default:
            exit(EXIT_SUCCESS); // Parent quitte
    }
    
    // Mask fichiers (AVANT chdir)
    umask(0);
    
    // Répertoire racine
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }
    
    // Fermer tous les descripteurs de fichiers ouverts
    long maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1) {
        maxfd = 1024;  // Valeur par défaut si échec
    }
    for (int fd = 0; fd < maxfd; fd++) {
        close(fd);
    }
    
    // Redirection stdin/stdout/stderr vers /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDIN_FILENO) < 0 ||
        dup2(fd, STDOUT_FILENO) < 0 ||
        dup2(fd, STDERR_FILENO) < 0) {
        exit(EXIT_FAILURE);
    }
    if (fd > STDERR_FILENO) {
        close(fd);
    }
    
    // SIGTERM
    struct sigaction term;
    term.sa_handler = sigterm_handler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = SA_RESTART;
    if (sigaction(SIGTERM, &term, nullptr) == -1) {
        exit(EXIT_FAILURE);
    }
    
    // SIGUSR1
    struct sigaction usr1;
    usr1.sa_handler = sigusr1_handler;
    sigemptyset(&usr1.sa_mask);
    usr1.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &usr1, nullptr) == -1) {
        exit(EXIT_FAILURE);
    }
    
    // SIGCHLD
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, nullptr);
    
    // Bloquer tous les autres signaux (APRÈS installation des handlers)
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGCHLD);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGTERM);
    sigdelset(&mask, SIGKILL);  // Non bloquable de toute façon
    sigdelset(&mask, SIGSTOP);  // Non bloquable de toute façon
    if (sigprocmask(SIG_SETMASK, &mask, nullptr) == -1) {
        exit(EXIT_FAILURE);
    }
}