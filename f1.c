#include <stddef.h>

#define TEXTTWIST_IMPLEMENTATION
#include "texttwist.h"



int main(void) {

	char **d=NULL;
	size_t nd=0;

	char **r=NULL;
	size_t nr=0;

	char **a=NULL;
	size_t na=0;

	TextTwist_LoadDict(&d,&nd,"wordlist.txt",3,8);
	TextTwist_LoadDict(&r,&nr,"wordeng.txt",6,8);

	FILE *fout=fopen("wordeng0.txt","w");

	for(size_t i=0;i<nr;i++) {
		size_t l=strlen(r[i]);
		if(l>=6 && l<=8) {

			bool found=false;
			for(size_t j=0;j<nd;j++) {
				if(!strcasecmp(r[i],d[j])) {
					found=true;
					break;
				}
			}

			if(found) {
				fprintf(fout,"%s\n",r[i]);
				fflush(fout);
				printf("%s\n",r[i]);
			}
		}
	}
	fclose(fout);

	TextTwist_FreeWords(&d,&nd);
	TextTwist_FreeWords(&r,&nr);

	return 0;
}
