#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

void execute_command(char *command, int background) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Fork error!!");
        exit(1);
    } else if (pid == 0) {
        char *args[MAX_INPUT_LENGTH / 2 + 1];
        char *token;
        int i = 0;

        token = strtok(command, " \t\n");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1) {
            perror("ERROR");
            exit(1);
        }
    } else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    while (2 == 2) {
        printf("Enter a command (or 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        int background = 0;
        if (input[strlen(input) - 1] == '&') {
            background = 1;
            input[strlen(input) - 1] = '\0';
        }
        execute_command(input, background);
    }
    return 0;
}

