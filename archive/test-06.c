#include <stdio.h>
#include <string.h>
#include <regex.h>

int main ()
{
  char * source = "$name = \"Harry\";$age = 16;";
  char * regexString = "(\\$\\w+)\\s*=\\s*(\\\"\\w*\\\"|\\d+\\.?\\d+)\\s*;";
  size_t maxGroups = 10;

  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];

  if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
  	printf("Could not compile regular expression.\n");
		return 1;
  }

  if (regexec(&regexCompiled, source, maxGroups, groupArray, 0) == 0) {
		for (size_t g = 0; g < maxGroups; g++) {
			if (groupArray[g].rm_so == -1) break;  // No more groups

			char sourceCopy[strlen(source) + 1];
			strcpy(sourceCopy, source);
			sourceCopy[groupArray[g].rm_eo] = 0;
			printf("Group %zu: [%2u-%2u]: %s\n",
					g, groupArray[g].rm_so, groupArray[g].rm_eo,
					sourceCopy + groupArray[g].rm_so);
		}
	}

  regfree(&regexCompiled);

  return 0;
}
