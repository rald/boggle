#include <stdio.h>

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"



int main(void) {

  char *s="Hello World\r\n";

  printf("%s\n",escape(s));

  return 0;
}
