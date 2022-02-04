#include "shell.h"

/* Change current directory to that of "path"
*  Change to HOME if no "path" specified
*/
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

/* Check PIDs from array "bgProcs" for status change
*  Remove finished PIDs from "bgProcs" and then
*  set status in int ptr "status"
*/
void checkBgChild(int *status, pid_t *bgProcs) {
    int termStatus;
    pid_t termChild;

    // Check array of PIDs
    for (int i = 0; i < MAX_BGPROCS; i++) {
        // Check whether valid PID has status change
        if (bgProcs[i] != EMPTY_BGPROC) {
            termChild = waitpid(bgProcs[i], &termStatus, WNOHANG);
            if (termChild > 0) {
                // Remove child from bg proc array
                bgProcs[i] = EMPTY_BGPROC;
                
                // Set child's termination status
                if( WIFEXITED(termStatus) ){
                    *status = WEXITSTATUS(termStatus);
                } else {
                    *status = WTERMSIG(termStatus);
                }

                // Display message before cmd prompt
                printf("background pid %d is done: ", termChild);
                fflush(stdout);
                printStatus(status);
            }
        }
    }
}

/* Execute built-in cmds "status" and "cd"
*  If other cmd, pass bgProcs array and "status" ptr
*  to execOther() and update status to return
*/
int *execCmd(struct Command *cmd, int *status, pid_t *bgProcs) {
    // Built-in "status"
    if (strcmp(cmd->cmd, "status") == 0) {
        printStatus(status);
    // Built-in "cd"
    } else if (strcmp(cmd->cmd, "cd") == 0) {
        changeDir(cmd->args[0]);
    // Other commands
    } else {
        status = execOther(cmd, status, bgProcs);
    }

    return status;
}

/* Execute non-built-in cmds as either foreground or background
*  waitpid() only runs on fg procs set in Command struct "bg" attribute
*  Update "status" ptr and return ptr
*/
int *execOther(struct Command *cmd, int *status, pid_t *bgProcs) {
    // Build argv vector
    char *newargv[(cmd->nArgs)+2];
    newargv[0] = cmd->cmd;
    newargv[(cmd->nArgs)+1] = NULL;
    for (int i = 0; i < (cmd->nArgs); i++) {
        newargv[i+1] = cmd->args[i];
    }

    // Block SIGTSTP until fg ends
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    if (!cmd->bg) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
    }

    // Code modified from Module: Exec a New Program
 	int childStatus;
	pid_t childPid = fork();
    char *nullFile = "/dev/null";

    if (childPid == -1) {
        perror("fork() failed!");
        fflush(stderr);
        exit(1);
    // childPid is 0 in child
    } else if (childPid == 0) {
        struct sigaction SIG_child = { {0} };
        sigfillset(&SIG_child.sa_mask);
        SIG_child.sa_flags = 0;

        // Set fg to terminate on SIGINT
        if (!cmd->bg) {
            SIG_child.sa_handler = SIG_DFL;
            sigaction(SIGINT, &SIG_child, NULL);
        // Set bg to ignore SIGTSTP
        } else {
            SIG_child.sa_handler = SIG_IGN;
            sigaction(SIGTSTP, &SIG_child, NULL);
        }

        // Handle input redirection
        if (cmd->inputFile != NULL) {
            redirectIO(cmd->inputFile, 0);
        } else if (cmd->inputFile == NULL && cmd->bg == 1) {
            // Default input for bg proc
            redirectIO(nullFile, 0);
        }

        // Handle output redirection
        if (cmd->outputFile != NULL) {
            redirectIO(cmd->outputFile, 1);
        } else if (cmd->outputFile == NULL && cmd->bg == 1) {
            // Default output for bg proc
            redirectIO(nullFile, 1);
        }
        
        execvp(newargv[0], newargv);

        // execv() returns only on error
        printf("%s: no such file or directory\n", newargv[0]);
        exit(1);
    // childPid is child's pid in parent
    } else {
        // After running as foreground process
        if (!cmd->bg) {
            waitpid(childPid, &childStatus, 0);

            // Set child's termination status
            if( WIFEXITED(childStatus) ){
                *status = WEXITSTATUS(childStatus);
            } else {
                *status = WTERMSIG(childStatus);
                printStatus(status);
            }

            // Unblock any SIGTSTP blocked by fg process
            sigprocmask(SIG_UNBLOCK, &mask, NULL);

        // After running as background process
        } else {
            printf("background pid is %d\n", childPid);
            fflush(stdout);
            // Add childPid to list of bg processes in empty index
            for (int i = 0; i < MAX_BGPROCS; i++) {
                if (bgProcs[i] == EMPTY_BGPROC) {
                    bgProcs[i] = childPid;
                    break;
                }
            }
        }
    }
    return status;
}

/* Search "bgProcs" array for any still-running bg procs to terminate
*  Only runs before smallsh exits
*/
void exitBackground(pid_t *bgProcs) {
    // Find any remaining bg processes
    for (int i = 0; i < MAX_BGPROCS; i++) {
        if (bgProcs[i] != EMPTY_BGPROC) {
            // Terminate still-running bg proc
            if( kill(bgProcs[i], 0) == 0 ) {
                kill(bgProcs[i], SIGTERM);
            }
        }
    }
}

/* 
*  Print exit value held by int ptr "status"
*/
void printStatus(int *status) {
    if (*status == 0 || *status == 1) {
        printf("exit value %d\n", *status);
        fflush(stdout);
    } else {
        printf("terminated by signal %d\n", *status);
        fflush(stdout); 
    }
}

/* 
*  Redirect I/O from stdin/stdout to a valid file
*/
void redirectIO(char *file, int std) {
    char *message = NULL;
    int openFD;
    // fd for stdin=0, stdout=1
    if (std) {
        message = "output";
        openFD = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    } else {
        message = "input";
        openFD = open(file, O_RDONLY);
    }

    // Check if file opened correctly
    if (openFD == -1) { 
        printf("cannot open %s for %s\n", file, message);
        fflush(stdout);
        exit(1); 
    } else {
        // Redirect I/O
        dup2(openFD, std);
    }
}
//