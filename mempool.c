#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define POOLSIZE 20
#define OFFSET 2

enum int_errorcodes{ALLOK = 0, NOTFOUND = 1, MALLOCFAIL = 2, POOLDEAD = 3, MEMSETFAIL = 4};
pthread_mutex_t lock;
static void* arr[POOLSIZE];					//static ensures that scope of this variable is limited to this file. Similar to private. Hence extern arr[] will not work in calling functions in separate file.

int init_pool(int blocksize)
{
	pthread_mutex_init(&(lock), 0);		
	assert(blocksize>0);
	int i;
	for(i = 0; i<POOLSIZE; i++)
	{
		arr[i] = (void*) malloc(blocksize+OFFSET);
		if(arr[i] == NULL)
		{
			int j;
			for(j = 0; j<=i; j++)
			{
				free(arr[j]);
			}
			return MALLOCFAIL;
		}
		*((char*) arr[i]) = '0';
	}
	return ALLOK;
}

void* alloc()
{
	int i;
	for(i = 0; i<POOLSIZE; i++)
	{
		pthread_mutex_lock(&(lock));
			if(*((char*)arr[i]) == '0')
			{
				break;
			}
		pthread_mutex_unlock(&(lock));
	}
	if(i==POOLSIZE)
	{
		pthread_mutex_unlock(&lock);
		return NULL;
	}
	*((char* )arr[i]) = '1';
	pthread_mutex_unlock(&(lock));
	return arr[i]+OFFSET;
}

int ret(void* pt, int blocksize)
{
	pt = memset(pt, 0, blocksize);
	*((char*)(pt-OFFSET)) = '0';
}

/*int main()
{
	init_pool(8);
	//int dummy[10000];
	int j = 1;
	int* i[40];
	i[0] = (int *) alloc();
	*(i[0]) = 0;
	for(; j<20; j++)
	{
		i[j] = (int *) alloc();
		*(i[j]) = 10000*j;
		printf("%d %d\n", *(i[j]), *(i[j-1]));
	}
	for(; j<40; j++)
	{
		ret(i[j-20], 8);
		i[j] = (int *) alloc();
		*(i[j]) = 2147483647;  
		printf("%d\n", *(i[j]));
	}
}*/
