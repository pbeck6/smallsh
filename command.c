#include "command.h"

struct Command *initCmd(void) {
    struct Command *newCmd = malloc(sizeof(struct Command));
    
    for (int i = 0; i < MAXARGS_CMD; i++) {
        newCmd->args[i] = NULL;
    }
    newCmd->inputFile = NULL;
    newCmd->outputFile = NULL;
    newCmd->bg = 0;

    return newCmd;
}

void parseCmd(char *line) {
    // Define command struct
    struct Command *newCmd = initCmd();

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

            // Arg for output_file
            } else if (strcmp(token, ">") == 0) {

            // [arg 1, arg2, ...]
            } else if (strcmp(token, "&") != 0){

            // Potential [&] arg
            } else {
                newCmd->bg = 1;
            }

            // Check for end of file
            token = strtok(NULL, " ");
            if (token != NULL && newCmd->bg == 1) {
                newCmd->bg = 0;
            }
        }

        if (newCmd->bg) {
            printf("BACKground\n");
        }
        printf("%s", line);
        printf("%zu\n", strlen(line));
        printf("%s", newCmd->cmd);
        printf("%zu\n", strlen(newCmd->cmd));
    }
}
//