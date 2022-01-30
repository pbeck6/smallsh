#include "shell.h"
#include "command.h"

int main() {
    int status;
    char *prompt = ": ";
    char *line = NULL;

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
            ////////////////////////////////////////

            // Free command struct dynamic memory
            if (newCmd->cmd != NULL) {
                destroyCmd(newCmd);       
            }
        }
        // Free buffer
        line = NULL;
        free(cmdBuffer);
    }

    return 0;
}
//