#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_subscribers>\n", argv[0]);
        exit(1);
    }

    int num_subscribers = atoi(argv[1]);

    char pipe_path[100];
    char message[1024];

    for (int i = 1; i <= num_subscribers; i++) {
        snprintf(pipe_path, sizeof(pipe_path), "./tmp/ex1/s%d", i);
        mkfifo(pipe_path, 0666);
    }

    int pipe_fds[num_subscribers];

    for (int i = 1; i <= num_subscribers; i++) {
        snprintf(pipe_path, sizeof(pipe_path), "./tmp/ex1/s%d", i);

        int pipe_fd = open(pipe_path, O_WRONLY);
        if (pipe_fd == -1) {
            perror("Error. Can't open pipe.");
            exit(1);
        }

        pipe_fds[i - 1] = pipe_fd;
    }

    while (1) {
        fgets(message, sizeof(message), stdin);

        for (int i = 0; i < num_subscribers; i++) {
            if (write(pipe_fds[i], message, strlen(message)) == -1) {
                perror("Error. Can't write in pipe.");
                exit(1);
            }
        }
    }

    return 0;
}

