CC=gcc
NAME=llbe
FILES=main.c commands.c strutils.c
CDEBUGFLAGS=-Wall -Wextra -ggdb3

all: install

install: $(FILES) 
	$(CC) -o $(NAME) $(FILES)

debug: $(FILES)
	$(CC) $(CDEBUGFLAGS) -o $(NAME) $(FILES)

clean:
	rm $(NAME)
