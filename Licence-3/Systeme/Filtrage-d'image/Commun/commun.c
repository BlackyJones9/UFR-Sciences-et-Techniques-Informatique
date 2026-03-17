#include "commun.h"

bool is_sem_incorrect(sem_t *s, ...) {
    if (s != SEM_FAILED) {
        return false;
    }
    va_list ap;
    va_start(ap);
    sem_t *sem;
    while ((sem = va_arg(ap, sem_t*)) != SEM_FAILED) {
        sem_close(sem);
    }
    va_end(ap);
    return true;
}

sem_t* get_semaphore(const char *name, unsigned int initial, int is_server) {
    sem_t *sem;

    if (is_server) { //server : create-open
        sem = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, initial);
    } else { //client : open
        sem = sem_open(name, 0);
    }

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    return sem;
}


void *get_shared_memory(const char *name, size_t size, int shm_flags, int mmap_flags, bool isServer, void (*init_fn)(void *ptr, void *), void *data) {
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, shm_flags, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        return nullptr;
    }
    
    if (isServer) {
        if (ftruncate(shm_fd, (off_t)size) == -1) {
            perror("ftruncate");
            close(shm_fd);
            return nullptr;
        }
    }
     
    ptr = mmap(nullptr, size, mmap_flags, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return nullptr;
    }
    if (isServer && init_fn) {
        init_fn(ptr, data);
    }
    return ptr;
}

void P(sem_t *s) {
    if(sem_wait(s) == -1) {
        if (errno == EINTR) {
            // Signal reçu, on retourne sans erreur
            // Le code appelant vérifiera do_stop | reload
            return;
        }
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

void V(sem_t *s) {
    if(sem_post(s) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}