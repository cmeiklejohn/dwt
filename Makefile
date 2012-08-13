CC=gcc
CFLAGS=-g -Wall -ansi -pedantic
LDFLAGS=-g -lncurses

dwt: dwt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o dwt dwt.c

clean:
	rm -rf dwt
