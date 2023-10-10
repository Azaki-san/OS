#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_MESSAGE_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    pipe(pipe_fd);

    child_pid = fork();


    if (child_pid == 0) {
        close(pipe_fd[1]);

        char message[MAX_MESSAGE_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], message, sizeof(message))) > 0) {
            write(STDOUT_FILENO, message, bytes_read);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_fd[0]);

        char message[MAX_MESSAGE_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(STDIN_FILENO, message, sizeof(message))) > 0) {
            write(pipe_fd[1], message, bytes_read);
        }

        close(pipe_fd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

