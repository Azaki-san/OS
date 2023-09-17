#include <stdio.h>
#include <stdlib.h>

int firstPattern(int n) {
    printf("First pattern!\n");
    char *stars = malloc(1);
    stars[0] = '\0';
    for (int i = 0; i < n; i++) {
        stars = realloc(stars, i + 2);
        stars[i] = '*';
        stars[i + 1] = '\0';
        printf("%s\n", stars);
    }
    return 0;
}

int secondPattern(int n) {
    printf("\nSecond pattern!\n");
    char *stars = malloc(1);
    stars[0] = '\0';
    for (int i = 0; i < n; i++) {
        if (i < n / 2 || (i == n / 2 && n % 2 != 0)) {
            stars = realloc(stars, i + 2);
            stars[i] = '*';
            stars[i + 1] = '\0';
        } else {
            stars = realloc(stars, n - i + 1);
            stars[n - i] = '\0';
        }
        printf("%s\n", stars);
    }
    return 0;
}

int thirdPattern(int n) {
    printf("\nThird pattern!\n");
    char *stars = malloc(n + 1);
    stars[n] = '\0';
    for (int i = 0; i < n; i++) {
        stars[i] = '*';
    }
    for (int i = 0; i < n; i++) {
        printf("%s\n", stars);
    }
    return 0;
}

int main(void) {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);
    firstPattern(n);
    secondPattern(n);
    thirdPattern(n);
    return 0;
}
