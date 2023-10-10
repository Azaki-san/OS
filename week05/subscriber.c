#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <subscriber_index>\n", argv[0]);
        exit(1);
    }

    int subscriber_index = atoi(argv[1]);

    char pipe_path[100];
    char message[1024];

    snprintf(pipe_path, sizeof(pipe_path), "./tmp/ex1/s%d", subscriber_index);

    int pipe_fd = open(pipe_path, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error. Can't open named pipe.");
        exit(1);
    }

    while (1) {
        ssize_t bytes_read = read(pipe_fd, message, sizeof(message));

        if (bytes_read == -1) {
            perror("Error. Can't read named pipe.");
            break;
        } else if (bytes_read == 0) {
            printf("Publisher finished broadcasting.\n");
            break;
        }

        printf("%.*s", (int)bytes_read, message);
    }

    close(pipe_fd);

    return 0;
}

