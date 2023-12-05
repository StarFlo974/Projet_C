CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: tree.o

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f tree.o
