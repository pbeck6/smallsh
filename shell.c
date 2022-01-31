#include "shell.h"

void changeDir(char *path) {
    // New dir specified
    if (path != NULL) {
        if (chdir(path) == -1) {
            printf("Directory error. Try again.\n");
            fflush(stdout);
        }
    // No dir specified
    } else {
        chdir(getenv("HOME"));
    }
}

int execCmd(struct Command *cmd, int status) {
    if (cmd->cmd != NULL) {
        // Built-in "exit"
        if (strcmp(cmd->cmd, "exit") == 0) {
            exit(0);
        // Built-in "status"
        } else if (strcmp(cmd->cmd, "status") == 0) {
            printStatus(status);
        // Built-in "cd"
        } else if (strcmp(cmd->cmd, "cd") == 0) {
            changeDir(cmd->args[0]);
        // Other commands
        } else {
            //////////////////////
        }
    }
    return status;
}

void printStatus(int status) {
    if (status == 0 || status == 1) {
        printf("exit value %d\n", status);
        fflush(stdout);
    } else {
        printf("terminated by signal %d\n", status);
        fflush(stdout); 
    }
}