#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define VECTOR_SIZE 120
#define resFile "temp.txt"

int generateNumber() {
    return rand() % 100;
}

int dotProduct(const int* vector1, const int* vector2, int startIndex, int stopIndex) {
    int result = 0;
    for (int i = startIndex; i < stopIndex; i++) {
        result += (vector1[i] * vector2[i]);
    }
    return result;
}

int main() {
    srand(time(NULL));
    int u[VECTOR_SIZE];
    int v[VECTOR_SIZE];
    int n;
    for (int i = 0; i < VECTOR_SIZE; i++) {
        u[i] = generateNumber();
        v[i] = generateNumber();
    }
    printf("Enter N: ");
    scanf("%d", &n);
    if (n % 2 != 0 || n <= 0) {
        printf("Invalid N.\n");
    } else {
        FILE* file = fopen(resFile, "w+");
        int elementsPerProcess = VECTOR_SIZE / n;
        for (int i = 0; i < n; i++) {
            pid_t child = fork();
            if (child == 0) {
                fprintf(file, "%d\n", dotProduct(v, u, i * elementsPerProcess,
                                                 elementsPerProcess * (i + 1)));
                fclose(file);
                exit(0);
            }
        }

        for (int i = 0; i < n; i++) {
            wait(NULL);
        }
        rewind(file);
        int res = 0;
        int notRes;
        for (int i = 0; i < n; i++) {
            fscanf(file, "%d\n", &notRes);
            res += notRes;
        }

        printf("Final result of dot product: %d\n", res);
        fprintf(file, "Final result of dot product: %d\n", res);
        fclose(file);
    }
    return 0;
}
