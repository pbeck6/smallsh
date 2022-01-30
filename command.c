#include "command.h"

void destroyCmd(struct Command *oldCmd) {
    if (oldCmd->cmd != NULL) {
        free(oldCmd->cmd);
    }
    for (int i = 0; i < MAXARGS_CMD; i++) {
        if (oldCmd->args[i] != NULL) {
            free(oldCmd->args[i]);
        }
    }
    if (oldCmd->inputFile != NULL) {
        free(oldCmd->inputFile);
    }
    if (oldCmd->outputFile != NULL) {
        free(oldCmd->outputFile);
    }
    free(oldCmd);
}

struct Command *initCmd(void) {
    struct Command *newCmd = malloc(sizeof(struct Command));

    newCmd->cmd = NULL;
    for (int i = 0; i < MAXARGS_CMD; i++) {
        newCmd->args[i] = NULL;
    }
    newCmd->inputFile = NULL;
    newCmd->outputFile = NULL;
    newCmd->bg = 0;

    return newCmd;
}

struct Command *parseCmd(char *line) {
    // Define command struct
    struct Command *newCmd = initCmd();
    int argIndex = 0;

    // Strip newline from fgets(), maintain null-terminator
    line[strlen(line)-1] = '\0';

    // Ignore empty lines and comments
    if (strlen(line) > 0 && line[0] != '#') {
        // The first token is the command
        char *token = strtok(line, " ");
        newCmd->cmd = calloc(strlen(token)+1, sizeof(char));
        strcpy(newCmd->cmd, token);

        // Read optional args
        token = strtok(NULL, " ");
        while (token != NULL) {
            // Arg for input_file
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " ");
                newCmd->inputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->inputFile, token);
            // Arg for output_file
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " ");
                newCmd->outputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->outputFile, token);
            // [arg 1, arg2, ...]
            } else if (strcmp(token, "&") != 0){
                newCmd->args[argIndex] = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->args[argIndex], token);
                argIndex++;
            // Potential [&] arg
            } else {
                newCmd->bg = 1;
            }
            // Check for EOF, reset bg if not
            token = strtok(NULL, " ");
            if (token != NULL && newCmd->bg == 1) {
                newCmd->bg = 0;
            }
        }
    }
    printCmd(newCmd);
    return newCmd;
}

void printCmd(struct Command *cmd) {
    if (cmd->cmd != NULL) {
        printf("Cmd %s of len %zu\n", cmd->cmd, strlen(cmd->cmd));
    }

    for (int i = 0; i < MAXARGS_CMD; i++) {
        if (cmd->args[i] != NULL) {
            printf("Arg %s of len %zu\n", cmd->args[i], strlen(cmd->args[i]));
        }
    }

    if (cmd->inputFile != NULL) {
        printf("Input %s of len %zu\n", cmd->inputFile, strlen(cmd->inputFile));
    }

    if (cmd->outputFile != NULL) {
        printf("Output %s of len %zu\n", cmd->outputFile, strlen(cmd->outputFile));
    }

    if (cmd->bg) {
        printf("Background activated.\n");
    }
}
//