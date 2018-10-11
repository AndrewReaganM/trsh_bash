#ifndef INC_2_TRSH_H
#define INC_2_TRSH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

#define MAX_BUFFER = 1024

int mimic(char *command, char *srcName, char *destName); //Function to copy files.

int erase(char *command, char *arguments, int tok); //Function to erase files

int morph(char *command, char *arguments); //Function to move files and directories.

int changeDir(char *command, char *arguments); //Function to change the working directories.


#endif //INC_2_TRSH_H
