#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

double power(double x, long n) {
    if (n == 0) return 1;
    if (n < 0) return power(1 / x, -n);
    if (n % 2) return x * power(x, n - 1);
    return power(x * x, n / 2);
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

void clearScreen() {

#if defined(__linux__)
    system("clear");
#elif defined(_WIN32)
    system("cls");
#else
    for (int i = 0; i < 32; i++)
        printf("\n");
#endif
}

void menu() {
    clearScreen();
    printf("\n1 -- CONVERT IN BLACK AND WHITE\n");
    printf("2 -- CONVERT IN NEGATIVE\n");
    printf("3 -- GAMMA CORRECTION\n");
    printf("4 -- MEDIAN FILTER\n");
    printf("0 -- EXIT\n");

}

bool check(int *number) {
    return scanf("%d", number) != 1 || getchar() != '\n';
}

void inputNumber(int *number) {
    while (check(number)) {
        printf("Enter a valid number: ");
        rewind(stdin);
    }

}

void inputDataForGamma(int *number, int *numberTwo, int *numberThree) {
    inputNumber(number);
    inputNumber(numberTwo);
    inputNumber(numberThree);

}

void inputDataForMedian(int *number, int *numberTwo) {
    inputNumber(number);
    inputNumber(numberTwo);
}