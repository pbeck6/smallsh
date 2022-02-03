#include "shell.h"

/* Atomic for sig handler */
volatile sig_atomic_t flag = 0;

/* Function declarations */
void foregroundOn(int);
void foregroundOff(int);

int main() {
    int x = 0;
    int *status = &x;
    char *prompt = ": ";
    char *line = NULL;

    // Code modified from Exploration: Sig Handling API
    struct sigaction SIG_parent = { {0} };
    SIG_parent.sa_handler = SIG_IGN;
    sigfillset(&SIG_parent.sa_mask);
    SIG_parent.sa_flags = 0;
    sigaction(SIGINT, &SIG_parent, NULL);
    
    // Set non-valid PID to -999
    pid_t bgProcs[MAX_BGPROCS];
    for (int i = 0; i < MAX_BGPROCS; i ++) {
        bgProcs[i] = EMPTY_BGPROC;
    }

    // Run until "exit" cmd, recycle EMPTY_BGPROC as exit condition
    while (*status != EMPTY_BGPROC) {
        // Reset SIGTSTP handler
        if (!flag) {
	        SIG_parent.sa_handler = foregroundOn;
        } else {
            SIG_parent.sa_handler = foregroundOff;
        }
        sigaction(SIGTSTP, &SIG_parent, NULL);

        // Set up buffer and command struct
        char *cmdBuffer = calloc(MAXLEN_CMD+2, sizeof(char));
        struct Command *newCmd = NULL;

        // Check for terminated bg process
        checkBgChild(status, bgProcs);

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
                    // Check for foreground-only mode
                    if (flag) {
                        newCmd->bg = 0;
                    }
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

void foregroundOn(int signo) {
    flag = 1;
	char* message = "\nEntering foreground-only mode (& is now ignored)\n";
	write(STDOUT_FILENO, message, 50);
}

void foregroundOff(int signo) {
    flag = 0;
    // Wait for last foreground process, if any
    wait(NULL);
	char* message = "\nExiting foreground-only mode\n";
	write(STDOUT_FILENO, message, 30);
}
//