#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <sys/types.h>

#define SHM_SIZE 30
#define FIFO_RES "/tmp/fifo_res_"

#define FILTER_SUCCESS 1
#define FILTER_FAILURE -1

typedef struct {
    pid_t pid;
    char chemin [256];
    int filtre;
    int parametres [5];
    unsigned int timeout;
} filter_request;

typedef struct {
    size_t read; //le curseur de lecture du serveur
    size_t write; //le curseur d'écriture des clients
    filter_request t[SHM_SIZE]; //un tableau de requêtes clients
} shared_data;

#endif