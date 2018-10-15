#include <assert.h>
#include "trsh.h"

extern char **environ;

/**
 *
 *
 * @param argc the number of arguments passed into the program.
 * @param argv the arguments that were passed in.
 * @return EXIT_SUCCESS on success or EXIT_FAILURE/no return value on failure.
 */
int main(int argc, char **argv) {
    int numArgs;

    while(1) {
        char *input = lineInput();
        char **tokenized = inputParse(input, &numArgs);
        trshHandler(tokenized);
    }

}

char *lineInput() {
    int bufferSize = LINE_BUFFER_SIZE;
    int resize = RESIZE_CONST;
    int length = 0;
    char *lineBuffer = calloc(bufferSize, sizeof(char));
    int c; //TODO: Check this to see if the char type will work, maybe do int.

    if (!lineBuffer) {
        fprintf(stderr, "lineInput: memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *cwdBuf;
    cwdBuf = calloc(PATH_MAX,sizeof(char)); //TODO: Is PATH_MAX acceptable?
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
                fprintf(stderr, "lineInput: memory reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **inputParse(char *input, int *numberOfTokens) {
    int bufferSize = TOKEN_BUFFER_SIZE;
    int numTok = 0;
    char **tokenizedData = malloc(bufferSize * sizeof(char *));
    char *tokenPointer;

    if (!tokenizedData) {
        fprintf(stderr, "inputParse: memory allocation error - pointer array\n");
        exit(EXIT_FAILURE);
    }

    tokenPointer = strtok(input, TOKEN_DELIMITERS);

    while (tokenPointer != NULL) {
        tokenizedData[numTok] = tokenPointer;
        if (numTok >= bufferSize) {
            bufferSize += bufferSize;
            tokenizedData = realloc(tokenizedData, bufferSize * sizeof(char *));
            if (!tokenizedData) {
                fprintf(stderr, "inputParse: error reallocating memory - pointer array\n");
                exit(EXIT_FAILURE);
            }
        }
        numTok++;
        tokenPointer = strtok(0, TOKEN_DELIMITERS);
    }
    *numberOfTokens = numTok;
    return tokenizedData;
}

int trshHandler(char** tokenizedData)
{
    return trsh_EXTERNAL(tokenizedData);
}

int trsh_EXTERNAL(char **tokenizedData)
{
    int pid = fork();
    if(pid == -1)
    {
        fprintf(stderr, "trsh_EXTERNAL: fork unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0)
    {
        //this is the child, run the process.

        exit(execvp(tokenizedData[0], tokenizedData));
    }
    else
    {
        // Parent processs
        waitpid(pid, NULL, WUNTRACED);
    }

}

int trsh_INTERNAL(char **tokenizedData)
{
    //Check for redirection
    //If redirection, create a pipe??? Or will stdin/stdout work?
    
}

int trsh_ditto(char** args)
{

}