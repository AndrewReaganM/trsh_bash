CC=gcc

project2 : project2.c
	$(CC) project2.c -o project2

all : project2.c
	$(CC) project2.c -o project2

clean :
	rm project2