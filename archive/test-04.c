#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_IMPLEMENTATION
#include "random.h"

int main(void)	{

	srand(time(NULL));

	printf("%d\n",randi2(3,5));

	return 0;
}
