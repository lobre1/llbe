#ifndef STRUTILS_H

typedef struct {
	char *bef, *aft;
	int befSize, aftSize;
} Split;

int isnum(char c[]);
int line_counter(char *text);

Split str_split(char *textFile, int splitPos, int lineSkip);

void prepend( char *s, char *t );

#endif
