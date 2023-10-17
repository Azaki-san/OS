#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

const char* pid_file_path = "/var/run/agent.pid";
volatile int paused = 0;

void sigusr1_handler(int signum) {
    if (signum == SIGUSR1 && paused == 0) {
        char buffer[256];
        FILE *file = fopen("text.txt", "r");
        if (file == NULL) {
            perror("Failed to open text.txt");
        } else {
            while (fgets(buffer, sizeof(buffer), file)) {
                printf("%s", buffer);
            }
            fclose(file);
        }
    }
}

void sigusr2_handler(int signum) {
    if (signum == SIGUSR2) {
        remove(pid_file_path);
        printf("Process terminating...\n");
        exit(EXIT_SUCCESS);
    }
}

void sigtstp_handler(int signum) {
    if (signum == SIGTSTP) {
        paused = 1;
        printf("Agent paused.\n");
    }
}

void sigcont_handler(int signum) {
    if (signum == SIGCONT) {
        paused = 0;
        printf("Agent resumed.\n");
    }
}

int main() {
    int pid_file = open(pid_file_path, O_WRONLY | O_CREAT, 0644);
    if (pid_file == -1) {
        perror("Failed to create /var/run/agent.pid");
        exit(EXIT_FAILURE);
    }

    dprintf(pid_file, "%d", getpid());
    close(pid_file);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGCONT, sigcont_handler);
    signal(SIGTSTP, sigtstp_handler);


    sigusr1_handler(SIGUSR1);

    while (1) {
        sleep(7200);
    }

    return EXIT_SUCCESS;
}

