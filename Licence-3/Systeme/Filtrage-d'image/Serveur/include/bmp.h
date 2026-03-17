#ifndef BMP_H
#define BMP_H

#include <stdlib.h>
#include <stdint.h>

typedef struct BITMAPFILEHEADER BITMAPFILEHEADER;
typedef struct BMPData BMPData;

typedef struct {
    BMPData *BMPData;
    uint32_t startOffset;
    uint32_t pixelCount;
    int filtre;
} thread_workspace;


extern void divideImageForThreads(BMPData *bmp, uint32_t numThreads, thread_workspace *tw, int filter);

extern BMPData *initBMPData(unsigned char *shm);

extern void apply_grayscale(void *arg);

extern void apply_redscale(void *arg);

extern void apply_greenscale(void *arg);

extern void apply_bluescale(void *arg);

extern void apply_pinkscale(void *arg);

extern void apply_transparency(void *arg);

extern void apply_negative(void *arg);

#endif //BMP_H