#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

void sigusr1_handler(int signo) {
}

void print_page_table(struct PTE *page_table, int num_pages) {
    printf("Page table\n");
    for (int i = 0; i < num_pages; ++i) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid,
               page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");
}

void print_ram(char ram[][8], int num_frames, int flag) {
    printf("RAM array\n");
    for (int i = 0; i < num_frames; ++i) {
        printf("Frame %d ---> %s\n", i, ram[i]);
    }
    if (flag == 1) {

        printf("-------------------------\n");
    }
}

void print_disk(char disk[][8], int num_pages) {
    printf("Disk array\n");
    for (int i = 0; i < num_pages; ++i) {
        printf("Page %d ----> %s\n", i, disk[i]);
    }
    printf("-------------------------\n");
}

int random_(struct PTE* page_table, int num_frames) {
    int victim_page = rand() % num_frames;
    return victim_page;
}

int nfu(struct PTE* page_table, int num_pages, int timer_interrupt) {
    static int counter = 0;

    if (counter % timer_interrupt == 0) {
        for (int i = 0; i < num_pages; i++) {
            if (page_table[i].valid) {
                page_table[i].referenced >>= 1;
            }
        }
    }

    int victim_page = -1;
    int min_referenced = INT_MAX;

    for (int i = 0; i < num_pages; i++) {
        if (page_table[i].valid) {
            if (page_table[i].referenced < min_referenced) {
                victim_page = i;
                min_referenced = page_table[i].referenced;
            }
        }
    }
    if (victim_page != -1) {
        page_table[victim_page].referenced += 1;
    }

    counter++;

    return victim_page;
}


int aging(struct PTE* page_table, int num_pages, int timer_interrupt) {
    static int counter = 0;

    if (counter % timer_interrupt == 0) {
        for (int i = 0; i < num_pages; i++) {
            if (page_table[i].valid) {
                page_table[i].referenced >>= 1;
                if (page_table[i].referenced) {
                    page_table[i].referenced |= (1 << 7);
                }
            }
        }
    }

    int victim_page = -1;
    int min_counter = INT_MAX;

    for (int i = 0; i < num_pages; i++) {
        if (page_table[i].valid) {
            if (page_table[i].referenced < min_counter) {
                victim_page = i;
                min_counter = page_table[i].referenced;
            }
        }
    }
    
    if (victim_page != -1) {
        page_table[victim_page].referenced = 0;
    }

    counter++;

    return victim_page;
}





int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_pages> <number_of_frames> <algorithm>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_pages = atoi(argv[1]);
    int num_frames = atoi(argv[2]);
    char *algorithm = argv[3];

    if (num_frames >= num_pages) {
        fprintf(stderr, "Number of frames must be less than the number of pages.\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening pagetable file");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, num_pages * sizeof(struct PTE));

    struct PTE *page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping pagetable file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_pages; ++i) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    signal(SIGUSR1, sigusr1_handler);

    printf("pager process\n");
    printf("-------------------------\n");
    printf("Initialized page table\n");
    print_page_table(page_table, num_pages);

    char ram[num_frames][8];
    for (int i = 0; i < num_frames; i++) {
        for (int j = 0; j < 6; j++) {
            ram[i][j] = '\0';
        }
    }
    char disk[num_pages][8];

    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("Error opening /dev/random");
        exit(1);
    }
    char c;
    for (int i = 0; i < num_pages; i++) {
        for (int j = 0; j < 7; j++) {
            disk[i][j] = 'A' + rand() % 58;
        }
        disk[i][7] = '\0';
    }

    printf("Initialized RAM\n");
    print_ram(ram, num_frames, 1);

    printf("Initialized disk\n");
    print_disk(disk, num_pages);
    int count = 1;
    while (1) {
        pause();
        int victim_page = -1;

        int mmuPID;
        bool isExit = false;
        for (int i = 0; i < num_pages; ++i) {
            if (page_table[i].referenced != 0) {
                printf("A disk access request from MMU Process (pid=%d)\n", page_table[i].referenced);
                mmuPID = page_table[i].referenced;
                victim_page = i;
                printf("Page %d is referenced\n", victim_page);
                isExit = true;
                break;
            }
        }

        if (isExit == false) {
            break;
        }

        if (victim_page != -1) {
            if (page_table[victim_page].frame == -1) {
                int victim_page_ram = -1;
                for (int i = 0; i < num_frames; i++) {
                    if (ram[i][0] == '\0') {
                        printf("We can allocate it to free frame %d\n", i);
                        victim_page_ram = i;
                        break;
                    }
                }
                if (victim_page_ram == -1) {
                    printf("We do not have free frames in RAM\n");
                    if (strcmp(algorithm, "random") == 0) {
                        victim_page_ram = random_(page_table, num_frames);
                    } else if (strcmp(algorithm, "nfu") == 0) {
                        victim_page_ram = nfu(page_table, num_pages, 10);
                        printf("I'M HEREEEE: ");
                        printf("%d\n", victim_page_ram);
                    } else if (strcmp(algorithm, "aging") == 0) {
                        victim_page_ram = aging(page_table, num_pages, 10);
                    } else {
                        fprintf(stderr, "Invalid algorithm specified\n");
                        exit(EXIT_FAILURE);
                    }

                    int delete_victim_page = -1;
                    for (int i = 0; i < num_pages; i++) {
                        if (strcmp(disk[i], ram[victim_page_ram]) == 0) {
                            delete_victim_page = i;
                            page_table[delete_victim_page].valid = false;
                            page_table[delete_victim_page].frame = -1;
                            page_table[delete_victim_page].dirty = false;
                            break;
                        }
                    }
                    if (delete_victim_page == -1) {
                        printf("SOmething went wrong. CHeck code please.\n");
                    }
                    printf("Choose a random victim page %d\n", delete_victim_page);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", victim_page, victim_page_ram);
                }

                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", victim_page, victim_page_ram);
                strcpy(ram[victim_page_ram], disk[victim_page]);
                page_table[victim_page].valid = true;
                page_table[victim_page].frame = victim_page_ram;
                page_table[victim_page].referenced = 0;
            }

            print_ram(ram, num_frames, 0);
            printf("%d disk accesses in total\n", count);
            kill(mmuPID, SIGCONT);
            printf("Resume MMU process\n");
            count++;
            printf("-------------------------\n");
        } else {
            break;
        }
    }


    munmap(page_table, num_pages * sizeof(struct PTE));
    close(fd);
    unlink("/tmp/ex2/pagetable");

    printf("Pager is terminated\n");
    return 0;
}
