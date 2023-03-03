#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

typedef struct A A;
typedef void *(*callback)(void *);

struct A {
	pthread_t tid;
	callback cb;
	bool done;
	size_t ticks;
	size_t count;
	size_t t;
};

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void *fn(void *a) {
	A *b=(A*)a;
	while(!b->done) {
		msleep(b->t);
		b->cb(b);
	}
}

A *A_Create(callback cb,size_t t,size_t c) {
	pthread_t tid;
	A *a=malloc(sizeof(A));
	a->tid=tid;
	a->cb=cb;
	a->done=false;
	a->ticks=0;	
	a->count=c;
	a->t=t;
	return a; 	
}

void A_Start(A *a) {
	pthread_create(&a->tid,NULL,fn,a);
	pthread_join(a->tid,NULL);
}



void *click(void *a) {
	A *b=(A*)a;
	b->ticks++;
	printf("%zu\n",b->ticks);
	if(b->count!=0 && b->ticks>=b->count) {
		b->done=true;
		printf("done\n");
	}
}



int main() {
	A *a=A_Create(click,1000,10);
	A_Start(a);
	return 0;
}
