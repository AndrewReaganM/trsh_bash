#include "trsh.h"

int trsh_ditto(char** args) {
    for(int i=1; i < numArgs; i++)
    {
        printf("%s", args[i]); // Print the argument
        if(((numArgs-1) != i)) { //Print a space only between each word.
            printf(" ");
        }
    }
    printf("\n"); // Print new line at the end of the statement.
}
int trsh_chdir(char* directory) {
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
int trsh_environ(void) {
    char **env = environ;
    while (*env) printf("%s\n", *env++);  // step through environment and print.
}
int trsh_erase(char** args)
{
    //TODO: Add check to see if file.
    if(remove(args[1]) !=0)
    {
        fprintf(stderr, "trsh_erase: file not successfully erased.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int trsh_filez(char** args)
{
    char **tempArgs = calloc(3, sizeof(char*));
    tempArgs[0] = "ls";
    tempArgs[1] = "-1";
    if(args[1] != NULL)
    {
        tempArgs[2] = args[1];
    }
    if(execvp(tempArgs[0], tempArgs) == -1)
    {
        fprintf(stderr, "trsh_wipe: Screen wipe failed.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int trsh_rmdirz(char** args)
{
    //TODO: Add check to see if directory.
    if(remove(args[1]) !=0)
    {
        fprintf(stderr, "trsh_rmdirz: directory not erased. Either non-empty directory, a file, or directory does not exist.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int trsh_wipe(void)
{
    char **tempArgs = calloc(2, sizeof(char*));
    tempArgs[0] = "clear";

    if(execvp(tempArgs[0], tempArgs) == -1)
    {
        fprintf(stderr, "trsh_wipe: Screen wipe failed.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int trsh_mkdirz(char** args)
{
    //TODO: Add error checking and return values.
    if(args[1] == NULL)
    {
        fprintf(stderr, "trsh_mkdirz: Directory not provided.\n");
        exit(EXIT_FAILURE);
    }
    if(mkdir(args[1], 0777) != 0)
    {
        fprintf(stderr, "trsh_mkdirz: Directory not created.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
