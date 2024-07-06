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
	splitText.bef=malloc(splitText.befSize);
	splitText.bef[0]='\0';

	splitText.aftSize=sizeof(char);
	splitText.aft=malloc(splitText.aftSize);
	splitText.aft[0]='\0';

	size_t len=strlen(textFile);
	size_t befIndex=0, aftIndex=0;


	int lineCounter=0;
	for (int i=0; i<len; i++) {
		if (textFile[i]=='\n') lineCounter++;
		if (lineCounter<splitPos) {
			splitText.befSize+=sizeof(char);
			splitText.bef=realloc(splitText.bef, splitText.befSize);
			splitText.bef[befIndex]=textFile[i];
			befIndex++;
		}
		if (lineCounter>=splitPos+lineSkip) {
			splitText.aftSize+=sizeof(char);
			splitText.aft=realloc(splitText.aft, splitText.aftSize);
			splitText.aft[aftIndex]=textFile[i];
			aftIndex++;
		}
	}

	splitText.bef[befIndex]='\0';
	splitText.aft[aftIndex]='\0';
	return splitText;
}

void prepend( char *s, char *t ){
  size_t len=strlen(t);
  memmove(s+len, s, strlen(s)+1);
  memcpy(s, t, len);
}
