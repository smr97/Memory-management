#include <stdio.h>
#include "mempool.h"
#include <pthread.h>
#include <semaphore.h>

pthread_t consumer, producer;
pthread_barrier_t b, c;
sem_t make, use;

void* mem[20];

void* produce(void* dummy)
{
	while(1)
	{
		//printf("%s", "producer spawned \n");
		int index = 0;
		pthread_barrier_wait(&b);
		for(; index<20; index++)
		{
			//printf("%s", "producer crossed barrier \n");
			sem_wait(&use);
			ret(mem[index], 8);
			//printf("%s", "returned \n");
		}
		
		int temp;
		
		for(temp = 0; temp<20; temp++)
		{	
			sem_post(&make);
		}
		pthread_barrier_wait(&c);
	}
}

void* consume(void* dummy)
{
	int sentinel;
	printf("%s", "Press any non zero integer then <Enter>\n");
	scanf("%d", &sentinel);
	while(sentinel)
	{
		int index = 0;
		for(; index<20; index++)
		{
			sem_wait(&make);
			mem[index] = alloc();
			int *temp = mem;
			*temp = index;
			printf("%d\n", *temp);
		}
		printf("%s", "Press 0 and <Enter> followed by ctrl+z to terminate. Else press non zero number and <Enter> to continue\n");
		scanf("%d", &sentinel);
		int temp; 
		for(temp = 0; temp<20; temp++)
		{	
			sem_post(&use);
		}
		pthread_barrier_wait(&b);
		pthread_barrier_wait(&c);
	}
}	

int main()
{
	
	int check = init_pool(8);
	sem_init(&make, 0, 20);
	sem_init(&use, 0, 0);
	pthread_barrier_init(&b, 0, 2);
	pthread_barrier_init(&c, 0, 2);
	pthread_create(&producer, 0, produce, NULL);
	pthread_create(&consumer, 0, consume, NULL);
	pthread_join(producer, 0);
	pthread_join(consumer, 0);
}
