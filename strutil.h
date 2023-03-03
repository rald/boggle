#ifndef STRUTIL_H
#define STRUTIL_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>



char *trim(char *a);
void tokenize(char ***tokens,size_t *ntokens,char *s,char *d);
void tokfree(char ***tokens,size_t *ntokens);
char *rmnl(char *s);
char *rmcrnl(char *s);
char *skip(char *s, char c);
char *escape(char *s);
char *unescape(char *s);
char *strlwr(char *s);
char *strupr(char *s);

#ifdef STRUTIL_IMPLEMENTATION



char *trim(char *a) {
    char *p=a,*q=a;
    while(isspace(*q)) ++q;
    while(*q) *p++=*q++;
    *p='\0';
    while(p>a && isspace(*--p)) *p='\0';
    return a;
}



void tokenize(char ***tokens,size_t *ntokens,char *s,char *d) {
  char *token=strtok(s,d);
  while(token) {
    *tokens=realloc(*tokens,sizeof(**tokens)*(*ntokens+1));
    (*tokens)[(*ntokens)++]=token?strdup(token):NULL;
    token=strtok(NULL,d);
  }
}



void tokfree(char ***tokens,size_t *ntokens) {
  for(size_t i=0;i<*ntokens;i++) {
    if((*tokens)[i]) {
    	free((*tokens)[i]);
    }
    (*tokens)[i]=NULL;
  }
  free(*tokens);
  *ntokens=0;
}



char *rmnl(char *s) {
  char *p=strchr(s,'\n');
  if(p) *p='\0';
  return s;
}

char *rmcrnl(char *s) {
  char *p=strstr(s,"\r\n");
  if(p) *p='\0';
  return s;
}



char *skip(char *s, char c) {
	while (*s != c && *s != '\0')
		s++;
	if (*s != '\0')
		*s++ = '\0';
	else
		return NULL;
	return s;
}



char *escape(char *s) {
  size_t n=strlen(s);
  char *d=calloc(n+1,sizeof(*d));
  size_t i=0,j=0;
  while(i<n) {
    switch(s[i]) {
      case '\a': d[j++]='\\'; d[j++]='a'; break;
      case '\b': d[j++]='\\'; d[j++]='b'; break;
      case '\f': d[j++]='\\'; d[j++]='f'; break;
      case '\n': d[j++]='\\'; d[j++]='n'; break;
      case '\r': d[j++]='\\'; d[j++]='r'; break;
      case '\t': d[j++]='\\'; d[j++]='t'; break;
      case '\v': d[j++]='\\'; d[j++]='v'; break;
      case '\'': d[j++]='\\'; d[j++]='\''; break;
      case '\"': d[j++]='\\'; d[j++]='\"'; break;
      case '\?': d[j++]='\\'; d[j++]='\?'; break;
      case '\\': d[j++]='\\'; d[j++]='\\'; break;
      default: d[j++]=s[i];
    }
    i++;
  }
  d[j]='\0';
  return d;
}



char *unescape(char *s) {
  size_t n=strlen(s);
  char *d=calloc(n+1,sizeof(*d));
  size_t i=0,j=0;
  while(i<n) {
    if(s[i]=='\\') {
      i++;
      switch(s[i]) {
        case 'a': d[j++]='\a'; break;
        case 'b': d[j++]='\b'; break;
        case 'f': d[j++]='\f'; break;
        case 'n': d[j++]='\n'; break;
        case 'r': d[j++]='\r'; break;
        case 't': d[j++]='\t'; break;
        case 'v': d[j++]='\v'; break;
        case '\'': d[j++]='\''; break;
        case '\"': d[j++]='\"'; break;
        case '\?': d[j++]='\?'; break;
        case '\\': d[j++]='\\'; break;
        default: d[j++]=s[i];
      }
    } else d[j++]=s[i];
    i++;
  }
  return d;
}


char *strlwr(char *s) {
	char *p=s;
	while(*p) {
		*p=tolower(*p);
		p++;
	}
	return s;
}



char *strupr(char *s) {
	char *p=s;
	while(*p) {
		*p=toupper(*p);
		p++;
	}
	return s;
}



#endif /* STRUTIL_IMPLEMENTATION */



#endif /* STRUTIL_H */


