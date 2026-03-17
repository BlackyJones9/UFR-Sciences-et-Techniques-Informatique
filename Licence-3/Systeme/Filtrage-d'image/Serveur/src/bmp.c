#include <bmp.h>

#pragma pack(push, 1)
// --- En tête BMP commun à tous ---
struct BITMAPFILEHEADER {
    uint16_t bfType;      // 'BM' = 0x4D42
    uint32_t bfSize;      // Taille du fichier
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;   // Offset début pixels
    uint32_t biSize;         
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
};
#pragma pack(pop)

struct BMPData{
    uint8_t *shm;        // mémoire partagée (début du fichier BMP)
    uint8_t *pixelArray; // pointeur vers les pixels dans shm

    int32_t width;
    int32_t height;
    uint32_t bytesPerPixel;
    uint32_t rowSize;
    uint32_t padding;
    uint32_t dataOffset; // bfOffBits
};

static inline uint32_t pixelIndexToOffset(const BMPData *bmp, uint32_t pixelIndex) {
    uint32_t width = (uint32_t)bmp->width;
    uint32_t row = pixelIndex / width;
    uint32_t col = pixelIndex % width;
    return bmp->dataOffset + row * (bmp->rowSize + bmp->padding) + col * bmp->bytesPerPixel;
}

void divideImageForThreads(BMPData *bmp, uint32_t numThreads, thread_workspace *tw, int filter) {
    uint32_t totalPixels = (uint32_t)(bmp->width * bmp->height);
    uint32_t pixelsPerThread = totalPixels / numThreads;
    uint32_t remainder = totalPixels % numThreads;
    uint32_t currentPixel = 0;

    for (uint32_t k = 0; k < numThreads; k++) {
        uint32_t count = pixelsPerThread + ((k < remainder) ? 1 : 0);
        tw[k].BMPData = bmp;
        tw[k].startOffset = pixelIndexToOffset(bmp, currentPixel);
        tw[k].pixelCount = count;
        tw[k].filtre = filter;
        currentPixel += count;
    }
}

BMPData *initBMPData(unsigned char *shm) {
    BMPData *bmp = malloc(sizeof(*bmp));
    if (bmp == nullptr) {
        return nullptr;
    }
    BITMAPFILEHEADER *hdr = (BITMAPFILEHEADER *)shm;

    bmp->shm = shm;

    bmp->dataOffset = hdr->bfOffBits;
    bmp->pixelArray = shm + hdr->bfOffBits;

    bmp->width  = hdr->biWidth;
    bmp->height = hdr->biHeight;

    bmp->bytesPerPixel = hdr->biBitCount / 8;
    bmp->rowSize  = (uint32_t)bmp->width * bmp->bytesPerPixel;
    bmp->padding  = (4 - (bmp->rowSize % 4)) % 4;

    return bmp;
}


// ---------------------------------------------------------------------------
// --- SCALE ---
// ---------------------------------------------------------------------------
uint32_t move_once(const BMPData *bmp, uint32_t offset) {
    uint32_t pixelSize = bmp->bytesPerPixel;
    uint32_t lineBytes = bmp->rowSize + bmp->padding;
    uint32_t posInRow = (offset - bmp->dataOffset) % lineBytes;

    if (posInRow + pixelSize < bmp->rowSize) {
        return offset + pixelSize;
    } else {
        return offset + (lineBytes - posInRow);
    }
}

void apply_grayscale(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        uint8_t g = (uint8_t)(pixel[0] + pixel[1] + pixel[2]) / 3;
        pixel[0] = g;
        pixel[1] = g;
        pixel[2] = g;

        offset = move_once(bmp, offset);
    }
}

void apply_redscale(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (255 > pixel[0] * 1.5 ? pixel[0] * 1.5 : 255);
        pixel[1] = (uint8_t) (pixel[1] * 0.5);
        pixel[2] = (uint8_t) (pixel[2] * 0.5);

        offset = move_once(bmp, offset);
    }
}

void apply_greenscale(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (pixel[0] * 0.5);
        pixel[1] = (uint8_t) (255 > pixel[1] * 1.5 ? pixel[1] * 1.5 : 255);
        pixel[2] = (uint8_t) (pixel[2] * 0.5);

        offset = move_once(bmp, offset);
    }
}

void apply_bluescale(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (pixel[0] * 0.5);
        pixel[1] = (uint8_t) (pixel[1] * 0.5);
        pixel[2] = (uint8_t) (255 > pixel[2] * 1.5 ? pixel[2] * 1.5 : 255);

        offset = move_once(bmp, offset);
    }
}

void apply_pinkscale(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (255 > pixel[0] * 1.5 ? pixel[0] * 1.5 : 255);
        pixel[1] = (uint8_t) (pixel[1] * 0.5);
        pixel[2] = (uint8_t) (255 > pixel[2] * 1.5 ? pixel[2] * 1.5 : 255);

        offset = move_once(bmp, offset);
    }
}

void apply_transparency(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (pixel[0] * 0.5 + 0.5 * 255);
        pixel[1] = (uint8_t) (pixel[1] * 0.5 + 0.5 * 255);
        pixel[2] = (uint8_t) (pixel[2] * 0.5 + 0.5 * 255);

        offset = move_once(bmp, offset);
    }
}

void apply_negative(void *arg) {
    thread_workspace *ws = (thread_workspace*)arg;
    BMPData *bmp = ws->BMPData;

    uint32_t offset = ws->startOffset;

    for(uint32_t k = 0; k < ws->pixelCount; k++) {
        uint8_t *pixel = bmp->shm + offset;
        pixel[0] = (uint8_t) (255 - pixel[0]);
        pixel[1] = (uint8_t) (255 - pixel[1]);
        pixel[2] = (uint8_t) (255 - pixel[2]);

        offset = move_once(bmp, offset);
    }
}