#include "trsh.h"

extern char **environ;

//TODO: Remember FILENAME_MAX

/**
 * This program is called "trsh".
 * The purpose of this is to take in commands and either execute a custom function,
 * or attempt to execute the function through bash.
 *
 * @param argc the number of arguments passed into the program.
 * @param argv the arguments that were passed in.
 * @return EXIT_SUCCESS on success or EXIT_FAILURE/no return value on failure.
 */
int main(int argc, char **argv) {
    char shellCommand[1000]; //Buffer to hold the given input. Must be less than 1000 characters.
    char *command; //Variable to hold the command/function name.
    char shellCopy[1000]; //Variable to hold a copy of the shell.
    char inputChar; //Char variable to hold the character being read in.
    int length = 0; //Stores the length of the current shell entry.
    const char READ = 'r'; //Represents READ for freopen.

    memset(shellCommand, 0, 1000); //Allocates memory for memset.

    setbuf(stdout, NULL); //Sets the buffer to NULL.

    //printf("Welcome to trsh! Enter text now.\n");

    if (argc > 1) {
        freopen(argv[1], &READ, stdin); //Sends macro file to stdin if applicable.
    }

    while (1) {
        if(argc < 2) // Handles the prompt for shell entry.
        {
            printf("==>");
            fflush(stdout);
        }



        memset(shellCommand, 0, length); //Clears the shellCommand variable for a new start.
        length = 0;

        while ((inputChar = getchar()) != '\n' && length < 1000) {
            if (inputChar == EOF) //If the macro file ends, print a following prompt.
            {
                printf("==>");
                return EXIT_SUCCESS;
            }

            shellCommand[length] = inputChar;
            ++length;
        }

        if (argc > 1) //Handles the prompt view for macro file entry.
        {
            printf("==>%s\n", shellCommand);
        }

        strcpy(shellCopy, shellCommand);

        command = strtok(shellCommand, " "); //Copying the first part of the input to the command var.

        if(length > 0){
            if (strcmp(command, "esc") == 0) {  //ESC function
                return EXIT_SUCCESS;
            } else if (strcmp(command, "ditto") == 0) { //Ditto command for repeating text.
                char *temp;
                temp = strtok(0, " "); //Pull the first block
                printf("%s", temp); //Print the first block

                while ((temp = strtok(0, " "))) { //Go through the rest of the input.
                    printf(" %s", temp);
                }
                printf("\n");

            } else if (strcmp(command, "help") == 0) { //help command that prints the readme.
                system("cat ./README.txt"); //Print out the readme
                // TODO: Maybe make this an absolute path?
            } else if (strcmp(command, "erase") == 0) { //erase command that deletes a given file or empty directory.
                erase(command, shellCommand, 1);
            } else if (strcmp(command, "wipe") == 0) { //Clears the console window.
                system("clear");
            } else if (strcmp(command, "filez") == 0) { // implements an ls command.
                char commandBuilder[400] = "ls -1 ";
                strcat(commandBuilder, (shellCommand + strlen(command) + 1)); // Build the command to pass back to bash.
                system(commandBuilder); //Run the ls -1 command in bash.
            } else if (strcmp(command, "environ") == 0) { // Displays all environment variables.
                char **env = environ;
                while (*env) printf("%s\n", *env++);  // step through environment
            } else if (strcmp(command, "mimic") == 0) { // Performs file copying.
                char *source = strtok(0, " ");
                char *dest = strtok(0, " ");
                mimic(command, source, dest);
            } else if (strcmp(command, "erase") == 0) { //Erases files
                erase(command, shellCommand, 1);
            } else if (strcmp(command, "morph") == 0) { //Moves directories or files.
                morph(command, shellCommand);
            } else if (strcmp(command, "chdir") == 0) { // Changes the working directory.
                changeDir(command, shellCommand);
            } else { //All other commands passed to system.
                system(shellCopy);
            }
        }


    }
}

int mimic(char *command, char *srcName, char *destName) {

    int cpy;

    FILE *source, *target;

    source = fopen(srcName, "r"); //Attempt to open source.

    if (source == NULL) { //If unsuccessful, failure.
        fprintf(stderr, "mimic: Invalid Source.\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    target = fopen(destName, "w"); //Attempt to open destination.

    if (target == NULL) { //If unsuccessful, failure.
        fclose(source);
        fprintf(stderr, "mimic: Invalid Destination. %s\n", destName);
        fflush(stderr);
        return EXIT_FAILURE;
    }

    while ((cpy = fgetc(source)) != EOF) {
        fputc(cpy, target); //Copy until the end of the file is reached.
    }

    fclose(source); //Close both files
    fclose(target);

    return EXIT_SUCCESS;
}

int erase(char *command, char *arguments, int tok) {
    char *parsed;

    if (tok == 1) //If the input is tokenized
    {
        parsed = strtok(0, " "); //Then parse it.
    } else {
        strcpy(parsed, command); //Else simply copy the path.
    }

    if (remove(parsed) < 0) {
        fprintf(stderr, "erase: file/directory removal not successful.\n");
        fflush(stderr);
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }

}

int morph(char *command, char *arguments) {
    char *srcName = strtok(0, " "); //Tokenize both of the args.
    char *destName = strtok(0, " ");
    char srcNameCopy[75];
    memset(srcNameCopy, 0, 75);
    struct stat s; //Stores the status of the source file.
    struct stat d; //Stores the status of the dest file.


    if(stat(srcName, &s) < 0) //If the source is not a file/does not exist, exit.
    {
        fprintf(stderr, "morph: source does not exist.\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    stat(destName, &d);

    if(!S_ISREG(d.st_mode)) //If the destination is not a file.
    {
        strcpy(srcNameCopy, srcName);
        char *fileName = basename(srcNameCopy);
        strcat(destName, fileName);
        if(mimic(NULL, srcName, destName) != EXIT_FAILURE)//Copy the file over
        {
            remove(srcName); //Remove the source file
            return EXIT_SUCCESS;
        }
    }
    else // dest is a file
    {
        if(mimic(NULL, srcName, destName) != EXIT_FAILURE)//Copy the file over, dont remove if unsuccessful.
        {
            remove(srcName); //Remove the source file
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;

}

int changeDir(char *command, char *arguments) {
    char *tempDir = strtok(0, " "); //Grab the directory
    char envDir[150];

    if (-1 == chdir(tempDir)) { //Change the directory using chdir
        fprintf(stderr, "chdir: directory change not successful. (chdir)\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }
    snprintf(envDir, 150, "PWD=%s", tempDir);
    if (putenv(envDir) == -1) { //Change the directory using putenv.
        fprintf(stderr, "chdir: directory change not successful.(putenv)\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    //printf("Directory changed.\n");
    return EXIT_SUCCESS;
}
