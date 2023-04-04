#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"struct.h"
#include<string.h>
void getString(char **string) {
    int size = 0;
    int realSize = 8;
    *string = (char *) malloc(realSize * sizeof(char));
    rewind(stdin);
    char check;
    while (true) {
        check = (char) getchar();
        if (check == '\n') {
            if (size + 1 < realSize) {
                *string = (char *) realloc(*string, (size + 1) * sizeof(char));
            }
            (*string)[size] = '\0';
            return;
        }

        (*string)[size] = check;
        size++;
        if (size + 1 == realSize) {
            realSize *= 2;
            *string = (char *) realloc(*string, realSize * sizeof(char));
        }
    }
}


Pixel *readPixels(FILE *file, const BMPImage *image) {
    int dataSize = image->imageHeader.width * image->imageHeader.height;
    int bytesPP = image->imageHeader.bpp / 8;
    int rowSize = (image->imageHeader.width * bytesPP);
    int rowPadding = (4 - (rowSize % 4)) % 4;
    int pixelOffset = (int) image->fileHeader.offset + rowPadding;
    fseek(file, pixelOffset, SEEK_SET);
    Pixel *pixels = (Pixel *) malloc(dataSize * sizeof(Pixel));
    unsigned char *buffer = (unsigned char *) malloc(bytesPP * sizeof(unsigned char));
    memset(buffer, 0, bytesPP * sizeof(unsigned char));
    for (int i = 0; i < dataSize; i++) {
        fread(buffer, bytesPP, 1, file);
        if (buffer != NULL && bytesPP == 3) {
            pixels[i].blue = buffer[0];
            pixels[i].green = buffer[1];
            pixels[i].red = buffer[2];
        }
    }
    free(buffer);
    return pixels;
}

BMPImage *readImage(const char *fileName) {
    BMPImage *image = (BMPImage *) malloc(sizeof(BMPImage));
    if (image == NULL || !image) {
        printf("Unable to allocate memory\n");
        return NULL;
    }
    FILE *file = fopen(fileName, "rb");
    if (file == NULL || !file) {
        printf("Unable to open file\n");
        free(image);
        return NULL;
    }
    fread(&image->fileHeader, sizeof(BMPFileHeader), 1, file);
    if (image->fileHeader.magic[0] != 'B' || image->fileHeader.magic[1] != 'M') {
        printf("Invalid file format\n");
        fclose(file);
        free(image);
        return NULL;
    }
    fread(&image->imageHeader, sizeof(BMPImageHeader), 1, file);
    image->pixels = readPixels(file, image);
    fclose(file);
    if (!image->pixels) {
        printf("Unable to read image data\n");
        free(image);
        return NULL;
    }
    return image;
}

void printDataOfPixel(const BMPImage *image) {
    if (image == NULL || image->pixels == NULL) {
        printf("Error: invalid image or pixel data\n");
        return;
    }
    const Pixel *p = image->pixels;
    int dataSize = image->imageHeader.width * image->imageHeader.height;
    for (int i = 0; i < dataSize; i++) {

        printf("\n %05x: ", i);
        printf(" %02x %02x %02x ", p[i].red, p[i].green, p[i].blue);
    }
}


void freeMemory(BMPImage *image) {
    if (image->pixels)
        free(image->pixels);
    free(image);

}