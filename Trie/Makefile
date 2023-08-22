CC = gcc
CFLAGS = -Wall -g

.PHONY:
all: trie
	
run: trie
	./trie

trie:	src/trie.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:
clean:
	-rm trie
