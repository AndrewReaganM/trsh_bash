Project 2
Andrew Massey - andrewreaganm@ou.edu
10/23/2018

In this project, I created a program to accept stdin input, parse it, and either execute an internal function, or execute a bash function. This was accomplished using an if/if else/else statement, which was the best way that I could think of to implement this.

To run this code, simply:
1) Place the code in the desired directory.
2) cd to the directory where it resides.
3) In bash, run 'make clean' and then 'make'.
4) In bash, run './project1'.

OPTIONAL:
If you would like to use macros, simply pipe the macro file into stdin, or supply the file path as an argument (e.g. './project1 macro.txt').

After the project is running, you will see a prompt that looks like this: '==>'. You can now enter your own command.

This shell runs any bash command, as well as any of the custom commands implemented in the code.

Supported Commands:
	esc - exits the shell.
	wipe -  clears the screen.
	files - prints the files in the current directory.
	environ - prints the current system environment variables.
	ditto - repeats the input.
	help - prints this readme.
	mimic - file copying recursively and non-recursively.
	erase - file deletion.
	morph - file or directory moving recursively and non-recursively.
	chdir - change the working directory of the shell.

Sources Cited:

General structure and implementation:
    https://brennan.io/2015/01/16/write-a-shell-in-c/

How to use calloc with the char data type:
    https://www.tutorialspoint.com/c_standard_library/c_function_calloc.htm\

Using malloc:
    https://en.cppreference.com/w/c/memory/malloc

How to allocate memory for a char array:
    https://stackoverflow.com/questions/32442600/memory-allocation-for-char-array

Using realloc:
    https://www.tutorialspoint.com/c_standard_library/c_function_realloc.htm

Listing all files in a directory:
    https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

execvp info:
    https://linux.die.net/man/3/execvp

mkdir info:
    https://linux.die.net/man/2/mkdir

How to use fts:
    http://man7.org/linux/man-pages/man3/fts.3.html

fts example:
    https://stackoverflow.com/questions/12609747/traversing-a-filesystem-with-fts3

basename info:
    http://man7.org/linux/man-pages/man3/basename.3.html

Traversal info:
    https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/

Remove info:
    https://www.tutorialspoint.com/c_standard_library/c_function_remove.htm

Stat file type testing info:
    https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html

dirname info:
    http://man7.org/linux/man-pages/man1/dirname.1.html

Using stat for file metadata:
    https://linux.die.net/man/2/stat

File printing guidance:
    https://www.geeksforgeeks.org/c-program-print-contents-file/

Finding if directory is empty:
    https://stackoverflow.com/questions/6383584/check-if-a-directory-is-empty-using-c-on-linux

