CC=gcc
CFLAGS=-g -Wall -ansi -pedantic

dwt: dwt.c
	$(CC) $(CFLAGS) -o dwt dwt.c

clean:
	rm -rf dwt
