#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"

typedef struct{
	unsigned int lineNum;
	char line[SENSIBLE_BUFFER_SIZE];
} Lines;

int quit();
int print( char *args[] );
int line_count();
int help( char *args[] );

int isnum(char c[]);

FILE *fp;

Lines *lines;

char *cmds[]={
	"q",
	"p",
	"lc",
	"h",
};

char *helpStr[]={
	"Quit program",
	"Print \n   args: \n   '.'':All lines\n   '(number)'nth line\n   default:curLine",
	"line count",
	"Print this text",
};

int ( *cmdsFunc[] )( char *arg[] )={
	&quit,
	&print,
	&line_count,
	&help
};
int cmdNum=sizeof(cmds)/sizeof(char*);
int curLine;
int lineCount;

int start_edit(){
	if ((fp=fopen(filename, "r"))==NULL) {
		perror("Filename");
		quit();
		return 1;
	}
	char path[SENSIBLE_BUFFER_SIZE];
	int counter=0;
	int linesSize=0;
	while (fgets(path, SENSIBLE_BUFFER_SIZE, fp)!=NULL) {
		linesSize+=sizeof(Lines);
		lines=realloc(lines, linesSize);
		strcpy(lines[counter].line, path);
		lines[counter].lineNum=counter;
		counter++;
	}
	lineCount=counter;
	fclose(fp);
	return 0;
}

int cmd_exec(char *cmd_name, char *args[]){
	for (int i=0; i<cmdNum; i++) {
		if (strcmp(cmd_name, cmds[i])==0) {
			return ( *cmdsFunc[i] )( args );
		}
	}
	if (isnum(cmd_name)) {
		curLine=atoi(cmd_name);
		if (curLine>=lineCount) {
			printf("Line number inputted out of file\n");
			curLine=lineCount-1;
		}
		return 0;
	}
	printf("Command does not exist\n");
	return 1;
}

int quit(){
	free(filename);
	free(lines);
	status=0;
	return 0;
}

int print( char *args[] ){
	if (args[1]==0) {
		printf("%d| %s", lines[curLine].lineNum,lines[curLine].line);
		return 0;
	}
	if (strcmp(args[1], ".")==0) {
		for (int i=0; i<lineCount; i++)
			printf("%d| %s", lines[i].lineNum,lines[i].line);
		return 0;
	}
	//RANGE
	/*if () {
	}*/
	if (isnum(args[1])) {
		int printNum=atoi(args[1]);
		if (printNum>=lineCount) {
			printf("ERROR: Line number is bigger than the line max\n");
			return 1;
		}
		printf("%d| %s", lines[printNum].lineNum,lines[printNum].line);
		return 0;
	}
	return 1;
}

int line_count(){
	printf("%d\n", lineCount);
	return 0;
}

int help( char *args[] ){
	for (int i=0; i<cmdNum; i++) {
		printf("%s: %s\n", cmds[i], helpStr[i]);
	}
	return 0;
}

int isnum(char c[]){
	for (int i=0; i<strlen(c)-1; i++) {
		if (isdigit(c[i])==0) {
			return 0;
		}
	}
	return 1;
}
