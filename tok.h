#ifndef TOK_H
#define TOK_H
struct TokenizerT_ {
char* fullString;
int inputLength;
int index;
char *tokenType;
};

typedef struct TokenizerT_ TokenizerT;


TokenizerT *TKCreate( char * ts );
char *TKGetNextToken( TokenizerT * tk );
void TKDestroy( TokenizerT * tk );

#endif
