#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>



#define IRC_IMPLEMENTATION 
#include "irc.h"

#define INFO_IMPLEMENTATION 
#include "info.h"

#define LEXER_IMPLEMENTATION 
#include "lexer.h"

#define PARSER_IMPLEMENTATION 
#include "parser.h"

#define CITE_IMPLEMENTATION 
#include "cite.h"



char *nick = "siesto";
char *channel = "#pantasya";
char *host = "irc.dal.net";
char *port = "6667";
char *pass = "paanoanggagawinko"; 



int conn;
char sbuf[512];



static Info **infos=NULL;
static size_t ninfos=0;

static Token **tokens=NULL;
static size_t ntokens=0;

static Cite **cites=NULL;
static size_t ncites=0;



int main() {
	char *user, *command, *where, *message, *sep, *target;

	int i, j, l, sl, o = -1, start, wordcount;
	char buf[513];

//	struct addrinfo hints, *res;

	srand(time(NULL));

	Info_Load(&infos,&ninfos,"kjv.inf");

/*	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(host, port, &hints, &res);
	conn = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(conn, res->ai_addr, res->ai_addrlen);
*/

	conn=Irc_Connect(host,port);
  if (conn<0) {
  	DieWithUserMessage("Irc_Connect() failed", "unable to connect");
  }
   	
	if(pass) raw(conn,"PASS %s\r\n", pass);
	raw(conn,"NICK %s\r\n", nick);
	raw(conn,"USER %s %s %s :%s\r\n", nick, nick, nick, nick);
	
	while ((sl = read(conn, sbuf, 512))) {
		for (i = 0; i < sl; i++) {
			o++;
			buf[o] = sbuf[i];
			if ((i > 0 && sbuf[i] == '\n' && sbuf[i - 1] == '\r') || o == 512) {
				buf[o + 1] = '\0';
				l = o;
				o = -1;
				
				printf(">> %s", buf);
				
				if (!strncmp(buf, "PING", 4)) {
					buf[1] = 'O';
					raw(conn,buf);
				} else if (buf[0] == ':') {
					wordcount = 0;
					user = command = where = message = NULL;
					for (j = 1; j < l; j++) {
						if (buf[j] == ' ') {
							buf[j] = '\0';
							wordcount++;
							switch(wordcount) {
								case 1: user = buf + 1; break;
								case 2: command = buf + start; break;
								case 3: where = buf + start; break;
							}
							if (j == l - 1) continue;
							start = j + 1;
						} else if (buf[j] == ':' && wordcount == 3) {
							if (j < l - 1) message = buf + j + 1;
							break;
						}
					}
					
					if (wordcount < 2) continue;
					
					if (!strncmp(command, "001", 3) && channel != NULL) {
						raw(conn,"JOIN %s\r\n", channel);
					} else if (!strncmp(command, "433", 3)) {
					
						raw(conn,"NICK %s%04X\r\n", nick,rand()%0xFFFF);
					
						raw(conn,"PRIVMSG %s: NicServ@services.dal.net ghost %s %s\r\n",channel,nick,pass);						
						
					} else if (!strncmp(command, "PRIVMSG", 7) || !strncmp(command, "NOTICE", 6)) {
						if (where == NULL || message == NULL) continue;
						if ((sep = strchr(user, '!')) != NULL) user[sep - user] = '\0';
						if (where[0] == '#' || where[0] == '&' || where[0] == '+' || where[0] == '!') target = where; else target = user;

						printf("[from: %s] [reply-with: %s] [where: %s] [reply-to: %s] %s", user, command, where, target, message);

						size_t page=0;
						char text[STRING_MAX];

						if(sscanf(message,".kjv page %zu %[^\n]\n",&page,text)==2) {
						
							lex(&tokens,&ntokens,text);

							parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

							Cites_Print(conn,channel,page,infos,ninfos,cites,ncites);

							Tokens_Free(&tokens,&ntokens);
							Cites_Free(&cites,&ncites);

						} else if(sscanf(message,".kjv %[^\n]\n",text)==1) {
						
							lex(&tokens,&ntokens,text);

							parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

							Cites_Print(conn,channel,1,infos,ninfos,cites,ncites);

							Tokens_Free(&tokens,&ntokens);
							Cites_Free(&cites,&ncites);
									
									
						} else if(sscanf(message,".skjv page %zu %[^\n]\n",&page,text)==2) {
							search(conn,channel,page,text);	  					} else if(sscanf(message,".skjv %[^\n]\n",text)==1) {
							search(conn,channel,1,text);							}
						
						//raw("%s %s :%s", command, target, message); // If you enable this the IRCd will get its "*** Looking up your hostname..." messages thrown back at it but it works...

					}
				}
				
			}
		}
		
	}

	Infos_Free(&infos,&ninfos);
	
	return 0;
	
}
