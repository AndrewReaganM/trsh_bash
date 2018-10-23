#include "trsh.h"

char commands[10][10] =
        {
                "ditto",
                "erase",
                "morph",
                "mimic",
                "mkdirz",
                "rmdirz",
                "wipe",
                "filez",
                "help"

        };
int numCommands = 9;

/**
 * Loops through the program functions.
 *
 * @param argc the number of arguments passed into the program.
 * @param argv the arguments that were passed in.
 * @return EXIT_SUCCESS on success or EXIT_FAILURE/no return value on failure.
 */
int main(int argc, char **argv) {
    const char READ = 'r'; //Represents READ for freopen.
    if (argc > 1) {
        if((freopen(argv[1], &READ, stdin)) == NULL)
        {
            return EXIT_FAILURE;
        }
    }

    setbuf(stdout, NULL); //Sets the buffer to NULL.
    setbuf(stderr, NULL); //Sets the stderr buffer to NULL.

    while (1) {
        char *input = trsh_LINEINPUT(&argc); //Take in input from stdin.-
        char **tokenized = trsh_INPUTPARSE(input, &numArgs); //Parse string of input.
        if (trsh_HANDLER(tokenized) == ESC_PROGRAM) { // Route commands internally or externally.
            return EXIT_SUCCESS; // Exit gracefully if ESC is called.
        }
    }

}

/**
 * Takes in input from stdin and saves it to a c-string.
 *
 * @return c-string containing stdin until \n or EOF.
 */
char *trsh_LINEINPUT(int *argc) {
    int bufferSize = LINE_BUFFER_SIZE; //Size of line buffer
    int resize = RESIZE_CONST; //Multiplier for resizing line buffer.
    int length = 0; //Length of the current input.
    char *lineBuffer; //c-string containing the input.
    int c; //Character to handle the input from getchar.

    char *cwdBuf; //Create a buffer for the current working directory.
    cwdBuf = calloc(PATH_MAX, sizeof(char));
    cwdBuf = getcwd(cwdBuf, PATH_MAX);
    if (*argc < 2) {
        printf("%s==>", cwdBuf); //Print CWD and prompt.
    }
    lineBuffer = calloc(bufferSize, sizeof(char)); //Allocate space for the input.
    if (!lineBuffer) { //calloc error checking.
        fprintf(stderr, "trsh_LINEINPUT: memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar(); //Take in char from STDIN.

        if (c == EOF || c == '\n') {
            lineBuffer[length] = '\0';
            if (c == EOF) { //If EOF, exit the program entirely.
                exit(EXIT_SUCCESS);
            }
            break;
        } else {
            lineBuffer[length] = c; //Save char into the c string.
            length++;
        }
        //Reallocate space if necessary
        if (length >= bufferSize) {
            bufferSize += bufferSize / 2;
            lineBuffer = realloc(lineBuffer, bufferSize);
            //Error checking
            if (!lineBuffer) {
                fprintf(stderr, "trsh_LINEINPUT: memory reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if(*argc > 1)
    {
        printf("%s==>%s\n", cwdBuf, lineBuffer); //Prints CWD and command for macro file output.
    }
    free(cwdBuf);
    return lineBuffer;
}

/**
 * Parses input taken in from the command line.
 *
 * @param input char pointer to a space separated string.
 * @param numberOfTokens pointer to an int that stores the number of tokens.
 * @return a pointer to an array of pointers containin the tokenized data.
 */
char **trsh_INPUTPARSE(char *input, int *numberOfTokens) {
    int bufferSize = TOKEN_BUFFER_SIZE;
    int numTok = 0;
    char **tokenizedData = malloc(bufferSize * sizeof(char *));
    char *tokenPointer;

    if (!tokenizedData) {
        fprintf(stderr, "trsh_INPUTPARSE: memory allocation error - pointer array\n");
        exit(EXIT_FAILURE);
    }

    tokenPointer = strtok(input, TOKEN_DELIMITERS);

    while (tokenPointer != NULL) {
        tokenizedData[numTok] = tokenPointer;
        if (numTok >= bufferSize) {
            bufferSize += bufferSize;
            tokenizedData = realloc(tokenizedData, bufferSize * sizeof(char *));
            if (!tokenizedData) {
                fprintf(stderr, "trsh_INPUTPARSE: error reallocating memory - pointer array\n");
                exit(EXIT_FAILURE);
            }
        }
        numTok++;
        tokenPointer = strtok(0, TOKEN_DELIMITERS);
    }
    *numberOfTokens = numTok;
    return tokenizedData;
}

/**
 * Handles command routing for internal and external commands.
 * @param tokenizedData pointer to array of pointers containing instructions.
 * @return success value of function called.
 */
int trsh_HANDLER(char **tokenizedData) {
    // Attempt to run the commands that need to run in the parent process.
    if (strcmp(tokenizedData[0], "chdir") == 0) {
        return trsh_chdir(tokenizedData[1]); //Change the directory.
    } else if (strcmp(tokenizedData[0], "esc") == 0) {
        return ESC_PROGRAM; //ESC the program.
    } else if (strcmp(tokenizedData[0], "environ") == 0) {
        return trsh_environ(); //Print the environ variables.
    } else {
        for (int i = 0; i < numCommands; i++) {
            if (strcmp(tokenizedData[0], commands[i]) == 0) //If command is internal.
            {
                return trsh_INTERNAL(tokenizedData); //Run internal commands.
            }
        }
        return trsh_EXTERNAL(tokenizedData); //Run external commands.
    }
}

/**
 * Handles external commands.
 * @param tokenizedData pointer to array of pointers containing instructions.
 * @return return EXIT_SUCCESS on successful parent wait.
 */
int trsh_EXTERNAL(char **tokenizedData) {
    int pid = fork();
    if (pid == -1) {
        fprintf(stderr, "trsh_EXTERNAL: fork unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        //this is the child, do redirection and run the process.

        trsh_REDIRECTION(tokenizedData);
        //char* stdinData = trsh_LINEINPUT();
        //char** stdinTokens = trsh_INPUTPARSE(stdinData, &numArgs);
        _exit(execvp(tokenizedData[0], tokenizedData));
    } else // Parent process.
    {
        waitpid(pid, NULL, WUNTRACED); //Wait until child is finished.
        return EXIT_SUCCESS;
    }

}

/**
 * Handles internal commands.
 * @param tokenizedData pointer to array of pointers containing instructions.
 * @return return EXIT_SUCCESS after parent waits.
 */
int trsh_INTERNAL(char **tokenizedData) {
    int pid = fork();
    if (pid == -1) {
        fprintf(stderr, "trsh_INTERNAL: fork unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) //Child process
    {
        if (strcmp(tokenizedData[0], "ditto") == 0) {
            exit(trsh_ditto(tokenizedData));
        } else if (strcmp(tokenizedData[0], "wipe") == 0) {
            exit(execvp("clear", tokenizedData));
        } else if (strcmp(tokenizedData[0], "erase") == 0) {
            exit(trsh_erase(tokenizedData));
        } else if (strcmp(tokenizedData[0], "filez") == 0) {
            exit(trsh_filez(tokenizedData));
        } else if (strcmp(tokenizedData[0], "rmdirz") == 0) {
            exit(trsh_rmdirz(tokenizedData));
        } else if (strcmp(tokenizedData[0], "wipe") == 0) {
            exit(trsh_wipe());
        } else if (strcmp(tokenizedData[0], "mkdirz") == 0) {
            exit(trsh_mkdirz(tokenizedData));
        } else if ((strcmp(tokenizedData[0], "morph") == 0) || (strcmp(tokenizedData[0], "mimic") == 0)) {
            exit(trsh_mimic_morph(tokenizedData));
        } else if (strcmp(tokenizedData[0], "help") == 0) {
            exit(trsh_help(tokenizedData));
        }
    } else //Parent process
    {
        waitpid(pid, NULL, WUNTRACED); // Wait until child is finished.
        return EXIT_SUCCESS;
    }
}

/**
 * Sets up redirection in a given process.
 *
 * @param tokenizedData pointer to array of pointers containing tokenized input.
 * @return EXIT_SUCCESS.
 */
int trsh_REDIRECTION(char **tokenizedData) {
    for (int i = 0; i < numArgs - 1; i++) //Check all but the last entry for i/o redirection symbols.
    {
        // Set up redirection in a given process.
        if (strcmp(tokenizedData[i], "<") == 0) {
            freopen(tokenizedData[i + 1], "r", stdin); //Opens file to read to stdin.
            //printf("Input redirection set up.\n");
            numArgs -= 2;
            *tokenizedData[i] = NULL;
            *tokenizedData[i + 1] = NULL;
        }
        if (strcmp(tokenizedData[i], ">") == 0) {
            freopen(tokenizedData[i + 1], "w", stdout); //Opens stdout to write to file.
            //printf("Output redirection set up.\n");
            numArgs -= 2;
            *tokenizedData[i] = NULL;
            *tokenizedData[i + 1] = NULL;
        }
        if (strcmp(tokenizedData[i], ">>") == 0) {
            freopen(tokenizedData[i + 1], "a", stdout); //Opens stdout to append to a file.
            //printf("Output appending redirection set up.\n");
            numArgs -= 2;
            *tokenizedData[i] = NULL;
            *tokenizedData[i + 1] = NULL;
        }
    }
    return EXIT_SUCCESS;
}

