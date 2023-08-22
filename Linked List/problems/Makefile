CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g
DEBUG=-g -ggdb -O0 -march=native

ELF=detect_loop

.PHONY: build, clean

build: $(ELF)

$(ELF): $(ELF).c
	$(CC) $(CFLAGS) $^ -o $@

run: $(ELF)
	./$<

clean:
	rm -f $(ELF)