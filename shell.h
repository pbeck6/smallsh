#ifndef SHELL_H_
#define SHELL_H_

/* Processes functionality */
#include "command.h"
#include <sys/wait.h>
#include <signal.h>

/* Macros */
#define MAX_BGPROCS 200
#define EMPTY_BGPROC -999

/* Function declarations */
void changeDir(char *);
void checkBgChild(int *, pid_t *);
int *execCmd(struct Command *, int *, pid_t *);
int *execOther(struct Command *, int *, pid_t *);
void exitBackground(pid_t *);
void foregroundOnly(int);
void printStatus(int *);
void redirectIO(char *, int);

#endif
//