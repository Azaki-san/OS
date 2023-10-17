#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

pid_t get_agent_pid() {
    pid_t agent_pid = -1;
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        return -1;
    }
    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);
    return agent_pid;
}

int main() {
    pid_t agent_pid = get_agent_pid();

    if (agent_pid == -1) {
        printf("Error: No agent found.\n");
        exit(1);
    }

    printf("Agent found.\n");

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        char command[16];
        scanf("%s", command);

        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGTSTP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        }
    }

    return EXIT_SUCCESS;
}

