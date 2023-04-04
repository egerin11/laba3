#include"tools.h"
#include<stdio.h>
#include "convertPhoto.h"
#include "minifunction.h"

int main() {
    char *fileName = NULL;
    int choice = 0;
    BMPImage *image = readImage("input.bmp");
    printDataOfPixel(image);
    do {
        menu();
        inputNumber(&choice);
        if(choice != 0) {
            getString(&fileName);
            convert(image, fileName, choice);
        }
    } while (choice != 0);
    freeMemory(image);
    return 0;

}
