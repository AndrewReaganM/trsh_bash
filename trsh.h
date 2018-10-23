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
#include <errno.h>
#include <dirent.h>
#include <fts.h>
#include <sys/wait.h>

#define LINE_BUFFER_SIZE 1024 // The size of the buffer that is to be created.
#define TOKEN_BUFFER_SIZE 128 // The buffer size of the tokens.
#define TOKEN_DELIMITERS " " // Characters that the parser uses to delimit text.
#define MIMIC 2 //Flag used for mimic
#define MORPH 4 //Flag used for morph
#define MAX_FILENAME 256
#define HAS_FILES 3

int numArgs;
extern char **environ;

char** trsh_INPUTPARSE(char *input, int *numArgs); //Parses the input into tokens.

int trsh_REDIRECTION(char **tokenizedData);

int trsh_ROUTING(char **tokenizedData); //Runs external exec commands.

int trsh_chdir(char* directory); //Changes the working directory of the current shell.

int trsh_ditto(char** args); // Command similar to echo in bash.

int trsh_environ(void); // Prints the environ variable of the current process.

int trsh_erase(char** args); //Deletes a specific file.

int trsh_filez(char** args); //Lists all files in the current directory or the directory provided.

int trsh_rmdirz(char** args); // Removes empty directories.

int trsh_mkdirz(char** args); // Creates a new directory if the path's parent exists, but the directory does not.

int trsh_wipe(void); //Wipes the terminal display.

int recursiveFileOperation(int mode, int recursive_flag, char* source, char* destination);

int trsh_mimic_morph(char** args);

int isDir(const char *path);

int trsh_fts_cmp(const FTSENT** one, const FTSENT** two);

int fileCpy(char* source, char* destination);

int recursivePathBuilder(int level, char* path, FTSENT* location);

int trsh_help(char** args); // Prints the contents of the readme.


// *************************** Macros/Structs **************************

struct filemanip_st {
    char src[MAX_FILENAME]; // This is the source file or the only file in case of an erase;
    char dst[MAX_FILENAME];
    unsigned int op; // 0 = erase; 1 = mimic; 2 = morph;
    int recursion; // recursion flag
};
typedef struct filemanip_st filemanip;



#endif //INC_2_TRSH_H
