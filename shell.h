#ifndef SHELL_H_
#define SHELL_H_

// Use if --std=c99
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Directory & file functionality */
#include <dirent.h>
#include <fcntl.h>

/* File meta-data functionality */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "command.h"

/* Function declarations */
int execCmd(struct Command *, int);

#endif
//