#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_SIZE 10000000

unsigned int memory[MEMORY_SIZE];

void init_memory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
}

void clear(unsigned int adrs) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

void allocate_first_fit(unsigned int adrs, int size) {
    int count = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            count++;
            if (count == size) {
                for (int j = i - size + 1; j <= i; j++) {
                    memory[j] = adrs;
                }
                return;
            }
        } else {
            count = 0;
        }
    }
}

void allocate_best_fit(unsigned int adrs, int size) {
    int bestFitIndex = -1;
    int bestFitSize = MEMORY_SIZE;
    int currentFit = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            currentFit++;
        } else {
            if (currentFit >= size && currentFit < bestFitSize) {
                bestFitSize = currentFit;
                bestFitIndex = i - currentFit;
            }
            currentFit = 0;
        }
    }

    if (currentFit >= size && currentFit < bestFitSize) {
        bestFitIndex = MEMORY_SIZE - currentFit;
    }

    if (bestFitIndex != -1) {
        for (int i = bestFitIndex; i < bestFitIndex + size; i++) {
            memory[i] = adrs;
        }
    }
}

void allocate_worst_fit(unsigned int adrs, int size) {
    int worstFitIndex = -1;
    int worstFitSize = 0;
    int currentFit = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            currentFit++;
        } else {
            if (currentFit >= size && currentFit > worstFitSize) {
                worstFitSize = currentFit;
                worstFitIndex = i - currentFit;
            }
            currentFit = 0;
        }
    }

    if (currentFit >= size && currentFit > worstFitSize) {
        worstFitIndex = MEMORY_SIZE - currentFit;
    }

    if (worstFitIndex != -1) {
        for (int i = worstFitIndex; i < worstFitIndex + size; i++) {
            memory[i] = adrs;
        }
    }
}

int main() {
    init_memory();
    for (int i = 0; i < 3; i++) {
        FILE *file = fopen("/home/andrew/CLionProjects/week07/queries.txt", "r");
        if (file == NULL) {
            perror("Error opening the file");
            return EXIT_FAILURE;
        }
        char line[50];
        clock_t start = clock();
        int totalCount = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strcmp(line, "end\n") == 0) {
                break;
            } else {
                char query[9];
                unsigned int adrs;
                int size;
                if (line[0] == 'a') {
                    sscanf(line, "%s %u %d\n", query, &adrs, &size);
                    if (i == 0) {
                        allocate_first_fit(adrs, size);
                    } else if (i == 1) {
                        allocate_best_fit(adrs, size);
                    } else if (i == 2) {
                        allocate_worst_fit(adrs, size);
                    }
                } else if (line[0] == 'c') {
                    sscanf(line, "%s %u\n", query, &adrs);
                    clear(adrs);
                }
                totalCount++;
            }
        }
        clock_t end = clock();
        fclose(file);

        double totalTime = (double) (end - start) / CLOCKS_PER_SEC;
        double throughput = totalCount / totalTime;
        printf("Throughput: %.2f queries/second\n", throughput);
        init_memory();
    }
    return EXIT_SUCCESS;
}



