CC=gcc

all: install

install: main.c commands.c
	$(CC) -Wall -Wextra -ggdb3 -o llbe main.c commands.c

clean:
	rm llbe
