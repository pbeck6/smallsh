#ifndef COMMAND_H_
#define COMMAND_H_

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

/* Macros */
#define MAXLEN_CMD 2048
#define MAXARGS_CMD 512

/* struct for command */
struct Command {
    char *cmd;
    char *args[MAXARGS_CMD]; 
    char *inputFile;
    char *outputFile;
    int bg;
};

/* Function declarations */
void destroyCmd(struct Command *);
struct Command *initCmd(void);
struct Command *parseCmd(char *);
void printCmd(struct Command *);

#endif
//