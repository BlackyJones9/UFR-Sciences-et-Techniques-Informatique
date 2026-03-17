#ifndef WORKER_H
#define WORKER_H

#include <bmp.h>
#include <types.h>

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>


#define THREAD_NB 16

extern int worker(filter_request fr);

#endif //WORKER_h