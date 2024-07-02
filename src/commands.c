#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"

int quit();
int print( char *args[] );
int line_count();
int help( char *args[] );
int append();
int save();

int isnum(char c[]);
int line_counter( char text[] );

FILE *fp;

char *cmds[]={
	"q",
	"p",
	"lc",
	"a",
	"s",
	"h",
};
char *helpStr[]={
	"Quit program",
	"Print \n   args: \n   '.'':All lines\n   '(number)'nth line\n   default:curLine",
	"line count",
	"appened lines to the end",
	"Saves the file",
	"Print this text",
};
char *textFile;

int ( *cmdsFunc[] )( char *arg[] )={
	&quit,
	&print,
	&line_count,
	&append,
	&save,
	&help
};
int cmdNum=sizeof(cmds)/sizeof(char*);
int curLine;
int lineCount;
int textSize=0;

int start_edit(){
	textFile=malloc(1);
	textFile[0]='\0';
	if ((fp=fopen(filename, "r"))==NULL) {
		perror("Filename");
		quit();
		return 1;
	}
	int c;
	while ((c=getc(fp))!=EOF) {
		textFile[textSize]=c;
		textSize+=1;
		textFile=realloc(textFile, textSize+1);
	}
	textFile[textSize]='\0';
	lineCount=line_counter(textFile);
	fclose(fp);
	return 0;
}

int cmd_exec(char *cmd_name, char *args[]){
	int size=0;
	while (args[size]!=NULL) size++;
	if (size==0) { return 0; }
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
	free(textFile);
	status=0;
	return 0;
}

//ADD RANGE PRINT
int print( char *args[] ){
	if (textFile == NULL) {
		printf("No file loaded\n");
		return 1;
	}
	int size=0, counter=0;
	size_t n;
	while (args[size]!=NULL) size++;
	switch (size) {
		case 1:
			n=strlen(textFile);
			for (size_t i=0; i<n; i++) {
				if (textFile[i]=='\n' || textFile[i]=='\0') {
					counter++;
				}
				if (counter==curLine && textFile[i]!='\n') putchar(textFile[i]);
			}
			printf("\n");
			break;
		default:
			if (strcmp(args[1],".")==0) {
				printf("%s", textFile);
				return 0;
			}
			if (isnum(args[1])) {
				if (atoi(args[1])>=lineCount) {
					printf("Line is out of file\n");
					return 1;
				}
				n=strlen(textFile);
				for (size_t i=0; i<n; i++) {
					if (textFile[i]=='\n' || textFile[i]=='\0') {
						counter++;
					}
					if (counter==atoi(args[1]) && textFile[i]!='\n') putchar(textFile[i]);
				}
				printf("\n");
				return 0;
			}
			break;
	}
	return 1;
}

int line_count(){
	printf("%d\n", line_counter(textFile));
	return 0;
}

int append(){
	int isTyping=1;
	int inpLength;
	char inp[SENSIBLE_BUFFER_SIZE]={0};
	while (isTyping) {
		fgets(inp, sizeof(inp), stdin);
		if (strcmp(inp, ".\n")==0) {
			break;
		}
		textSize+=strlen(inp);
		textFile=realloc(textFile, textSize);
		strcat(textFile, inp);
		lineCount++;
		curLine=lineCount-1;
	}
	return 0;
}

int help( char *args[] ){
	for (int i=0; i<cmdNum; i++) {
		printf("%s: %s\n", cmds[i], helpStr[i]);
	}
	return 0;
}

int line_counter(char *text){
	if (text == NULL) return 0;
	int counter=0;
	size_t len=strlen(text);
	for (size_t i=0; i<len; i++) {
		if (text[i]=='\n' || text[i]=='\0') {
			counter++;
		}
	}
	return counter;
}

int save(){
	fp=fopen(filename, "w");
	if (fp==NULL) {
		printf("Cannot find file\n");
		return 1;
	}
	fprintf(fp, "%s", textFile);
	printf("Saved to file\n");
	fclose(fp);
	return 0;
}

int isnum(char c[]){
	for (size_t i=0; i<strlen(c)-1; i++) {
		if (isdigit(c[i])==0) {
			return 0;
		}
	}
	return 1;
}
