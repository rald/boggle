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



#include "common.h"



#define INI_IMPLEMENTATION 
#include "ini.h"

#define STRUTIL_IMPLEMENTATION 
#include "strutil.h"

#define IRC_IMPLEMENTATION 
#include "irc.h"

#define BRAIN_IMPLEMENTATION 
#include "brain.h"

#define RANDOM_IMPLEMENTATION 
#include "random.h"

#define RE_IMPLEMENTATION 
#include "re.h"


#define BRAIN_FILE "brain.csv"

#define BUF_MAX 512



const char *hst = "irc.dal.net";
const char *prt = "6667";
const char *nck = "mia";
const char *chn = "#pantasya";
const char *pss = NULL;



int sck;



int main(void) {
	char *src, *dst, *cmd, *msg, *sep;

	int sl, wordcount;

	char buf[BUF_MAX];
	size_t buflen=BUF_MAX;


	Brain **brains=NULL;
	size_t nbrains=0;

	size_t bi=0,si=0;



	srand(time(NULL));



	Brain_Load(&brains,&nbrains,BRAIN_FILE);
	


  ini_t *config = ini_load("config.ini");

  const char *val; 

  val = ini_get(config,"default","host");

  hst=val?val:hst;

  val = ini_get(config,"default","port");

  prt=val?val:prt;

  val = ini_get(config,"default","nick");

  nck=val?val:nck;

  val = ini_get(config,"default","pass");

  pss=val?val:pss;

  val = ini_get(config,"default","chan");

  chn=val?val:chn;



	sck=Irc_Connect(hst,prt);
  if (sck<0) {
  	DieWithUserMessage("Irc_Connect() failed", "unable to connect");
  }


   	
	if(pss) raw(sck,"PASS %s\r\n",pss);
	raw(sck,"NICK %s\r\n",nck);
	raw(sck,"USER %s %s %s :%s\r\n",nck,nck,nck,nck);



  while ((sl=Irc_Recv(sck,buf,buflen))!=-1) {

    if(sl>0) {

  		printf(">> %s", buf);
  		
  		if (!strncmp(buf, "PING", 4)) {

  			buf[1] = 'O';
  			raw(sck,"%s",buf);

  		} else if (buf[0] == ':') {

  			wordcount = 0;
  			src = dst = cmd = msg = NULL;
  			
  			src=buf+1; 
  			wordcount++;
  			if((cmd=skip(src,' '))) {
  				wordcount++; 
  				if((dst=skip(cmd,' '))) {
  					wordcount++;
  					if((msg=skip(dst,' '))) {
  						msg=msg[0]==':'?msg+1:msg;
  						wordcount++;
  					}
  				}
  			}



  /*
  			printf("usr: %s\n",user);	
  			printf("cmd: %s\n",command);	
  			printf("whr: %s\n",where);	
  			printf("msg: %s\n\n",message);	
  */
  	


  			if (wordcount < 2) continue;


  			
  			if (!strncmp(cmd,"001",3)) {

  				raw(sck,"JOIN %s\r\n",chn);
  				
  			} else if (!strncmp(cmd,"PRIVMSG",7) || !strncmp(cmd,"NOTICE",6)) {

  				if ((sep = strchr(src, '!')) != NULL) src[sep - src] = '\0';

          printf("%s <%s> %s",dst,src,msg);

					if(msg && *msg) {

						int midx=0;
						int mlen=0;

						si=bi;

						do {

							bi++;
						
							if(bi>=nbrains) bi=0;

							if((midx=re_match(brains[bi]->lines[0],msg,&mlen))!=-1) {
								privmsg(sck,chn,"%s\n",brains[bi]->lines[randi2(1,brains[bi]->nlines-1)]);
								break;
							}						

						} while(bi!=si);
						
					}


  			}

  		}

  		buf[0]='\0';

  	}

  }

	return 0;
	
}


