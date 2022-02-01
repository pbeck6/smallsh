#ifndef SHELL_H_
#define SHELL_H_

/* Processes functionality */
#include "command.h"
#include <sys/wait.h>

/* Function declarations */
void changeDir(char *);
int execCmd(struct Command *, int);
int execOther(struct Command *, int);
void printStatus(int);
void redirectInput(char *);
void redirectOutput(char *);

#endif
//