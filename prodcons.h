#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#define DOWN 326
#define UP 327

struct cs1550_sem{
	int value;
	struct node* first; 
	struct node* last;
};

void up(struct cs1550_sem* semaphore);
void down(struct cs1550_sem* semaphore);
void produce(struct cs1550_sem *semAdr, int *dataAdr, int producers, int bufferSize);
void consume(struct cs1550_sem *semAdr, int *dataAdr, int consumers, int bufferSize);


