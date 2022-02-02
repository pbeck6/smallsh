#ifndef SHELL_H_
#define SHELL_H_

/* Processes functionality */
#include "command.h"
#include <sys/wait.h>

/* Macros */
#define MAX_BGPROCS 100

/* Function declarations */
void changeDir(char *);
int execCmd(struct Command *, int, pid_t *);
int execOther(struct Command *, int, pid_t *);
void exitBackground(pid_t *);
void printStatus(int);
void redirectIO(char *, int);

#endif
//