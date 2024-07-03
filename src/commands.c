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
int del_line( char *args[] );
int save();

int isnum(char c[]);
int line_counter( char text[] );

FILE *fp;

char *cmds[]={
	"q",
	"p",
	"lc",
	"a",
	"d",
	"s",
	"h",
};
char *helpStr[]={
	"Quit program",
	"Print \n   args: \n   '.'':All lines\n   '(number)'nth line\n   default:current line",
	"Line count",
	"Appened lines to the end",
	"Delete focus line   \n   args: \n   '(number)'nth line\n   default:current line",
	"Saves the file",
	"Print this text",
};
char *textFile;

int ( *cmdsFunc[] )( char *arg[] )={
	&quit,
	&print,
	&line_count,
	&append,
	&del_line,
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
				int lineCount=0;
				n=strlen(textFile);
				for (size_t i=0; i<n-1; i++) {
					if (i==0){
						printf("%d| ", lineCount);
						lineCount++;
					}
					if (textFile[i]!='\n') putchar(textFile[i]);
					if (textFile[i]=='\n') {
						printf("\n%d| ", lineCount);
						lineCount++;
					}
					if (textFile[i+1]=='\0') break;
				}
				putchar('\n');
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

int del_line( char *args[] ){
	int line;
	( args[1]==NULL )?line=curLine+1:0;
	if (isnum(args[1]) && atoi(args[1])>lineCount-1 || atoi(args[1])<0) {
		printf("Out of range\n");
		return 1;
	}
	( isnum(args[1]) )?line=atoi(args[1])+1:0;
	size_t n=strlen(textFile);
	size_t endIndex=0;
	int lineCounter=0, startIndex=0;
	int isStart=0;
	for (size_t i=0; i<n; i++) {
		if (textFile[i]=='\n') {
			lineCounter++;
			if (lineCounter==line-1 && isStart==0) {
				startIndex=i+1;
				isStart=1;
			}
			if (lineCounter==line) {
				endIndex=i+1;
				break;
			}
		}
	}
	if (endIndex==0)endIndex=n;
	if (startIndex<endIndex && endIndex<=n) {
		size_t movedBytes=n-endIndex;
		size_t newSize=n-(endIndex-startIndex);
		memmove(textFile+startIndex, textFile+endIndex, movedBytes);
		//textFile=realloc(textFile, newSize+1);
		textFile[newSize]='\0';
	}
	return 0;
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

int isnum(char c[]){
	for (size_t i=0; i<strlen(c)-1; i++) {
		if (isdigit(c[i])==0) {
			return 0;
		}
	}
	return 1;
}
