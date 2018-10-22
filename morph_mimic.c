#include "trsh.h"

// Check if a file is a directory
int isDir(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}
/**
 * Determines whether a directory is empty or not.
 *
 * @param path path of the directory to check.
 * @return EXIT_FAILURE if not a dir, HAS_FILES if dir contains a file or dir, or EXIT_SUCCESS if empty.
 */
int hasFiles(const char *path)
{
    int var = 0;
    struct dirent *drnt;
    DIR *directory = opendir(path);

    if(directory == NULL) //If not a DIR or DIR does not exist.
    {
        return EXIT_FAILURE;
    }

    while((drnt = readdir(directory)) != NULL)
    {
        if(++var > 2)
        {
            return HAS_FILES; //Contains a file or directory.
        }
    }
    return EXIT_SUCCESS;

}

int trsh_mimic_morph(char** args)
{
    int mode = 0;
    int recursionFlag = 0;
    int sourceIndex = -1;
    int destinationIndex = -1;

    char* sourceDirectory;
    char* destinationDirectory;
    if(strcmp(args[0], "mimic") == 0) //If mimic command
        mode = MIMIC;
    else if(strcmp(args[0], "morph") == 0) //If morph command.
        mode = MORPH;
    else {
        fprintf(stderr, "Neither mimic nor morph specified. Exiting...\n");
        return EXIT_FAILURE;
    }

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

    /*if(isDir(sourceDirectory) && isDir(destinationDirectory)) // Both are directories.
    {
        if(hasFiles(sourceDirectory) != HAS_FILES) // Source is empty
        {
            char* moveFileName = malloc(PATH_MAX);
            strcpy(moveFileName, destinationDirectory);
            strcat(moveFileName, "/");
            strcat(moveFileName, basename(sourceDirectory));
            printf("%s\n", moveFileName);

            if(mkdir(moveFileName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                fprintf(stderr, "Unable to create directory %s\n", moveFileName);
                free(moveFileName);
                return EXIT_FAILURE;
            }
            if(rmdir(sourceDirectory) == -1)
            {
                fprintf(stderr, "Unable to remove source directory %s\n", sourceDirectory);
                free(moveFileName);
                return EXIT_FAILURE;
            }
            free(moveFileName);
            return EXIT_SUCCESS;
        }

        if(recursionFlag != 1)
        {
            fprintf(stderr, "%s contains files and -r was not supplied, exiting...", sourceDirectory);
            return EXIT_FAILURE;
        }
        // Recursively move directories.



    }
    else if(!(isDir(sourceDirectory)) && !(isDir(destinationDirectory))) // Neither are directories
    {


    }
    else if(isDir(sourceDirectory) && !(isDir(destinationDirectory))) //SRC is a dir, DST is not a dir.
    {


    }
    else if(!(isDir(sourceDirectory)) && isDir(destinationDirectory)) //SRC is not a dir, DST is a dir.
    {

    }*/

    recursiveFileOperation(mode, recursionFlag, sourceDirectory, destinationDirectory);
}

int recursiveFileOperation(int mode, int recursive_flag, char* source, char* destination)
{
    FTS* srcFileStructure = NULL;
    FTSENT* child = NULL;
    FTSENT* parent = NULL;
    char* path[2] = {source, NULL};

    srcFileStructure = fts_open(path, FTS_PHYSICAL, &trsh_fts_cmp);

    if(srcFileStructure != NULL)
    {
        while((parent = fts_read(srcFileStructure)) != NULL)
        {
            child = fts_children(srcFileStructure, 0);
            if(parent->fts_info == FTS_D)
            {
                //TODO: Will only create directories on one level. Find way to properly create destination path.
                char* tempDirName = malloc(PATH_MAX);
                strcpy(tempDirName, destination);
                recursivePathBuilder(parent->fts_level, tempDirName, parent);
                printf("New directory will be: %s\n", tempDirName);
                mkdir(tempDirName, 0777);


            }

            if(errno != 0)
            {
                fprintf(stderr, "Error calling fts_children\n");
                return EXIT_FAILURE;
            }
            while((child != NULL))
            {
                if(child->fts_info == FTS_F)
                {
                    char* srcCpy = malloc(PATH_MAX);
                    strcpy(srcCpy, child->fts_path);
                    strcat(srcCpy, "/");
                    strcat(srcCpy, child->fts_name);

                    //TODO: Will only copy files up to one level. Find way to properly create destination path.
                    char* dstCpy = malloc(PATH_MAX);
                    strcpy(dstCpy, destination);
                    recursivePathBuilder(child->fts_level, dstCpy, child);
                    printf("Copying %s to %s\n", srcCpy, dstCpy);
                    fileCpy(srcCpy, dstCpy);
                    if(mode == MORPH) {
                        if(remove(srcCpy) != 0)
                        {
                            fprintf(stderr, "recursiveOperation: removal of file %s unsuccessful.\n", child->fts_name);
                        }
                    }
                    free(srcCpy);
                    free(dstCpy);
                }
                child = child->fts_link;
            }
            if(mode == MORPH) {
                if (remove(parent->fts_name) != 0) {
                    fprintf(stderr, "recursiveOperation: removal of directory %s unsuccessful.\n", parent->fts_name);
                }
            }
        }
    }
    if(mode == MORPH)
    {
        remove(source);
    }
    fts_close(srcFileStructure);
    return EXIT_SUCCESS;
}

int trsh_fts_cmp(const FTSENT** one, const FTSENT** two)
{
    return strcmp((*one)->fts_name, (*two)->fts_name);
}

int fileCpy(char* source, char* destination)
{
    int cpy;

    FILE *sourceFile, *targetFile;

    sourceFile = fopen(source, "r"); //Attempt to open source.

    if (sourceFile == NULL) { //If unsuccessful, failure.
        fprintf(stderr, "mimic: Invalid Source.\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    targetFile = fopen(destination, "w"); //Attempt to open destination.

    if (targetFile == NULL) { //If unsuccessful, failure.
        fclose(sourceFile);
        fprintf(stderr, "mimic: Invalid Destination. %s\n", destination);
        fflush(stderr);
        return EXIT_FAILURE;
    }

    while ((cpy = fgetc(sourceFile)) != EOF) {
        fputc(cpy, targetFile); //Copy until the end of the file is reached.
    }

    fclose(sourceFile); //Close both files
    fclose(targetFile);

    return EXIT_SUCCESS;
}

int recursivePathBuilder(int level, char* path, FTSENT* location)
{
    if (level > 0)
    {
        recursivePathBuilder(level-1, path, location->fts_parent);
    }
    strcat(path, "/");
    strcat(path, location->fts_name);
    return 0;
}