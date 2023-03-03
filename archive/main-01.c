#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DYAD_IMPLEMENTATION 
#include "dyad.h"

#define INFO_IMPLEMENTATION 
#include "info.h"

#define LEXER_IMPLEMENTATION 
#include "lexer.h"

#define PARSER_IMPLEMENTATION 
#include "parser.h"

#define CITE_IMPLEMENTATION 
#include "cite.h"



static char *server  = "irc.dal.net";
static char *channel = "#pantasya";
static char *password = "paanoanggagawinko";
static char nick[32] = "siesto";
static int  isRegistered = 0;

static char buf[1024];
static char src[32], usr[32], srv[256], dst[32], txt[512];

static Info **infos=NULL;
static size_t ninfos=0;

static Token **tokens=NULL;
static size_t ntokens=0;

static Cite **cites=NULL;
static size_t ncites=0;

static char *skip(char *s, char c) {
	while (*s != c && *s != '\0')
		s++;
	if (*s != '\0')
		*s++ = '\0';
	return s;
}

static void onConnect(dyad_Event *e) {
  /* Introduce ourselves to the server */
  dyad_writef(e->stream, "NICK %s\r\n", nick);
  dyad_writef(e->stream, "PASS %s\r\n", password);
  dyad_writef(e->stream, "USER %s %s %s :%s\r\n", nick, nick, nick, nick);
}

static void onError(dyad_Event *e) {
  printf("error: %s\n", e->msg);
}

static void onLine(dyad_Event *e) {
//  printf("%s\n\n", e->data);

  /* Handle PING */
  if (!memcmp(e->data, "PING", 4)) {
    dyad_writef(e->stream, "PONG%s\r\n", e->data + 4);
  }
  
  /* Handle RPL_WELCOME */
  if (!isRegistered && strstr(e->data, "001")) {
    /* Join channel */
    dyad_writef(e->stream, "JOIN %s\r\n", channel);
    isRegistered = 1;
  }

  if (strstr(e->data, "432")) {
	  dyad_writef(e->stream, "NICK %s%04x\r\n",nick,rand()%0xFFFF);		
    dyad_writef(e->stream, "PRIVMSG NickServ@services.dal.net :release %s %s\r\n",nick,password);
	  dyad_writef(e->stream, "NICK %s\r\n",nick);		
  }

	strcpy(buf,e->data);

	if(sscanf(buf, ":%31[^!]!~%31[^@]@%255s PRIVMSG %31s :%511[^\n]", src, usr, srv, dst, txt)==5) {
		printf("%s <%s> %s\n",dst,src,txt);

		lex(&tokens,&ntokens,txt);

		parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

		Cites_Print(e->remote,channel,infos,ninfos,cites,ncites);

		Tokens_Free(&tokens,&ntokens);
		Cites_Free(&cites,&ncites);
	}

}

int main(void) {
	srand(time(NULL));

	Info_Load(&infos,&ninfos,"kjv.inf");
	
  dyad_Stream *s;
  dyad_init();

  s = dyad_newStream();
  dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
  dyad_addListener(s, DYAD_EVENT_ERROR,   onError,   NULL);
  dyad_addListener(s, DYAD_EVENT_LINE,    onLine,    NULL);
  dyad_connect(s, server, 6667);

  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }
  
  dyad_shutdown();
  return 0;
}
