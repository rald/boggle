#ifndef TRIE_H
#define TRIE_H



#include <stdlib.h>



#define NUM_LETTERS 26



typedef struct TrieNode TrieNode;
struct TrieNode {
  int letter;
  int mark;
  TrieNode *next[NUM_LETTERS];
};



TrieNode *Trie_NewNode(int letter);

void Trie_AddWord(TrieNode **root,char *word);
void Trie_WalkNode(TrieNode *n, int depth);

void Trie_Walk(TrieNode *root) ;



#ifdef TRIE_IMPLEMENTATION



TrieNode *Trie_NewNode(int letter) {
	TrieNode *node=malloc(sizeof(*node));
	if(node) {
		node->letter=letter;
		node->mark=0;
		for(int i=0;i<NUM_LETTERS;i++) {
			node->next[i]=NULL;
		}
	}
	return node;
}



void Trie_AddWord(TrieNode **root,char *word) {
	TrieNode *currNode=*root;
	char *currLet;
	int nextLet;
	if(word==NULL || *word=='\0') return;
	currLet=word;
	while(*currLet) {
		nextLet=tolower(*currLet)-'a';
		if(nextLet<0 || nextLet>=NUM_LETTERS) return;
		if(currNode->next[nextLet]==NULL) {
			currNode->next[nextLet]=Trie_NewNode(nextLet);
		}
		currNode=currNode->next[nextLet];
		currLet++;
	}
	currNode->mark=1;
}



char walk_buf[128];
void Trie_WalkNode(TrieNode *n, int depth){
  int i;

  depth++;

  if (n->mark) {
    walk_buf[depth] = '\0';
    puts(walk_buf);
  }

  for (i = 0; i < NUM_LETTERS; i++) {
    if (n->next[i] != NULL) {
      walk_buf[depth] = i + 'a';
      Trie_WalkNode(n->next[i], depth);
    }
  }
}



void Trie_Walk(TrieNode *root) {
	Trie_WalkNode(root,-1);
}



#endif /* TRIE_IMPLEMENTATION */



#endif /* TRIE_H */


