#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <sys/stat.h>
#define FILE_SIZE (500 * 1000 * 1000)

int main() {
    int fd = open("text.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, 0);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("open");
        close(fd);
        exit(1);
    }

    int char_count = 0;
    off_t file_size = 0;

    while (file_size < FILE_SIZE) {
        char c;
        if (read(random_fd, &c, 1) != 1) {
            perror("Failed to read text.txt");
            close(fd);
            close(random_fd);
            exit(1);
        }

        if (isprint(c)) {
            if (write(fd, &c, 1) != 1) {
                perror("Failed to write text.txt");
                close(fd);
                close(random_fd);
                exit(1);
            }
            char_count++;

            if (char_count >= 1024) {
                if (write(fd, "\n", 1) != 1) {
                    perror("Failed to write text.txt");
                    close(fd);
                    close(random_fd);
                    exit(1);
                }
                file_size++;
                char_count = 0;
            }
            file_size++;
        }
    }

    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = page_size * 1024;
    off_t offset = 0;
    int capital_count = 0;

    while (offset < FILE_SIZE) {
        char *mmapped_data = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);

        if (mmapped_data == MAP_FAILED) {
            perror("Failed MMAP");
            close(fd);
            close(random_fd);
            exit(1);
        }

        for (off_t i = 0; i < chunk_size; i++) {
            if (i < FILE_SIZE - offset) {
                if (isupper(mmapped_data[i])) {
                    capital_count++;
                    mmapped_data[i] = tolower(mmapped_data[i]);
                }
            }
        }
        printf("Chunk finished\n");

        munmap(mmapped_data, chunk_size);
        offset += chunk_size;
    }

    printf("Total capital letters: %d\n", capital_count);
    close(fd);
    close(random_fd);

    return EXIT_SUCCESS;
}
