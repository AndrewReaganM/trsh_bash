#ifndef INC_2_TRSH_H
#define INC_2_TRSH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <limits.h>

#define LINE_BUFFER_SIZE 1024 // The size of the buffer that is to be created.
#define RESIZE_CONST 2 // BUFFER_SIZE/RESIZE_CONST = the bytes added to the buffer once BUFFER_SIZE exceeded.
#define TOKEN_BUFFER_SIZE 128 // The buffer size of the tokens.
#define TOKEN_DELIMITERS " " // Chararcters that the parser uses to delimit text.

struct CommandData
{
    int numTokens;

};

char* lineInput(void); //Handles taking in stdin, terminates with EOF or \n.

char** inputParse(char* input, int *numArgs);

int trshHandler(char** tokenizedData);

int trsh_EXTERNAL(char **tokenizedData);

int trsh_INTERNAL(char **tokenizedData);

//int trsh_exec(char** tokenizedData, int** filedes);

#endif //INC_2_TRSH_H
