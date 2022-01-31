#include "shell.h"

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

int execCmd(struct Command *cmd, int status) {
    if (cmd->cmd != NULL) {
        // Built-in "exit"
        if (strcmp(cmd->cmd, "exit") == 0) {
            //////////////////////WAIT FOR BG PROCS
            exit(0);
        // Built-in "status"
        } else if (strcmp(cmd->cmd, "status") == 0) {
            printStatus(status);
        // Built-in "cd"
        } else if (strcmp(cmd->cmd, "cd") == 0) {
            changeDir(cmd->args[0]);
        // Other commands
        } else {
            status = execOther(cmd, status);
        }
    }
    return status;
}

int execOther(struct Command *cmd, int status) {
    // Code modified from Module: Exec a New Program
 	int   childStatus;
	pid_t childPid = fork();

    // Build argv vector
    char *newargv[(cmd->nArgs)+2];
    newargv[0] = cmd->cmd;
    newargv[(cmd->nArgs)+1] = NULL;
    for (int i = 0; i < (cmd->nArgs); i++) {
        newargv[i+1] = cmd->args[i];
    }

    switch (childPid){
    case -1:
        perror("fork() failed!");
        exit(1);
        break;
    // spawnpid is 0 in child
    case 0:
        execvp(newargv[0], newargv);

        // execv() returns only on error
        perror("execv");
        exit(2);
        break;
    // spawnpid is child's pid in parent
    default:
        childPid = waitpid(childPid, &childStatus, 0);
        printf("Parent's waiting done as child pid %d exited\n", childPid);
        fflush(stdout);
        // Set child's termination status
        if( WIFEXITED(childStatus) ){
            status = WEXITSTATUS(childStatus);
		} else {
            status = WTERMSIG(childStatus);
		}
        break;
    }
    return status;
}

void printStatus(int status) {
    if (status == 0 || status == 1) {
        printf("exit value %d\n", status);
        fflush(stdout);
    } else {
        printf("terminated by signal %d\n", status);
        fflush(stdout); 
    }
}