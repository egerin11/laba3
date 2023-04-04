#ifndef LABA3__CONVERTPHOTO_H
#define LABA3__CONVERTPHOTO_H

#include "struct.h"

void convert(BMPImage *oldImage, const char *fileName, int choice);

void switchForConvert(int choice, int dataSize, BMPImage *oldImage, BMPImage blackAndWhite);

void convertBlackAndWhite(const BMPImage *oldImage, BMPImage blackAndWhite, int dataSize);

void convertNegative(const BMPImage *oldImage, BMPImage blackAndWhite, int dataSize);

int comparePixels(const void *a, const void *b);

Pixel **memoryPixel(BMPImage blackAndWhite);
void medianFilter(BMPImage blackAndWhite, int imageWidth, int imageHeight);


void gammaConvert(BMPImage blackAndWhite, double gammaOne, double gammaTwo, double gammaThree);


#endif
