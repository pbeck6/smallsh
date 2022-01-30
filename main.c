#include "shell.h"
#include "command.h"

int main() {
    int status;
    char *prompt = ": ";
    char *line = NULL;
    char command[MAXLEN_CMD+2];

    while (1) {
        // Repeat command prompt
        printf("%s", prompt);
        fflush(stdout);

        // Set MAXLEN_CMD+2 for fgets() \n and \0
        line = fgets(command, MAXLEN_CMD+2, stdin);
        if (line != NULL) {
            struct Command *newCmd = parseCmd(line);


            if (newCmd->cmd != NULL) {
                destroyCmd(newCmd);
            }
        }
    }

    return 0;
}
//