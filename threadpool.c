#include <stdio.h>
#include "mempool.h"
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

typedef struct data{
int min;
int max;
double avg;
int number;
} sample;

sample s;

pthread_t consumer[5], producer;
pthread_barrier_t b, c;
sem_t make, use;
pthread_mutex_t qu, d;
mqd_t global, globalrcv;

#define QUEUE_PERMISSIONS 0760
#define MAX_MESSAGES 35
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

createQ()
{
	struct mq_attr buf;        /* buffer for stat info */
    buf.mq_msgsize = MAX_MSG_SIZE;
    buf.mq_maxmsg = MAX_MESSAGES;
    int flag1 = O_RDWR | O_NONBLOCK | O_CREAT;
    int flag2 = O_RDWR | O_CREAT;
	global = mq_open("/test", flag1, QUEUE_PERMISSIONS, &buf);
	globalrcv = mq_open("/test", flag2, QUEUE_PERMISSIONS, &buf);
	if(global<0)
	perror("mq_open()");
	char *buffer = (char*) malloc(MAX_MSG_SIZE);
	int pri = 1;
	while(1)
	{
		int ret = mq_receive(global, buffer, MSG_BUFFER_SIZE, &pri);
		if(ret<0)
		{
			break;
		}
	}
	free(buffer);
}

void empty_queue()
{
    char *buffer = (char*) malloc(MAX_MSG_SIZE);
	int pri = 1;
	while(1)
	{
		int ret = mq_receive(global, buffer, MSG_BUFFER_SIZE, &pri);
		if(ret<0)
		{
			break;
		}
	}
	free(buffer);
}

void* produce(void* dummy)
{
	int *ptr;
	while(1)
	{
		sem_wait(&make);
		ptr = (int*) alloc();
		*ptr = rand() % 100;
		printf("Random int is %d\n", *ptr);
		//pthread_mutex_lock(&qu);
		if(mq_send(global, (char*)(&ptr), MAX_MSG_SIZE, 1)==-1)
		{
			ret((void*)ptr, 4);
			perror("mq_send()");
			//empty_queue();
			//pthread_mutex_unlock(&qu);
			continue;
		}
		pthread_mutex_unlock(&qu);
		sem_post(&use);
	}
}

void compute(int num)
{
    pthread_mutex_lock(&d);
        if(num<s.min)
            s.min = num;
        if(num>s.max)   //can break lock into 2 here (?)
            s.max = num;
        if(s.number == 0)
        {
            s.avg = (double) num;
            s.number++;
        }
        else
        {
            double temp = s.avg*s.number;
            temp+= (double) num;
            s.avg = (double) (temp/(double)(++s.number)); 
        }
		printf("%d %d %f %d\n", s.min, s.max, s.avg, s.number);
    pthread_mutex_unlock(&d);
}

void* consume(void* dummy)
{
	int *temp;
	int pri = 1;
	while(1)
	{
		sem_wait(&use);
		//pthread_mutex_lock(&qu);
		if(mq_receive(globalrcv, (char*)(&temp), MSG_BUFFER_SIZE, &pri)<0)
		{
			perror("mq_receive()");
			//pthread_mutex_unlock(&qu);
			continue;
		}
		//pthread_mutex_unlock(&qu);
		compute(*temp);
		ret((void*)temp, 4);
		sem_post(&make);
	}
}

int main()
{
    s.min = 10000;
	pthread_mutex_init(&qu, 0);
	pthread_mutex_init(&d, 0);
	int check = init_pool(4);
	sem_init(&make, 0, 19);
	sem_init(&use, 0, 0);
	createQ();
	pthread_create(&producer, 0, produce, NULL);
	int i = 0;
	for(; i<5; i++)
	{
	   pthread_create(&(consumer[i]), 0, consume, NULL); 
	}
	pthread_join(producer, 0);
	pthread_join(consumer[0], 0);
}
