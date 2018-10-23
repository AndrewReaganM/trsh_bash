CC=gcc

project2 : trsh.h project2.c trsh_internal_functions.c morph_mimic.c
	$(CC) project2.c trsh_internal_functions.c morph_mimic.c -o project2

all : project2.c trsh.h trsh_internal_functions.c morph_mimic.c
	$(CC) project2.c trsh_internal_functions.c morph_mimic.c -o project2

clean :
	rm project2