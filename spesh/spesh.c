#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define DELIMS " \t\r\n"

void execute_command(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        char *args[MAX_CMD_LEN];
        char *token = strtok(command, DELIMS);
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, DELIMS);
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1) {
            perror("Command not found");
        }
        exit(1);
    } else {
        wait(NULL);
    }
}

int main() {
	printf("spesh, the Simple Program Execution SHell\n\n");
    char command[MAX_CMD_LEN];

    while (1) {
        printf("spesh >$ ");
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            if (feof(stdin)) {
                break;
            } else {
                perror("Input error");
                continue;
            }
        }

        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        }

        execute_command(command);
    }

    return 0;
}
