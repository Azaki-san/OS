#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file == NULL) {
        perror("Error opening /tmp/ex1.pid");
        exit(1);
    }
    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);

    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("Error opening /dev/random");
        exit(1);
    }

    char password[13];
    strcpy(password, "pass:");
    int count = 0;
    while (count < 8) {
        char c;
        if (read(random_fd, &c, 1) != 1) {
            perror("Failed.");
            exit(EXIT_FAILURE);
        }

        if (isprint(c)) {
            password[5 + count] = c;
            count++;
        }
    }
    close(random_fd);

    char *password_ptr = mmap(NULL, 13, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (password_ptr == MAP_FAILED) {
        perror("Error mapping memory");
        exit(EXIT_FAILURE);
    }

    strcpy(password_ptr, password);
    while (1) {
        sleep(1);
    }

    munmap(password_ptr, 13);

    return 0;
}
