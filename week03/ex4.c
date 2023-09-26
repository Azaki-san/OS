#include <stdio.h>
#include <float.h>
#include <limits.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    char* current = (char*)base;
    void* res = initial_value;

    for (int i = 0; i < n; i++) {
        res = opr(res, (void*)current);
        current += size;
    }

    return res;
}

void* addition(const void* a, const void* b) {
    double* res = (double*)a;
    double* operand = (double*)b;
    *res += *operand;
    return res;

}

void* multiplication(const void* a, const void* b) {
    double* res = (double*)a;
    double* operand = (double*)b;
    *res *= *operand;
    return res;
}

void* intMultiplication(const void* a, const void* b) {
    int* res = (int*)a;
    int* operand = (int*)b;
    *res *= *operand;
    return res;
}

void* max_operation(const void* a, const void* b) {
    double* res = (double*)a;
    double* operand = (double*)b;
    if (*(double*)a < *(double*)b) {
        *res = *operand;
    }
    return res;
}

void* max_operationInt(const void* a, const void* b) {
    int* res = (int*)a;
    int* operand = (int*)b;
    if (*(int*)a < *(int*)b) {
        *res = *operand;
    }
    return res;
}
int main() {
    double doubleArray[] = {1.5, 2.5, 3.5, 4.5, 5.5};
    int intArray[] = {5, 6, 12, 24, 42};

    int* sumIntArray = (int*) aggregate(intArray, sizeof(int), 5, (void*)&(double){0.0}, addition);
    printf("Sum of int array: %d\n", *sumIntArray);

    int* productIntArray = (int*) aggregate(intArray, sizeof(int), 5, (void*)&(int){1}, intMultiplication);
    printf("Product of int array: %d\n", *productIntArray);

    int* maxInt = (int*) aggregate(intArray, sizeof(int), 5, (void*)&(int){INT_MIN}, max_operationInt);
    printf("Max value in int array: %d\n", *maxInt);

    double* sumDoubleArray = (double*) aggregate(doubleArray, sizeof(double), 5, (void*)&(double){1.0}, addition);
    printf("Sum of double array: %f\n", *sumDoubleArray);

    double* multiplyDoubleArray = (double*) aggregate(doubleArray, sizeof(double), 5, (void*)&(double){1.0}, multiplication);
    printf("Product of double array: %f\n", *multiplyDoubleArray);

    double* maxDouble = (double*) aggregate(doubleArray, sizeof(double), 5, (void*)&(double){DBL_MIN}, max_operation);
    printf("Max value in double array: %f\n", *maxDouble);

    return 0;
}
