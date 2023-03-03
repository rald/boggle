#ifndef TEXTTWIST_H
#define TEXTTWIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

size_t *TextTwist_Freq(char *w);

bool TextTwist_IsZero(size_t *f);

bool TextTwist_IsAnagram(char *w1,char *w2);

void TextTwist_AddWord(char ***words,size_t *nwords,char *word);

void TextTwist_GetAnagrams(char ***anagrams,size_t *nanagrams,char **dict,size_t ndict,char *word);

void TextTwist_LoadDict(char ***dict,size_t *ndict,char *filename,size_t min,size_t max);

char *TextTwist_ShuffleWord(char *w);

void TextTwist_ShuffleAnagrams(char ***a,size_t na);

void TextTwist_SortAnagrams(char ***a,size_t na);
void TextTwist_FreeWords(char ***w,size_t *nw);



#ifdef TEXTTWIST_IMPLEMENTATION
	


size_t *TextTwist_Freq(char *w) {
	size_t *f=calloc(26,sizeof(*f));
	for(size_t i=0;w[i];i++) {
		if(isalpha(w[i])) {
			f[toupper(w[i])-'A']++;
		}
	}
	return f;
}

bool TextTwist_IsZero(size_t *f) {
	for(size_t i=0;i<26;i++) {
		if(f[i]!=0) return false;
	}
	return true;
}


bool TextTwist_IsAnagram(char *w1,char *w2) {
	bool res=true;
	size_t *f1=TextTwist_Freq(w1);
	size_t *f2=TextTwist_Freq(w2);

	if(TextTwist_IsZero(f1) || TextTwist_IsZero(f2)) 
		res=false; 
	else
		for(size_t i=0;i<26;i++) {
	 		if(f1[i]<f2[i]) { res=false; break; }
		}

	free(f1);
	f1=NULL;
	
	free(f2);
	f2=NULL;
		
	return res;
}

void TextTwist_AddWord(char ***words,size_t *nwords,char *word) {
	*words=realloc(*words,sizeof(**words)*(*nwords+1));
	(*words)[(*nwords)++]=strdup(trim(word));
}

void TextTwist_GetAnagrams(char ***anagrams,size_t *nanagrams,char **dict,size_t ndict,char *word) {
	for(size_t i=0;i<ndict;i++) {
		if(TextTwist_IsAnagram(word,dict[i])) {
			TextTwist_AddWord(anagrams,nanagrams,dict[i]);
			if(*nanagrams>30) break;
		}
	}
}

void TextTwist_LoadDict(char ***dict,size_t *ndict,char *filename,size_t min,size_t max) {
	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;

	FILE *fin=fopen(filename,"r");
	while((rlen=getline(&line,&llen,fin))!=-1) {
		char *p=strchr(line,'\n'); if(p) *p='\0';
		size_t len=strlen(trim(line));
		if(len>=min && len<=max) TextTwist_AddWord(dict,ndict,line);
		free(line);
		line=NULL;
		llen=0;
		rlen=0;
	}
	free(line);
	line=NULL;
	llen=0;
	rlen=0;
	fclose(fin);	
}

void TextTwist_PrintWords(char **w,size_t nw) {
	for(size_t i=0;i<nw;i++) {
		if(i!=0) printf(", ");
		printf("%s",w[i]);
	}
	printf("\n");
}

char *TextTwist_ShuffleWord(char *w) {
	for(size_t i=strlen(w)-1;i>0;i--) {
		size_t j=rand()%(i+1);
		char t=w[i];
		w[i]=w[j];
		w[j]=t;
	}
	return w;
}

void TextTwist_ShuffleAnagrams(char ***a,size_t na) {
	for(size_t i=na-1;i>0;i--) {
		size_t j=rand()%(i+1);
		char *t=(*a)[i];
		(*a)[i]=(*a)[j];
		(*a)[j]=t;
	}
}

void TextTwist_SortAnagrams(char ***a,size_t na) {
	for(size_t j=0;j<na-1;j++) {
		for(size_t i=j+1;i<na;i++) {
			if(strlen((*a)[i])>strlen((*a)[j])) {
				char *t=(*a)[i];
				(*a)[i]=(*a)[j];
				(*a)[j]=t;
			}
		}
	}
}

void TextTwist_FreeWords(char ***w,size_t *nw) {	for(size_t i=0;i<*nw;i++) {
		free((*w)[i]);
		(*w)[i]=NULL;
	}
	free(*w);
	*w=NULL;
	*nw=0;
}



#endif /* TEXTTWIST_IMPLEMENTATION */



#endif /* TEXTTWIST_H */


