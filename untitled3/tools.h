
#ifndef LABA3__TOOLS_H
#define LABA3__TOOLS_H
#include"struct.h"
#include<stdio.h>
void getString(char **string);
BMPImage *readImage(const char *fileName);
void printDataOfPixel(const BMPImage *image);
void freeMemory(BMPImage *image);
void readPixels(FILE *file,const BMPImage *image);
#endif
