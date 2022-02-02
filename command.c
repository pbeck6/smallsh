#include "command.h"

void destroyCmd(struct Command *oldCmd) {
    if (oldCmd != NULL) {
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
}

void expandDoubleDollar(char *line, char *res){
    int i = 0;
    int lineLen = strlen(line);
    
    // Convert pid to a string
    pid_t processId = getpid();
    int len = snprintf(NULL, 0, "%d", processId);
    char *pidString = calloc(len+1, sizeof(char));
    snprintf(pidString, len+1, "%d", processId);

    while (i < lineLen && strlen(res) < MAXLEN_CMD) {
        // line[i+1] NOT out of index, will be '\0' at end
        if (line[i] == '$' && line[i+1] == '$') {
            // Expand $$ variable
            strcat(res, pidString);
            i++;
        } else {
            // Concatenate single char
            strncat(res, &line[i], 1);
        }
        i++;
    }

    free(pidString);
}

struct Command *initCmd(void) {
    struct Command *newCmd = calloc(1, sizeof(struct Command));

    newCmd->cmd = NULL;
    // Initialize to array of NULL char pointers
    for (int i = 0; i < MAXARGS_CMD; i++) {
        newCmd->args[i] = NULL;
    }
    newCmd->inputFile = NULL;
    newCmd->outputFile = NULL;
    newCmd->nArgs = 0;
    newCmd->bg = 0;

    return newCmd;
}

struct Command *parseCmd(char *oldLine) {
    // Buffer for $$ expanded command str
    char *line = calloc(MAXLEN_CMD+1, sizeof(char));

    // Define command struct
    struct Command *newCmd = initCmd();

    // Strip newline from fgets(), maintain null-terminator
    oldLine[strlen(oldLine)-1] = '\0';

    // Ignore empty lines and comments
    if (strlen(oldLine) > 0 && oldLine[0] != '#') {
        // Expand $$ variables, if any
        expandDoubleDollar(oldLine, line);

        // The first token is the command
        char *token = strsep(&line, " ");
        newCmd->cmd = calloc(strlen(token)+1, sizeof(char));
        strcpy(newCmd->cmd, token);

        // Read optional args
        token = strsep(&line, " ");
        while (token != NULL) {
            // Arg for input_file
            if (strcmp(token, "<") == 0) {
                token = strsep(&line, " ");
                newCmd->inputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->inputFile, token);
            // Arg for output_file
            } else if (strcmp(token, ">") == 0) {
                token = strsep(&line, " ");
                newCmd->outputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->outputFile, token);
            // [arg 1, arg2, ...]
            } else if (strcmp(token, "&") != 0){
                newCmd->args[newCmd->nArgs] = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCmd->args[newCmd->nArgs], token);
                newCmd->nArgs++;
            // Potential [&] for bg process
            } else {
                newCmd->bg = 1;
            }
            // Check for EOF
            token = strsep(&line, " ");
            // Reset bg if not EOF, treat as arg
            if (token != NULL && newCmd->bg == 1) {
                newCmd->bg = 0;
                char *regular = "&";
                newCmd->args[newCmd->nArgs] = calloc(strlen(regular)+1, sizeof(char));
                strcpy(newCmd->args[newCmd->nArgs], regular);
                newCmd->nArgs++;
            }
        }
    }
    ////////////////////////////////DELETE
    printCmd(newCmd);

    // Free buffer
    free(line);
    
    return newCmd;
}

void printCmd(struct Command *cmd) {
    if (cmd->cmd != NULL) {
        printf("Cmd %s of len %zu\n", cmd->cmd, strlen(cmd->cmd));
        fflush(stdout);
    }

    printf("%d args passed\n", cmd->nArgs);
    fflush(stdout);

    for (int i = 0; i < MAXARGS_CMD; i++) {
        if (cmd->args[i] != NULL) {
            printf("Arg %s of len %zu\n", cmd->args[i], strlen(cmd->args[i]));
            fflush(stdout);
        }
    }

    if (cmd->inputFile != NULL) {
        printf("Input %s of len %zu\n", cmd->inputFile, strlen(cmd->inputFile));
        fflush(stdout);
    }

    if (cmd->outputFile != NULL) {
        printf("Output %s of len %zu\n", cmd->outputFile, strlen(cmd->outputFile));
        fflush(stdout);
    }

    if (cmd->bg) {
        printf("Background activated.\n");
        fflush(stdout);
    }
}
//