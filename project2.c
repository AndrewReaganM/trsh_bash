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
int numCommands = 8;

/**
 * @param argc the number of arguments passed into the program.
 * @param argv the arguments that were passed in.
 * @return EXIT_SUCCESS on success or EXIT_FAILURE/no return value on failure.
 */
int main(int argc, char **argv) {


    while(1) {
        char *input = trsh_LINEINPUT();
        char **tokenized = trsh_INPUTPARSE(input, &numArgs);
        if(trsh_HANDLER(tokenized) == ESC_PROGRAM)
        {
            return EXIT_SUCCESS;
        }
    }

}

char *trsh_LINEINPUT() {
    int bufferSize = LINE_BUFFER_SIZE;
    int resize = RESIZE_CONST;
    int length = 0;
    char *lineBuffer = calloc(bufferSize, sizeof(char));
    int c;

    if (!lineBuffer) {
        fprintf(stderr, "trsh_LINEINPUT: memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *cwdBuf;
    cwdBuf = calloc(PATH_MAX,sizeof(char));
    cwdBuf = getcwd(cwdBuf, PATH_MAX);
    printf("%s==>",cwdBuf);
    fflush(stdout);


    while (1) {
        c = getchar(); //Take in char from STDIN.

        if (c == EOF || c == '\n') {
            lineBuffer[length] = '\0';

            return lineBuffer;
        } else {
            lineBuffer[length] = c;
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
}

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

int trsh_HANDLER(char **tokenizedData)
{
    // Attempt to run the commands that need to run in the parent process.
    if(strcmp(tokenizedData[0], "chdir") == 0)
    {
        return trsh_chdir(tokenizedData[1]);
    }
    else if(strcmp(tokenizedData[0], "esc") == 0)
    {
        return ESC_PROGRAM;
    }
    else if(strcmp(tokenizedData[0], "environ") == 0)
    {
        return trsh_environ();
    }
    else
    {
        for(int i = 0; i < numCommands; i++)
        {
            if(strcmp(tokenizedData[0], commands[i]) == 0)
            {
                return trsh_INTERNAL(tokenizedData);
            }
        }
        return trsh_EXTERNAL(tokenizedData);
    }
}

int trsh_EXTERNAL(char **tokenizedData) {
    int pid = fork();
    if(pid == -1)
    {
        fprintf(stderr, "trsh_EXTERNAL: fork unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0)
    {
        //this is the child, do redirection and run the process.
        //freopen(ahh, , stdin);

        exit(execvp(tokenizedData[0], tokenizedData));
    }
    else // Parent process.
    {
        waitpid(pid, NULL, WUNTRACED); //Wait until child is finished.
    }

}

int trsh_INTERNAL(char **tokenizedData)
{
    //Check for redirection
    //If redirection, create a pipe??? Or will stdin/stdout work?
    int pid = fork();
    if(pid == -1)
    {
        fprintf(stderr, "trsh_INTERNAL: fork unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) //Child process
    {
        trsh_REDIRECTION(tokenizedData); // Set up redirection in the current process.

        if(strcmp(tokenizedData[0], "ditto") == 0)
        {
            exit(trsh_ditto(tokenizedData));
        }
        else if(strcmp(tokenizedData[0], "wipe") == 0)
        {
            execvp("clear", tokenizedData);
        }
        else if(strcmp(tokenizedData[0], "erase") == 0)
        {
            exit(trsh_erase(tokenizedData));
        }
        else if(strcmp(tokenizedData[0], "filez") == 0)
        {
            exit(trsh_filez(tokenizedData));
        }
        else if(strcmp(tokenizedData[0], "rmdirz") == 0)
        {
            exit(trsh_rmdirz(tokenizedData));
        }
        else if(strcmp(tokenizedData[0], "wipe") == 0)
        {
            exit(trsh_wipe());
        }
        else if(strcmp(tokenizedData[0], "mkdirz") == 0)
        {
            exit(trsh_mkdirz(tokenizedData));
        }

    }
    else //Parent process
    {
        waitpid(pid, NULL, WUNTRACED); // Wait until child is finished.
    }


}

int trsh_REDIRECTION(char **tokenizedData)
{
    for(int i=0; i < numArgs; i++)
    {
        // Set up redirection in a given process.
        if(strcmp(tokenizedData[i], "<") == 0)
        {
            freopen(tokenizedData[i+1], "r", stdin);
        }
        if(strcmp(tokenizedData[i], ">") == 0)
        {
            freopen(tokenizedData[i+1], "w+", stdout);
        }
        if(strcmp(tokenizedData[i], "<") == 0)
        {
            freopen(tokenizedData[i+1], "w", stdout);
            fseek(tokenizedData[i+1], 0, SEEK_END);
        }

    }
    return 0;
}

