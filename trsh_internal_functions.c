#include "trsh.h"
/**
 * Ditto - repeats the input back to stdout.
 *
 * @param args the input in tokenized string form.
 * @return success value.
 */
int trsh_ditto(char** args) {
    for(int i=1; i < numArgs; i++)
    {
        printf("%s", args[i]); // Print the argument
        if(((numArgs-1) != i)) { //Print a space only between each word.
            printf(" ");
        }
    }
    printf("\n"); // Print new line at the end of the statement.
    return EXIT_SUCCESS;
}
/**
 * chdir - function that changes the working directory.
 *
 * @param directory the text that contains an absolute or relative path.
 * @return success value.
 */
int trsh_chdir(char* directory) {
    char pathName[PATH_MAX];
    if(realpath(directory, pathName) == NULL) // Finds the full path.
    {
        fprintf(stderr, "trsh_chdir: %s not resolved with realpath.\n", directory);
        return EXIT_FAILURE;
    }
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
/**
 * Print out environ variable.
 *
 * Code taken from Project 1 specification by Andrew Fagg and Christian Grant.
 *
 * @return success value.
 */
int trsh_environ(void) {
    char **env = environ;
    while (*env) printf("%s\n", *env++);  // step through environment and print.
    return EXIT_SUCCESS;
}
/**
 * Erase a file.
 *
 * @param args pointer to an array of pointers containing arguments.
 * @return success or failure.
 */
int trsh_erase(char** args)
{
    //TODO: Add check to see if file.
    if(remove(args[1]) !=0)
    {
        fprintf(stderr, "trsh_erase: file not successfully erased.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Prints out files in current directory or directory supplied.
 *
 * @param args pointer to array of pointers containing arguments.
 * @return success or failure.
 */
int trsh_filez(char** args)
{
    char *tempArgs[4]; //Create array of pointers, with room for terminating null.
    tempArgs[0] = "ls"; //Use ls -1
    tempArgs[1] = "-1";
    tempArgs[3] = NULL; //Terminating null
    if(args[1] != NULL)
    {
        tempArgs[2] = args[1]; //If supplied, use directory.
    } else{
        tempArgs[2] = NULL; //Else null terminate early.
    }
    if(execvp(tempArgs[0], tempArgs) == -1)
    {
        fprintf(stderr, "trsh_filez: filez failed.\n"); //Print failure if fail.
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Removes a specified directory.
 *
 * @param args rgs pointer to array of pointers containing arguments.
 * @return success or failure.
 */
int trsh_rmdirz(char** args)
{
    //TODO: Add check to see if directory.
    if(remove(args[1]) !=0)
    {
        fprintf(stderr, "trsh_rmdirz: %s: directory not erased. Either non-empty directory, a file, or directory does not exist.\n", args[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Wipe the current terminal session.
 *
 * @return success or failure.
 */
int trsh_wipe(void)
{
    char **tempArgs = calloc(2, sizeof(char*));
    tempArgs[0] = "clear";

    if(execvp(tempArgs[0], tempArgs) == -1)
    {
        fprintf(stderr, "trsh_wipe: Screen wipe failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
/**
 * Make a directory.
 *
 * @param args rgs pointer to array of pointers containing arguments.
 * @return success or failure
 */
int trsh_mkdirz(char** args)
{
    //TODO: Add error checking and return values.
    if(args[1] == NULL)
    {
        fprintf(stderr, "trsh_mkdirz: Directory not provided.\n");
        return EXIT_FAILURE;
    }
    if(mkdir(args[1], 0777) != 0)
    {
        fprintf(stderr, "trsh_mkdirz: %s: Directory not created.\n", args[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int trsh_help(char** args) // Prints the contents of the readme.
{
    FILE *file; //Help file pointer.
    int c; //Character for printing.

    file = fopen("/projects/2/README.txt", "r"); //Open file

    if(file == NULL)
    {
        fprintf(stderr, "trsh_help: file unable to be opened.\n"); //If not opened, error.
        return EXIT_FAILURE;
    }
    c = fgetc(file);
    while(c != EOF) //While there is a char, print.
    {
        printf("%c", c);
        c = fgetc(file);
    }

    fclose(file); //Close the file.
    return EXIT_SUCCESS;
}