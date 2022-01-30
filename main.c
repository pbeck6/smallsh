#include "shell.h"
#include "command.h"

int main() {
    int status;
    char *prompt = ": ";
    char *line = NULL;
    char command[MAXLEN_CMD+2];

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        // Set MAXLEN_CMD+2 for fgets() newline and null-terminator
        line = fgets(command, MAXLEN_CMD+2, stdin);
        if (line != NULL) {
            // Strip newline from fgets(), maintain null-terminator
            line[strlen(line)-1] = '\0';

            parseCmd(line);
        }
    }

    return 0;
}
//