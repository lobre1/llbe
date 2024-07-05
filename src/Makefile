CC=gcc
NAME=llbe

all: install

install: main.c commands.c
	$(CC) -o $(NAME) main.c commands.c

debug: main.c commands.c
	$(CC) -Wall -Wextra -ggdb3 -o $(NAME) main.c commands.c

clean:
	rm $(NAME)
