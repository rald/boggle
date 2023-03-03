#ifndef RANDOM_H
#define RANDOM_H



#include <stdlib.h>


double drand();

int rndi(int n);

int randi2(int s,int e);



#ifdef RANDOM_IMPLEMENTATION



double drand() {
	return rand()/(RAND_MAX+1.0);
}



int irandi(int n) {
	return (int)(drand()*n);
}



int irandi2(int s,int e) {
	return randi(e-s+1)+s;
}



#endif


#endif


