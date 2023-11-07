#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

int num_hits = 0;
int num_misses = 0;


void sigcont_handler(int signo) {
    printf("MMU resumed by SIGCONT signal from pager\n");
}

void print_page_table(struct PTE *page_table, int num_pages, int flag) {
    printf("Page table\n");
    for (int i = 0; i < num_pages; ++i) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid,
               page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    if (flag == 1) {
        printf("-------------------------\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_pages> <reference_string> <pager_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_pages = atoi(argv[1]);
    char *reference_string = argv[2];
    int pager_pid = atoi(argv[3]);

    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("Error opening pagetable file");
        exit(EXIT_FAILURE);
    }

    struct PTE *page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping pagetable file");
        exit(EXIT_FAILURE);
    }

    signal(SIGCONT, sigcont_handler);

    printf("mmu process\n");
    printf("-------------------------\n");
    printf("Initialized page table\n");
    print_page_table(page_table, num_pages, 1);
    char *token = strtok(reference_string, " ");
    while (token != NULL) {
        char mode = token[0];
        int page_number = atoi(token + 1);

        printf("%s Request for page %d\n", (mode == 'R') ? "Read" : "Write", page_number);

        if (!page_table[page_number].valid) {
            // Page fault
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page_number].referenced = getpid();
            kill(pager_pid, SIGUSR1);
            pause();
            num_misses++;
        } else {
            num_hits++;
            printf("It is a valid page\n");
        }

        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            page_table[page_number].dirty = true;
        }

        print_page_table(page_table, num_pages, 0);
        printf("-------------------------\n");

        token = strtok(NULL, " ");
    }

    if (kill(pager_pid, SIGUSR1) == -1) {
        perror("Error signaling pager");
        exit(EXIT_FAILURE);
    }

    munmap(page_table, num_pages * sizeof(struct PTE));
    close(fd);
    float hit_ratio = (float)num_hits / (num_hits + num_misses);
    printf("Hit Ratio: %.2f\n", hit_ratio);


    printf("Done all requests\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");
    return 0;
}
