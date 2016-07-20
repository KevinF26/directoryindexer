#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<tok.h>


TokenizerT *TKCreate( char * ts ) {
  TokenizerT *input=malloc(sizeof(TokenizerT));
  input->inputLength=strlen(ts);
  input->fullString=malloc(input->inputLength+1);
  strcpy(input->fullString,ts);
  input->index=0;
  return input;
}

void TKDestroy( TokenizerT * tk ) {
free(tk->fullString);
free(tk);
}

char *TKGetNextToken( TokenizerT * tk ) {
  if(tk->index==tk->inputLength-1)
  {
	return 0;
  }
  int i=0;
  int error=0;
  char *curToken=malloc(tk->inputLength - tk->index);
  
  /*if current char is nonalphanumeric, continue until alphanum or end of string*/
  while(!isalpha(tk->fullString[tk->index]))
  {
	if(tk->index<(tk->inputLength-1))
	{
		tk->index++;
	}
	else
	{
		return 0;
	}
  }

  
  int endindex=0,startindex=tk->index;

  /*handle all alphanumeric tokens (ie word, digit, octal, hex, float*/
  if(isalpha(tk->fullString[tk->index]))
  {	
	/*words*/
	tk->tokenType="word";
  	while(tk->index<(tk->inputLength-1))
	{
		if(isalpha(tk->fullString[tk->index])||isdigit(tk->fullString[tk->index]))
       		{
       			tk->index++;
		}
		else
		{
			endindex=tk->index-1;
			break;
		}
  	}

  }
  if(tk->index==tk->inputLength-1)
  {
	endindex=tk->inputLength-2;
  }
  for(i=0;i<=(endindex-startindex);i++)
  {
	curToken[i]=tk->fullString[startindex+i];
  }
  return curToken;
}


