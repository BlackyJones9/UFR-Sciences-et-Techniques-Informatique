#ifndef COMMUN_H
#define COMMUN_H

// #include <stdint.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <errno.h>

#define MUTEX "/semaphore_mutex"
#define EMPTY "/semaphore_empty"
#define FULL "/semaphore_full"
#define SHM "/shared_data"
#define SHM_INFO "/shared_data_info"

#define MAX_FILTER 8

extern bool is_sem_incorrect(sem_t *s, ...);

extern sem_t *get_semaphore(const char *name, unsigned int initial, int is_server);

extern void *get_shared_memory(const char *name, size_t size, int shm_flags, int mmap_falgs, bool is_server, void (*init_fn)(void *, void *), void *data);

extern void P(sem_t *s);

extern void V(sem_t *s);

#endif // COMMUN_H