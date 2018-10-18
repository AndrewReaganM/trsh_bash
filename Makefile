CC=gcc

project2 : trsh.h project2.c trsh_internal_functions.c
	$(CC) project2.c trsh_internal_functions.c -o project2

all : project2.c
	$(CC) project2.c -o project2

clean :
	rm project2