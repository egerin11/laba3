#include "struct.h"
#include<stdlib.h>
#include <stdio.h>
#include"convertPhoto.h"
#include <string.h>
#include "minifunction.h"

void convert(BMPImage *oldImage, const char *fileName, int choice) {
    if (oldImage == NULL) {
        printf("Input image is NULL\n");
        return;
    }
    if (oldImage->pixels == NULL) {
        printf("Input image has no data\n");
        return;
    }
    BMPImage blackAndWhite;
    blackAndWhite.fileHeader = oldImage->fileHeader;
    blackAndWhite.imageHeader = oldImage->imageHeader;
    blackAndWhite.pixels = (Pixel *) malloc(
            blackAndWhite.imageHeader.width * blackAndWhite.imageHeader.height * sizeof(Pixel));
    if (blackAndWhite.pixels == NULL) {
        printf("Unable to allocate memory\n");
        return;
    }
    int dataSize = oldImage->imageHeader.width * oldImage->imageHeader.height;

    memcpy(blackAndWhite.pixels, oldImage->pixels, dataSize * sizeof(Pixel));
    switchForConvert(choice, dataSize, oldImage, blackAndWhite);

    FILE *file = fopen(fileName, "wb");
    if (!file) {
        printf("Unable to open file\n");
        free(blackAndWhite.pixels);
        return;
    }
    fwrite(&blackAndWhite.fileHeader, sizeof(blackAndWhite.fileHeader), 1, file);
    fwrite(&blackAndWhite.imageHeader, sizeof(blackAndWhite.imageHeader), 1, file);
    fwrite(blackAndWhite.pixels, sizeof(Pixel), dataSize, file);
    free(blackAndWhite.pixels);
    fclose(file);
}

void switchForConvert(int choice, int dataSize, const BMPImage *oldImage, BMPImage blackAndWhite) {
    switch (choice) {
        case 1: {
            convertBlackAndWhite(oldImage, blackAndWhite, dataSize);
        }
            break;
        case 2: {
            convertNegative(oldImage, blackAndWhite, dataSize);
        }
            break;
        case 3: {
            double number = 0;
            double numberTwo = 0;
            double numberThree = 0;
            inputDataForGamma(&number, &numberTwo, &numberThree);
            gammaConvert(blackAndWhite, number, numberTwo, numberThree);
        }
            break;
        case 4: {
            int number = 0;
            int numberTwo = 0;
            inputDataForMedian(&number, &numberTwo);
            medianFilter(blackAndWhite, number, numberTwo);
        }
            break;
        default: {
            printf("");
        }
    }
}

void convertBlackAndWhite(const BMPImage *oldImage, BMPImage blackAndWhite, const int dataSize) {
    const Pixel *pixel = oldImage->pixels;
    for (int i = 0; i < dataSize; i++) {
        unsigned char gray = (unsigned char) ((0.3 * pixel[i].red) + pixel[i].blue * 0.11 + pixel[i].green * 0.59);
        blackAndWhite.pixels[i].red = gray;
        blackAndWhite.pixels[i].green = gray;
        blackAndWhite.pixels[i].blue = gray;
    }
}

void convertNegative(const BMPImage *oldImage, BMPImage blackAndWhite, const int dataSize) {
    const Pixel *pixel = oldImage->pixels;
    for (int i = 0; i < dataSize; i++) {

        blackAndWhite.pixels[i].red = 255 - pixel[i].red;
        blackAndWhite.pixels[i].green = 255 - pixel[i].green;
        blackAndWhite.pixels[i].blue = 255 - pixel[i].blue;
    }
}

int comparePixels(const void *a, const void *b) {
    const Pixel pixelA = *(Pixel *) a;
    const Pixel pixelB = *(Pixel *) b;

    unsigned int brightnessA = (pixelA.red + pixelA.green + pixelA.blue) / 3;
    unsigned int brightnessB = (pixelB.red + pixelB.green + pixelB.blue) / 3;

    if (brightnessA < brightnessB) {
        return -1;
    } else if (brightnessA > brightnessB) {
        return 1;
    } else {
        return 0;
    }
}


Pixel **memoryPixel(BMPImage blackAndWhite) {
    Pixel **outputPixels = (Pixel **) malloc(blackAndWhite.imageHeader.width * sizeof(Pixel *));
    for (int i = 0; i < blackAndWhite.imageHeader.width; i++) {
        outputPixels[i] = (Pixel *) malloc(blackAndWhite.imageHeader.height * sizeof(Pixel));
    }
    return outputPixels;
}

void medianFilter(BMPImage blackAndWhite, int windowWidth, int windowHeight) {
    if (windowHeight < 0 || windowWidth < 0) {
        printf("input error");
        return;
    }
    int edgeX = windowWidth / 2;
    int edgeY = windowHeight / 2;
    Pixel **outputPixels = memoryPixel(blackAndWhite);
    Pixel window[windowWidth * windowHeight];
    for (int x = edgeX; x < blackAndWhite.imageHeader.width - edgeX; x++) {
        for (int y = edgeY; y < blackAndWhite.imageHeader.height - edgeY; y++) {
            int index = 0;
            for (int fx = 0; fx < windowWidth; fx++) {
                for (int fy = 0; fy < windowHeight; fy++) {
                    int pixelIndex = (x + fx - edgeX) * blackAndWhite.imageHeader.width + (y + fy - edgeY);
                    if (pixelIndex < 0 ||
                        pixelIndex >= blackAndWhite.imageHeader.width * blackAndWhite.imageHeader.height) {
                        window[index].red = 0;
                        window[index].green = 0;
                        window[index].blue = 0;
                    } else {
                        window[index] = blackAndWhite.pixels[pixelIndex];
                    }
                    index++;
                }
            }
            qsort(window, index, sizeof(Pixel), comparePixels);
            outputPixels[x][y] = window[(windowWidth * windowHeight) / 2];
            blackAndWhite.pixels[x * blackAndWhite.imageHeader.width + y] = outputPixels[x][y];
        }
        free(outputPixels[x]);
    }
    free(outputPixels);

}


void gammaConvert(BMPImage blackAndWhite, double gammaOne, double gammaTwo, double gammaThree) {

    for (int i = 0; i < blackAndWhite.imageHeader.width * blackAndWhite.imageHeader.height; i++) {
        double red = power((double) blackAndWhite.pixels[i].red / 255, (int) (1.0 / gammaOne)) * 255;
        double green = power((double) blackAndWhite.pixels[i].green / 255, (int) (1.0 / gammaTwo)) * 255;
        double blue = power((double) blackAndWhite.pixels[i].blue / 255, (int) (1.0 / gammaThree)) * 255;
        blackAndWhite.pixels[i].red = (unsigned char) min(max((int) red, 0), 255);
        blackAndWhite.pixels[i].green = (unsigned char) min(max((int) green, 0), 255);
        blackAndWhite.pixels[i].blue = (unsigned char) min(max((int) blue, 0), 255);
    }
}