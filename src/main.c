#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "commands.h"

#define DELIM " \t\r\n\v\f"

char start[]="→";
char *filename;

int status=1;

void loop();

int main( int argc, char *argv[] ){
	switch (argc) {
		case 2:
			filename=malloc(strlen(argv[argc-1])*sizeof(char)+1);
			strcpy(filename, argv[argc-1]);
			start_edit();
			loop();
			break;
		default:
			printf("Syntax: llbe 'filename'\n");
			break;
	}
}

void loop(){
	while (status) {
		printf("%s(%d)%s", filename, curLine, start);
		char inp[SENSIBLE_BUFFER_SIZE];
		char *inpTok[SENSIBLE_BUFFER_SIZE];
		fgets(inp, SENSIBLE_BUFFER_SIZE, stdin);
		tokenizer(inp, inpTok, DELIM);
		cmd_exec(inpTok[0], inpTok);
		memset(inp, 0, sizeof(inp));
		memset(inpTok, 0, sizeof(inpTok));
	}
}


int tokenizer( char inp[], char *inpTok[], char delim[] ){
	int tokenCount=0;
	char *check=strtok(inp, delim);
	if (inp[0]=='\0' || check==NULL) {return 0;}
	while (check) {
		inpTok[tokenCount++]=check;
		check = strtok(NULL, delim);
	}
	inpTok[tokenCount]=NULL;
	free(check);
	return 0;
}
