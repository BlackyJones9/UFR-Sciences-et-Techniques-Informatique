#include <worker.h>

void *get_worker_shared_memory(const char *name, struct stat *st);
void *threadFunc(void *arg);
void alarm_handler(int sig);

volatile sig_atomic_t timeout_expired = 0;

int worker(filter_request fr) {
    char fifo_name[64];
    unsigned char *shm = nullptr;
    struct stat st;
    BMPData *data = nullptr;
    thread_workspace *threadArgs = nullptr;
    pthread_t *threadArray = nullptr;
    char shm_name[32];
    int fifo_fd = -1;
    int status = FILTER_SUCCESS;
    srand((unsigned int) time(nullptr));

    // Configuration du signal handler pour l'alarme
    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGALRM, &sa, nullptr) == -1) {
        perror("sigaction");
        goto send_error;
    }
    
    snprintf(fifo_name, sizeof(fifo_name), "%s%d", FIFO_RES, fr.pid);
    snprintf(shm_name, sizeof(shm_name), "/img_%d", fr.pid);

    //get image informations
    int fd_img = open(fr.chemin, O_RDONLY);
    if (fd_img == -1) {
        perror("open");
        goto send_error;
    }

    if (fstat(fd_img, &st) == -1) {
        perror("fstat");
        close(fd_img);
        goto send_error;
    }
 
    //create-open a shared memory for the image
    shm = get_worker_shared_memory(shm_name, &st);
    if (shm == nullptr) {
        close(fd_img);
        goto send_error;
    }
  
    //copie l'image originale dans la zone partagée
    ssize_t bytes_read = read(fd_img, shm, (size_t) st.st_size);
    close(fd_img);

    if (bytes_read != st.st_size) {
        perror("read");
        goto send_error;
    }
    
    //init BMPDATA
    data = initBMPData(shm);
    if (data == nullptr) {
        goto send_error;
    }

    //setUp func for threads
    threadArgs = calloc(THREAD_NB, sizeof(*threadArgs));
    if (threadArgs == nullptr) {
        goto send_error;
    }
 
    divideImageForThreads(data, THREAD_NB, threadArgs, fr.filtre);

    //Manage Threads
    threadArray = calloc(THREAD_NB, sizeof(*threadArray));
    if (threadArray == nullptr) {
        goto send_error;
    }
    for(int k = 0; k < THREAD_NB; k++) {
        pthread_t tid;
        if (pthread_create(&tid, nullptr, threadFunc, &threadArgs[k]) != 0) {
            perror("pthread_create");
            //Attendre threads déjà créés
            for(int j = 0; j < k; j++) {
                pthread_join(threadArray[j], nullptr);
            }
            goto send_error;
        }
        threadArray[k] = tid;
    }
    for (int k = 0; k < THREAD_NB; k++) {
        pthread_join(threadArray[k], nullptr);
    }


    goto send_success;

send_error:
    status = FILTER_FAILURE;

    //Envoyer la réponse d'erreur avec timeout
    printf("[Worker] Tentative d'ouverture du FIFO pour envoyer l'erreur...\n");
    timeout_expired = 0;
    alarm(fr.timeout);
    
    fifo_fd = open(fifo_name, O_WRONLY);
    alarm(0);  // Annuler l'alarme si ouverture réussie
    
    if (timeout_expired) {
        fprintf(stderr, "[Worker] Timeout: client n'a pas ouvert le FIFO\n");
        goto cleanup;
    }
    
    if (fifo_fd == -1) {
        if (errno == EINTR) {
            fprintf(stderr, "[Worker] Ouverture du FIFO interrompue\n");
        } else {
            perror("[Worker] open fifo");
        }
        goto cleanup;
    }
    
    // Écrire avec timeout
    timeout_expired = 0;
    alarm(fr.timeout);
    
    if (write(fifo_fd, &status, sizeof(status)) == -1) {
        alarm(0);
        if (errno == EINTR && timeout_expired) {
            fprintf(stderr, "[Worker] Timeout lors de l'écriture du status d'erreur\n");
        } else {
            perror("[Worker] write");
        }
        close(fifo_fd);
        goto cleanup;
    }
    
    alarm(0);
    close(fifo_fd);
    fifo_fd = -1;
    goto cleanup;

send_success:
    //Envoyer la valeur de succès avec timeout
    timeout_expired = 0;
    alarm(fr.timeout);
    fifo_fd = open(fifo_name, O_WRONLY);
    alarm(0);
    
    if (timeout_expired) {
        fprintf(stderr, "[Worker] Timeout: client n'a pas ouvert le FIFO\n");
        goto cleanup;
    }
    
    if (fifo_fd == -1) {
        if (errno == EINTR) {
            fprintf(stderr, "[Worker] Ouverture du FIFO interrompue\n");
        } else {
            perror("[Worker] open fifo");
        }
        goto cleanup;
    }
    
    // Écrire le status avec timeout
    timeout_expired = 0;
    alarm(fr.timeout);
    
    if (write(fifo_fd, &status, sizeof(status)) != sizeof(status)) {
        alarm(0);
        if (errno == EINTR && timeout_expired) {
            fprintf(stderr, "[Worker] Timeout lors de l'écriture du status\n");
        } else {
            perror("[Worker] write");
        }
        close(fifo_fd);
        goto cleanup;
    }
    
    //Envoyer le contenu de l'image modifiée avec timeout
    ssize_t written = 0;
    size_t total = (size_t) st.st_size;
    
    while (written < (ssize_t)total) {
        ssize_t n = write(fifo_fd, shm + written, total - (size_t)written);
        
        if (n == -1) {
            if (errno == EINTR && timeout_expired) {
                alarm(0);
                fprintf(stderr, "[Worker] Timeout lors de l'écriture des données (envoyé %zd/%zu octets)\n", 
                        written, total);
                break;
            } else {
                perror("[Worker] write image data");
                break;
            }
        }
        written += n;
    }
    
    alarm(0);  // Annuler l'alarme

    close(fifo_fd);
    fifo_fd = -1;
    goto cleanup;
    
cleanup:
    // S'assurer qu'aucune alarme n'est active
    alarm(0);
    
    if (fifo_fd != -1) close(fifo_fd);
    if (threadArray) {free(threadArray);}
    if (threadArgs) {free(threadArgs);}
    if (data) {free(data);}
    if (shm) {
        munmap(shm, (size_t) st.st_size);
        shm_unlink(shm_name);
    }
    
    return status;
}

void alarm_handler(int sig) {
    (void)sig;
    timeout_expired = 1;
}

void *threadFunc(void *arg) {
    thread_workspace *th = (thread_workspace*) arg;
    switch(th->filtre) {
        case 1:
            apply_grayscale(th);
            break;
        case 2:
            apply_redscale(th);
            break;
        case 3:
            apply_greenscale(th);
            break;
        case 4:
            apply_bluescale(th);
            break;
        case 5:
            apply_pinkscale(th);
            break;
        case 6:
            apply_transparency(th);
            break;
        case 7:
            apply_negative(th);
            break;
        case 8:
            th->filtre = (rand() % 7) + 1;
            threadFunc(th);
            break;
    }
    return nullptr;
}

void *get_worker_shared_memory(const char *name, struct stat *st) {
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // define map size
    if (ftruncate(shm_fd, st->st_size) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // map
    ptr = mmap(nullptr, (size_t) st->st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    close(shm_fd);  // On peut fermer le fd après mmap
    
    return ptr;
}