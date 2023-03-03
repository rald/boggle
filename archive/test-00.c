#include <stdio.h>
#include <string.h>

static char *skip(char *s, char c) {
	while (*s != c && *s != '\0')
		s++;
	if (*s != '\0')
		*s++ = '\0';
	return s;
}


int main(void) {
	char s[]=":siesta!~siesta@pantasya.rf.gd PRIVMSG #pantasya :hello world\r\n";

	char *src=NULL;
	char *cmd=NULL;
	char *dst=NULL;
	char *txt=NULL;

	src=s;
	cmd=skip(src,' ');
	dst=skip(cmd,' ');
	txt=skip(dst,' ');
		
	printf("src: %s\n",src);
	printf("cmd: %s\n",cmd);
	printf("dst: %s\n",dst);
	printf("txt: %s\n",txt);

	return 0;
}
