#ifndef IRC_H
#define IRC_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>



#include "common.h"


void raw(int conn,char *fmt, ...);

void privmsg(int conn,const char *dst,const char *fmt, ...);

void notice(int conn,const char *dst,const char *fmt, ...);

void DieWithUserMessage(const char *msg, const char *detail);

void DieWithSystemMessage(const char *msg);

int Irc_Connect(const char *host, const char *service);

int readline( int fd, char *bufptr, size_t len );

int Irc_Send(int sockfd, char *data, int datalen);

int Irc_Recv(int sockfd, char *data, int datalen);


#ifdef IRC_IMPLEMENTATION



void DieWithUserMessage(const char *msg, const char *detail) {
   fputs(msg, stderr);
   fputs(": ", stderr);
   fputs(detail, stderr);
   fputc('\n', stderr);
   exit(1);
}



void DieWithSystemMessage(const char *msg) {
   perror(msg);
   exit(1);
}



int Irc_Connect(const char *host, const char *service) {
   
   // Tell the system what kind(s) of address info we want
   struct addrinfo addrCriteria;                   // Criteria for address match
   memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
   addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
   addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
   addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

   // Get address(es)
   struct addrinfo *servAddr; // Holder for returned list of server addrs
   
   int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
   if (rtnVal != 0) {
      DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
   }
   
   int sock = -1;
   for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
      // Create a reliable, stream socket using TCP
      sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
      if (sock < 0) {
         continue;  // Socket creation failed; try next address
      }
   
      // Establish the connection to the echo server
      if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
         break;     // Socket connection succeeded; break and return socket
      }
      
      close(sock); // Socket connection failed; try next address
      sock = -1;
   }

   freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo()
   return sock;
}



int Irc_Send(int sockfd, char *data, int datalen) {
    int total_bytes_sent = 0;
    int n;

    while (total_bytes_sent < datalen) {
        n = send(sockfd, data + total_bytes_sent, datalen - total_bytes_sent, 0);
        if (n < 0) {
            perror("Irc_Send(): send");
            return n;
        }
        total_bytes_sent += n;
    }

    return total_bytes_sent;
}



int Irc_Recv(int sockfd, char *data, int datalen) {
	int ret=readline(sockfd,data,datalen);
	if(ret>0) printf(">> %s",data);
	return ret;
}


/* readline - read a '\n' terminated line from socket fd 
              into buffer bufptr of size len. The line in the
              buffer is terminated with '\0'.
              It returns -1 in case of error or if
              the capacity of the buffer is exceeded.
	      It returns 0 if EOF is encountered before reading '\n'.
 */
int readline( int fd, char *bufptr, size_t len )
{
  /* Note that this function is very tricky.  It uses the
     static variables bp, cnt, and b to establish a local buffer.
     The recv call requests large chunks of data (the size of the buffer).
     Then if the recv call reads more than one line, the overflow
     remains in the buffer and it is made available to the next call
     to readline. 
     Notice also that this routine reads up to '\n' and overwrites
     it with '\0'. Thus if the line is really terminated with
     "\r\n", the '\r' will remain unchanged.
  */
  char *bufx = bufptr;
  static char *bp;
  static int cnt = 0;
  static char b[ STRING_MAX ];
  char c;
  
  while ( --len > 0 ) {
    if ( --cnt <= 0 ) {
  	  cnt = recv( fd, b, sizeof( b ), 0 );
  	  if ( cnt < 0 ) {
  	    if ( errno == EINTR ) {
  		    len++;		/* the while will decrement */
  		    continue;
  		  }
  	    return -1;
  	  }
  	  if ( cnt == 0 )
  	    return 0;
  	  bp = b;
	  }
    c = *bp++;
    *bufptr++ = c;
    if ( c == '\n' ) {
	    *bufptr = '\0';
	    return bufptr - bufx;
	  }
  }
  errno = EMSGSIZE;
  return -1;
}



void raw(int conn,char *fmt, ...) {
	char p[STRING_MAX];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p, STRING_MAX, fmt, ap);
	va_end(ap);

	printf("<< %s", p);
	Irc_Send(conn,p,strlen(p));
}



void privmsg(int conn,const char *dst,const char *fmt, ...) {
	char p[STRING_MAX];
	char b[256],c[STRING_MAX];
	size_t i,j;

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p, STRING_MAX, fmt, ap);
	va_end(ap);

	j=0;
	while(p[j]) {
		i=0;
		while(i<255 && p[j]) {
			b[i]=p[j];
			i++;
			j++;
		}
		b[i]='\0';				
		sprintf(c,"PRIVMSG %s :%s\r\n",dst,b);
		printf("<< %s", c);
		Irc_Send(conn,c,strlen(c));
	}

}



void notice(int conn,const char *dst,const char *fmt, ...) {
	char p[STRING_MAX];
	char b[256],c[STRING_MAX];
	size_t i,j;

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p, STRING_MAX, fmt, ap);
	va_end(ap);

	j=0;
	while(p[j]) {
		i=0;
		while(i<255 && p[j]) {
			b[i]=p[j];
			i++;
			j++;
		}
		b[i]='\0';				
		sprintf(c,"NOTICE %s :%s\r\n",dst,b);
		printf("<< %s", c);
		Irc_Send(conn,c,strlen(c));
	}

}


#endif



#endif


