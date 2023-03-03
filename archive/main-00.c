#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define INFO_IMPLEMENTATION
#include "info.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define CITE_IMPLEMENTATION
#include "cite.h"

#define LEXER_IMPLEMENTATION
#include "lexer.h"

#define PARSER_IMPLEMENTATION
#include "parser.h"

void gotoxy(int x,int y) {
	printf("\x1B[%d;%dH",y,x);
}

void clrscr(void) {
	printf("\x1B[2J");
	gotoxy(1,1);
}

int main(void) {

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;

  Token **tokens=NULL;
  size_t ntokens=0;

  Cite **cites=NULL;
  size_t ncites=0;

  Info **infos=NULL;
  size_t ninfos=0;

  Info_Load(&infos,&ninfos,"kjv.inf");

	clrscr();

  printf("> ");    

  while((rlen=getline(&line,&llen,stdin)) && rlen>0) {

		rmnl(line);
		trim(line);

		if(strcmp(line,"/q")==0) {
			break;
		} else if(strcmp(line,"/c")==0) {
			clrscr();
		} else if(strncmp(line,"/s ",3)==0) {
			char *text=line+3;
			search(text);
		}

    lex(&tokens,&ntokens,line);

    parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

    Cites_Print(infos,ninfos,cites,ncites);

    Cites_Free(&cites,&ncites);

    Tokens_Free(&tokens,&ntokens);

    free(line);
    line=NULL;
    llen=0;
    rlen=0;

    printf("> ");    

  }

	free(line);
	line=NULL;
	llen=0;
	rlen=0;

	Infos_Free(&infos,&ninfos);

  return 0;
  
}
