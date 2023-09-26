#include <stdio.h>
#include <stdlib.h>

const int x = 1;

int const_tri(int* p, int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1 || n == 2) {
        return 1;
    } else {
        for (int i = 0; i <= n; i++) {
            int temp = p[0] + p[1] + p[2];
            p[0] = p[1];
            p[1] = p[2];
            p[2] = temp;
        }
        return p[2];
    }
}

int main() {
    int* q = &x;

    int* p = (int*)malloc(3 * sizeof(int));
    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * *q;
    printf("Memory addresses of p cells:\n");
    printf("Cell 1: %p\n", &p[0]);
    printf("Cell 2: %p\n", &p[1]);
    printf("Cell 3: %p\n", &p[2]);
    if (&p[2] - &p[1] == 1 && &p[1] - &p[0] == 1) {
        printf("Cells are contiguous!\n");
    } else {
        printf("Cells are not contiguous!");
    }

    printf("Tribonacci number. Example: n = 36\n");
    printf("Value received from function const_tri: %d\n", const_tri(p, 36));
    printf("Value of p[2]: %d\n", p[2]);

    free(p);

    return 0;
}


