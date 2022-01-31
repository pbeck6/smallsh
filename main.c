#include "shell.h"
#include "command.h"

int main() {
    int status = 0;
    char *prompt = ": ";
    char *line = NULL;

    // Run until "exit" cmd
    while (1) {
        // Set up buffer
        char *cmdBuffer = calloc(MAXLEN_CMD+2, sizeof(char));

        // Display command prompt
        printf("%s", prompt);
        fflush(stdout);

        // Set MAXLEN_CMD+2 for fgets() \n and \0
        line = fgets(cmdBuffer, MAXLEN_CMD+2, stdin);
        if (line != NULL) {
            struct Command *newCmd = parseCmd(line);
            status = execCmd(newCmd, status);
            // Free command struct dynamic memory
            destroyCmd(newCmd);       
        }
        // Free buffer
        line = NULL;
        free(cmdBuffer);
    }

    return 0;
}
//