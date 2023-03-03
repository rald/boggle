#ifndef BOGGLE_H
#define BOGGLE_H



#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define TRIE_IMPLEMENTATION
#include "trie.h"



void Boggle_AddWord(char ***found,size_t *nfound,char *word);

void Boggle_DFS(TrieNode *root,char ***found,size_t *nfound,int letters[BOX_SIZE][BOX_SIZE],int graph[BOX_SIZE][BOX_SIZE],char *choices,int x,int y,int depth);

int Boggle_LoadDice(char ***dice,size_t *ndice,char *filename);

void Boggle_ShuffleDice(char ***dice,size_t ndice);

int Boggle_LoadWords(TrieNode **root,char *filename);



#ifdef BOGGLE_IMPLEMENTATION



void Boggle_AddWord(char ***words,size_t *nwords,char *word) {
	*words=realloc(*words,sizeof(**words)*(*nwords+1));
	(*words)[(*nwords)++]=strdup(word);
}



void Boggle_DFS(TrieNode *root,char ***found,size_t *nfound,int letters[BOX_SIZE][BOX_SIZE],int graph[BOX_SIZE][BOX_SIZE],char *choices,int x,int y,int depth) {

	if(x<0 || x>=BOX_SIZE || y<0 || y>=BOX_SIZE) return;

	if(graph[y][x]==SEEN) return;

	if((root=root->next[letters[y][x]])==NULL) return;

	choices[depth++]=letters[y][x]+'a';
	choices[depth]='\0';

	if(root->mark && depth>=MIN_WORD_LEN) {
		Boggle_AddWord(found,nfound,choices);
	}

	graph[y][x]=SEEN;

	for(int j=-1;j<=1;j++) {
		for(int i=-1;i<=1;i++) {
			if(i || j) {
				Boggle_DFS(root,found,nfound,letters,graph,choices,x+i,y+j,depth);
			}
		}
	}

	graph[y][x]=NOT_SEEN;
	
}



int Boggle_LoadDice(char ***dice,size_t *ndice,char *filename) {
	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
	FILE *fin; 

	if((fin=fopen(filename,"r"))==NULL) return -1;

	while((rlen=getline(&line,&llen,fin))!=-1) {
		rmnl(line);
		trim(line);
		strlwr(line);
		
		Boggle_AddWord(dice,ndice,line);
	}

	return 0;
	
}



void Boggle_ShuffleDice(char ***dice,size_t ndice) {
	for(int i=ndice-1;i>0;i--) {
		int j=rand()%(i+1);
		char *tmp=(*dice)[i];
		(*dice)[i]=(*dice)[j];
		(*dice)[j]=tmp;
	}
}


int Boggle_LoadWords(TrieNode **root,char *filename) {
	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
	FILE *fin; 

	if((fin=fopen(filename,"r"))==NULL) return -1;

	*root=Trie_NewNode('\0');

	while((rlen=getline(&line,&llen,fin))>0) {
		rmnl(line);
		trim(line);
		strlwr(line);
		
		Trie_AddWord(root,line);
	}

	return 0;
	
}


void Boggle_FreeWords(char ***words,size_t *nwords) {
	for(size_t i=0;i<*nwords;i++) {
		free((*words)[i]);
		(*words)[i]=NULL;
	}
	free(*words);
	*words=NULL;
	*nwords=0;
}


#endif /* BOGGLE_IMPLEMENTATION */



#endif /* BOGGLE_H */


