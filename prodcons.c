#include "prodcons.h"

void up(struct cs1550_sem* semaphore)
{
	syscall(UP, semaphore);
}

void down(struct cs1550_sem* semaphore)
{
	syscall(DOWN, semaphore);
}

void produce(struct cs1550_sem *semAdr, int *dataAdr, int producers, int bufferSize)
{
	int i;
	for(i = 0; i<producers; i++) 
	{
		if(fork()==0)
		{
			while(1)
			{
				int pitem;
				int *produced = dataAdr;
			    int *buffer = dataAdr+2;
				/* produce an item into pitem */
				down(semAdr);   //empty
				down(semAdr+2); //mutex
				pitem = *produced;
				buffer[*produced%bufferSize] = pitem;
				printf("\nChef %c produced pancake %d\n", (i+65), pitem);
				*produced = (*produced + 1);
			    up(semAdr+2); //mutex
				up(semAdr+1);	//full
			}
		}
	}	
}

void consume(struct cs1550_sem *semAdr, int *dataAdr, int consumers, int bufferSize)
{
	int i;
	for(i = 0; i<consumers; i++) 
	{
		if(fork()==0)
		{
			while(1)
			{
				int citem;
				int *consumed = dataAdr+1;
			    int *buffer = dataAdr+2;
				/* produce an item into citem */
				down(semAdr+1);   //full
				down(semAdr+2); //mutex
				citem = buffer[*consumed%bufferSize];
				printf("\nCustomer %c consumed pancake %d\n", (i+65), citem);
				*consumed = (*consumed + 1);
			    up(semAdr+2); //mutex
				up(semAdr);	//empty
			}
		}
	}	
}

int main(int argc, char *argv[])
{
	int producers = atoi(argv[1]);
	//printf("\nThere are %d producers\n", producers);
	int consumers = atoi(argv[2]);
	//printf("\nThere are %d consumers\n", consumers);
	int bufferSize = atoi(argv[3]);
	//printf("\nThe buffer is of length %d \n", bufferSize);

	//return 0;

	struct cs1550_sem *semAdr = (struct cs1550_sem *)mmap(NULL, 3*sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	int *dataAdr = (int*) mmap(NULL, (sizeof(int)*2) + (bufferSize)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

	struct cs1550_sem *empty;
	empty = semAdr;
	empty->value = bufferSize;
	empty->first = NULL;
	empty->last = NULL;
	struct cs1550_sem *full;
	full = semAdr+1;
	full->value = 0;
	full->first = NULL;
	full->last = NULL;
	struct cs1550_sem *mutex;
	mutex = semAdr+2;
	mutex->value = 1;
	mutex->first = NULL;
	mutex->last = NULL;

	int *produced = dataAdr;
	int *consumed = dataAdr+1;
	int *buffer = dataAdr+2;

	*produced = 0;
	*consumed = 0;

	produce(semAdr, dataAdr, producers, bufferSize);
	consume(semAdr, dataAdr, producers, bufferSize);

	return 0;
}


 