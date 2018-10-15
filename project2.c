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
        char *input = trsh_LINEINPUT();
        char **tokenized = trsh_INPUTPARSE(input, &numArgs);
        trsh_HANDLER(tokenized);
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
    if(strcmp(tokenizedData[0], "chdir") == 0)
    {
        return trsh_chdir(tokenizedData[1]);
    }
    else
    {
        if(0)
        {
            //If internal command, run trsh_INTERNAL
            trsh_INTERNAL(tokenizedData);
        }
        else
        {
            return trsh_EXTERNAL(tokenizedData);
        }
    }
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
        //TODO: Open IO redirection, find program, run program, and exit.
    }
    else //Parent process
    {
        waitpid(pid, NULL, WUNTRACED); //Wait until child is finished.
    }


}

int trsh_ditto(char** args)
{
    printf("Entered a internal command.\n");
    return 0;
}

int trsh_chdir(char* directory)
{
    char pathName[PATH_MAX];
    realpath(directory, pathName); // Finds the full path.
    char envDir[PATH_MAX];

    if (-1 == chdir(pathName)) { //Change the directory using chdir
        fprintf(stderr, "trsh_chdir: directory change not successful. (chdir)\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }
    snprintf(envDir, PATH_MAX, "PWD=%s", directory); //Creates the env command.
    if (putenv(envDir) == -1) { //Change the directory using putenv.
        fprintf(stderr, "trsh_chdir: directory change not successful.(putenv)\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}