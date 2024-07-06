#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strutils.h"

int isnum(char c[]){
	for (size_t i=0; i<strlen(c)-1; i++) {
		if (isdigit(c[i])==0) {
			return 0;
		}
	}
	return 1;
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

Split str_split(char *textFile, int splitPos, int lineSkip){
	Split splitText;
	splitText.befSize=sizeof(char);
	splitText.aftSize=sizeof(char);
	splitText.bef=malloc(splitText.befSize);
	splitText.aft=malloc(splitText.aftSize);
	size_t len=strlen(textFile);
	int lineCounter=0;
	for (int i=0; i<len; i++) {
		if (textFile[i]=='\n') lineCounter++;
		if (lineCounter<splitPos) {
			splitText.befSize+=sizeof(char);
			splitText.bef=realloc(splitText.bef, splitText.befSize);
			splitText.bef[i]=textFile[i];
		}
		if (lineCounter<splitPos+lineSkip) {
			splitText.aftSize+=sizeof(char);
			splitText.aft=realloc(splitText.aft, splitText.aftSize);
			splitText.aft[i]=textFile[i];
		}
	}
	splitText.bef[splitText.befSize]='\0';
	splitText.aft[splitText.aftSize]='\0';
	printf("%s|%s", splitText.bef, splitText.aft);
	free(splitText.bef);
	free(splitText.aft);
}
