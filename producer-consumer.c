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

pthread_t consumer, producer;
pthread_barrier_t b, c;
sem_t make, use;
pthread_mutex_t qu;
mqd_t global;

#define QUEUE_PERMISSIONS 0760
#define MAX_MESSAGES 35
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

createQ()
{
	struct mq_attr buf;        /* buffer for stat info */
    	buf.mq_msgsize = MAX_MSG_SIZE;
    	buf.mq_maxmsg = MAX_MESSAGES;
    	int flag = O_RDWR | O_NONBLOCK | O_CREAT;
	global = mq_open("/test", flag, QUEUE_PERMISSIONS, &buf);
	char *buffer = (char*) malloc(MAX_MSG_SIZE);
	int pri = 1;
	if(global<0)
	perror("mq_open()");
	while(1)		//to clear the queue of any garbage data
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
	int ctr = 0;
	while(1)
	{
		sem_wait(&make);
		ptr = (int*) alloc();
		*ptr = ctr;
		pthread_mutex_lock(&qu);
		if(mq_send(global, (char*)(&ptr), MAX_MSG_SIZE, 1)==-1)
		{
			ret((void*)ptr, 4);
			perror("mq_send()");
			pthread_mutex_unlock(&qu);
			ctr++;
			continue;
		}
		pthread_mutex_unlock(&qu);
		ctr++;
		sem_post(&use);
	}
}

void* consume(void* dummy)
{
	char sentinel;
	printf("%s", "Press <Enter>\n");
	scanf("%c", &sentinel);
	int *temp;
	int pri = 1;
	while(1)
	{
		sem_wait(&use);
		pthread_mutex_lock(&qu);
		if(mq_receive(global, (char*)(&temp), MSG_BUFFER_SIZE, &pri)<0)
		{
			perror("mq_receive()");
			pthread_mutex_unlock(&qu);
			continue;
		}
		pthread_mutex_unlock(&qu);
		printf("%d\n", *temp);
		ret((void*)temp, 4);
		sem_post(&make);
	}
}	

int main()
{
	pthread_mutex_init(&qu, 0);
	int check = init_pool(4);
	sem_init(&make, 0, 19);
	sem_init(&use, 0, 0);
	createQ();
	pthread_create(&producer, 0, produce, NULL);
	pthread_create(&consumer, 0, consume, NULL);
	pthread_join(producer, 0);
	pthread_join(consumer, 0);
}
