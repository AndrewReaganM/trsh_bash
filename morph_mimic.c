#include "trsh.h"

// Check if a file is a directory
int isDir(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int hasFiles(const char *path)
{
    int var = 0;
    struct dirent *drnt;
    DIR *directory = opendir(path);

}

int trsh_mimic(char** args)
{
    int recursionFlag = 0;
    int sourceIndex = -1;
    int destinationIndex = -1;

    char* sourceDirectory;
    char* destinationDirectory;

    // ******************* Check for flags and src/dst. ********************
    for(int i=1; i < numArgs; i++)
    {
        if(strcmp(args[i], "-r") == 0)
        {
            recursionFlag = 1;//Set the flag to 1
            printf("Recursion flag set to %i.\n", recursionFlag);
        }
        if(strcmp(args[i], "-r") != 0)
        {
            if(sourceIndex == -1)
            {
                sourceIndex = i;
                printf("Set source to %s.\n", args[sourceIndex]);
            }
            else if(destinationIndex == -1)
            {
                destinationIndex = i;
                printf("Set destination to %s.\n", args[destinationIndex]);
            }
        }
    }
    // ************** Check that both the source and the destination are set. **************
    if(sourceIndex == -1)
    {
        fprintf(stderr, "mimic/morph: Source directory not set.\n");
        return EXIT_FAILURE;
    }
    if(destinationIndex == -1)
    {
        fprintf(stderr, "mimic/morph: Destination directory not set.\n");
        return EXIT_FAILURE;
    }

    // *************** Create complete paths for the directories. *****************
    sourceDirectory = malloc(PATH_MAX);
    realpath(args[sourceIndex], sourceDirectory); // Get full path of source.

    destinationDirectory = malloc(PATH_MAX);
    realpath(args[destinationIndex], destinationDirectory); // Get full path of destination.

    // *********** Check the status of source and destination for debugging *************
    //TODO: Remove this code at some point.
    if(isDir(sourceDirectory))
    {
        printf("Source is a directory\n");
    }
    if(isDir(destinationDirectory))
    {
        printf("Destination is a directory\n");
    }

    // ****************************** Cases ***********************************

    if(isDir(sourceDirectory) && isDir(destinationDirectory)) // Both are directories.
    {
        if()



    }
    else if(!(isDir(sourceDirectory)) && !(isDir(destinationDirectory))) // Neither are directories
    {


    }
    else if(isDir(sourceDirectory) && !(isDir(destinationDirectory))) //SRC is a dir, DST is not a dir.
    {


    }
    else if(!(isDir(sourceDirectory)) && isDir(destinationDirectory)) //SRC is not a dir, DST is a dir.
    {

    }
}