#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>

int main() {
    clock_t start_time = clock();

    printf("Main process ID: %d\n", getpid());
    printf("Main parent ID: %d\n", getppid());

    pid_t child1 = fork();
    if (child1 < 0) {
        printf("Error. Cannot create first child process.\n");
    } else if (child1 == 0) {
        clock_t child1_start_time = clock();

        printf("First child process ID: %d\n", getpid());
        printf("First child parent ID: %d\n", getppid());

        clock_t child1_end_time = clock();
        double exTime = (double)(child1_end_time - child1_start_time) / CLOCKS_PER_SEC * 1000;
        printf("First child execution time: %.2f ms\n", exTime);
        exit(0);
    } else {
        pid_t child2 = fork();
        if (child2 < 0) {
            printf("Error. Cannot create second child process.\n");
        } else if (child2 == 0) {

            clock_t child2_start_time = clock();

            printf("Second child process ID: %d\n", getpid());
            printf("Second child parent ID: %d\n", getppid());

            clock_t child2_end_time = clock();
            double exTime = (double) (child2_end_time - child2_start_time) / CLOCKS_PER_SEC * 1000;
            printf("Second child execution time: %.2f ms\n", exTime);
            exit(0);
        } else {
            wait(NULL);
            wait(NULL);
            clock_t end_time = clock();
            double exTime = (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000;
            printf("Main process execution time: %.2f ms\n", exTime);
        }
    }
    return 0;
}
