CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: treeload treetest treesearch

treeload: treeload.c lib/tree.o
	$(CC) $(CFLAGS) -o $@ $^

treetest: treetest.c lib/tree.o
	$(CC) $(CFLAGS) -o $@ $^

treesearch: treesearch.c lib/tree.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f treeload treetest treesearch
	$(MAKE) -C lib clean
