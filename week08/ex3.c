#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MEMORY_SIZE (10 * 1024 * 1024) // 10 MB

void printMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}

int main() {
    for (int i = 0; i < 10; i++) {
        char *ptr = (char *)malloc(MEMORY_SIZE);
        if (ptr == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }

        memset(ptr, 0, MEMORY_SIZE);
        printMemoryUsage();
        
        sleep(1);

        free(ptr);
    }

    return 0;
}

