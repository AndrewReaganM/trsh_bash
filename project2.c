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
    char *input = lineInput();
    char **tokenized = inputParse(input);
    printf(tokenized[0]);

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
    cwdBuf = malloc(PATH_MAX); //TODO: Is PATH_MAX acceptable?
    cwdBuf = getcwd(cwdBuf, PATH_MAX);
    printf("%s==>",cwdBuf);

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

char **inputParse(char *input) {
    int bufferSize = TOKEN_BUFFER_SIZE;
    int numberOfTokens = 0;
    char **tokenizedData = malloc(bufferSize * sizeof(char *));
    char *tokenPointer;

    if (!tokenizedData) {
        fprintf(stderr, "inputParse: memory allocation error - pointer array\n");
        exit(EXIT_FAILURE);
    }

    tokenPointer = strtok(input, TOKEN_DELIMITERS);

    while (tokenPointer != NULL) {
        tokenizedData[numberOfTokens] = tokenPointer;
        if (numberOfTokens >= bufferSize) {
            bufferSize += bufferSize;
            tokenizedData = realloc(tokenizedData, bufferSize * sizeof(char *));
            if (!tokenizedData) {
                fprintf(stderr, "inputParse: error reallocating memory - pointer array\n");
                exit(EXIT_FAILURE);
            }
        }
        numberOfTokens++;
        tokenPointer = strtok(0, TOKEN_DELIMITERS);
    }
    return tokenizedData;
}
