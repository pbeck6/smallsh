#include "shell.h"

int main() {
    int x = 0;
    int *status = &x;
    char *prompt = ": ";
    char *line = NULL;
    
    // Set non-valid PID to -999
    pid_t bgProcs[MAX_BGPROCS];
    for (int i = 0; i < MAX_BGPROCS; i ++) {
        bgProcs[i] = EMPTY_BGPROC;
    }

    // Run until "exit" cmd, recycle EMPTY_BGPROC as exit condition
    while (*status != EMPTY_BGPROC) {
        // Set up buffer and command struct
        char *cmdBuffer = calloc(MAXLEN_CMD+2, sizeof(char));
        struct Command *newCmd = NULL;

        // Check for terminated bg process
        checkBgChild(status);

        // Display command prompt
        printf("%s", prompt);
        fflush(stdout);

        // Set MAXLEN_CMD+2 for fgets() \n and \0
        line = fgets(cmdBuffer, MAXLEN_CMD+2, stdin);
        if (line != NULL) {
            newCmd = parseCmd(line);
            if (newCmd->cmd != NULL) {
                // Built-in "exit"
                if (strcmp(newCmd->cmd, "exit") == 0) {
                    // Set status to exit condition
                    *status = EMPTY_BGPROC;
                // Execute non-exit command
                } else {
                    status = execCmd(newCmd, status, bgProcs);
                }
            }
        }
        // Free buffer and command struct
        destroyCmd(newCmd);  
        line = NULL;
        free(cmdBuffer);
    }

    // Clean up background processes
    exitBackground(bgProcs);

    return EXIT_SUCCESS;
}
//