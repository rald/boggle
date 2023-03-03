#include <stdio.h>



#include "common.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define BRAIN_IMPLEMENTATION
#include "brain.h"

#define CSV_IMPLEMENTATION
#include "csv.h"


Brain **brains=NULL;
size_t nbrains=0;

int main(void) {

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;

  char **lines=NULL;
  size_t nlines=0;
  
  while((rlen=getline(&line,&llen,stdin))>0) {

    CSV_Parse(&lines,&nlines,line);

    Brain_Add(&brains,&nbrains,Brain_New(lines,nlines));

    lines=NULL;
    nlines=0;

    free(line);
    line=NULL;
    llen=0;
    rlen=0;
  }

  free(line);
  line=NULL;
  llen=0;
  rlen=0;



  Brains_Print(brains,nbrains);

  Brains_Free(&brains,&nbrains);



  return 0;
}



