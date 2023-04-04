#ifndef LABA3__STRUCT_H
#define LABA3__STRUCT_H
#pragma pack(1)
typedef struct {
    char magic[2];
    unsigned int size;
    unsigned int reserved;
    unsigned int offset;
} BMPFileHeader;

typedef struct {
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bpp;
    unsigned int compression;
    unsigned int imageSize;
    int xPpm;
    int yPpm;
    unsigned int colors;
    unsigned int importantColors;
} BMPImageHeader;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;

typedef struct {
    BMPFileHeader fileHeader;
    BMPImageHeader imageHeader;
    Pixel *pixels;
} BMPImage;
#pragma pack()
#endif
