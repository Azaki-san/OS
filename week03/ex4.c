#include <stdio.h>
#include <float.h>

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

void* max_operation(const void* a, const void* b) {
    double* res = (double*)a;
    double* operand = (double*)b;
    if (*(double*)a < *(double*)b) {
        *res = *operand;
    }
    return res;
}
int main() {


    double doubleArray[] = {1.0, 3.4, 5.8, 7.0, 9.9};
    int intArray[] = {5, 6, 12, 24, 42};

    int* sumIntArray = (int*) aggregate(intArray, sizeof(int), 5, (void*)&(double){0.0}, addition);
    printf("Sum of int array: %d\n", *sumIntArray);

    double* multiplyDoubleArray = (double*) aggregate(doubleArray, sizeof(double), 5, (void*)&(double){1.0}, multiplication);
    printf("Product of double array: %f\n", *multiplyDoubleArray);

    double* maxDouble = (double*) aggregate(doubleArray, sizeof(double), 5, (void*)&(double){DBL_MIN}, max_operation);
    printf("Max value in double array: %f\n", *maxDouble);


    return 0;
}
