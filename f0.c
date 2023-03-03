#include <stddef.h>

#define TEXTTWIST_IMPLEMENTATION
#include "texttwist.h"

#define DICT_FILE "wordeng0.txt"
#define RAND_FILE "randeng.txt"



int main(void) {

	char **d=NULL;
	size_t nd=0;

	char **r=NULL;
	size_t nr=0;

	char **a=NULL;
	size_t na=0;

	TextTwist_LoadDict(&d,&nd,"wordlist.txt",3,8);
	TextTwist_LoadDict(&r,&nr,DICT_FILE,6,8);

	FILE *fout=fopen(RAND_FILE,"w");

	for(size_t i=0;i<nr;i++) {
		TextTwist_GetAnagrams(&a,&na,d,nd,r[i]);
		if(na>=10 && na<=30) {
			fprintf(fout,"%s\n",r[i]);
			fflush(fout);
			printf("%s\n",r[i]);
		}
		TextTwist_FreeWords(&a,&na);
	}
	fclose(fout);

	TextTwist_FreeWords(&d,&nd);
	TextTwist_FreeWords(&r,&nr);

	return 0;
}
