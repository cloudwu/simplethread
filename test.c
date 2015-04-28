#include "simplethread.h"
#include <stdio.h>

#define THREADS 30

struct args {
	int n;
	struct thread_event *wait;
	struct thread_event *trigger;
};

static void
func(void * p) {
	struct args * arg = p;
	if (arg->wait) {
		thread_event_wait(arg->wait);
	}
	printf("n = %d\n", arg->n);
	if (arg->trigger) {
		thread_event_trigger(arg->trigger);
	}
}

int
main() {
	struct thread t[THREADS];
	struct args args[THREADS];
	struct thread_event ev[THREADS];
	int i;
	for (i=0;i<THREADS;i++) {
		thread_event_create(&ev[i]);
		printf("create %d %p\n",i,&ev[i]);
		t[i].func = func;
		t[i].ud = &args[i];
		args[i].n = i;
		if (i+1<THREADS) {
			args[i].wait = &ev[i];
		} else {
			args[i].wait = NULL;
		}
		if (i-1 >=0) {
			args[i].trigger = &ev[i-1];
		} else {
			args[i].trigger = NULL;
		}
	}
	thread_join(t, THREADS);
	for (i=0;i<THREADS;i++) {
		thread_event_release(&ev[i]);
	}

	return 0;
}
