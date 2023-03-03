#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RE_IMPLEMENTATION
#include "re.h"

#define STRING_MAX 65536



int main(void) {

	#define pnum "\\d+[.?]\\d*"

	char *pat="ang presyo ng pagibig ay \\d+[.?]\\d* pesos";
	char *str="ang presyo ng pagibig ay 143.00 pesos";

	int idx=0,len=0;
	char res[STRING_MAX];

	idx=re_match(pat,str,&len);

	res[0]='\0';
	strncpy(res,str+idx,len);
	res[len]='\0';

	printf("idx: %d\n",idx);
	printf("len: %d\n",len);
	printf("res: '%s'\n",res);

	return 0;
}

