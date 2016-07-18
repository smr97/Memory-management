#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define POOLSIZE 20

enum interrorcodes{ALLOK = 0, NOTFOUND = 1, MALLOCFAIL = 2, POOLDEAD = 3, MEMSETFAIL = 4};
//enum ptrerrorcodes{BUFFERFULL = NULL, DEADPOOL = NULL};						//See what can be done

pthread_mutex_t locks[POOLSIZE];

typedef struct{
void* ptr;
int flag;
} chunk;

static chunk arr[POOLSIZE];														//static ensures that scope of this variable is limited to this file. Similar to private. Hence extern arr[] will not work in calling functions in separate file.

int init_pool(int blocksize)													//No need for thread-safe as this should be called before forking
{
	int j;
	for(j = 0; j<POOLSIZE; j++)
	{
		pthread_mutex_init(&(locks[j]), 0);	
	}
		
	assert(blocksize>0);
	
	int i;
	
	for(i = 0; i<POOLSIZE; i++)
	{
		arr[i].ptr = (void*) malloc(blocksize);
		if(arr[i].ptr == NULL)
		{
			return MALLOCFAIL;
		}
		arr[i].flag = 0;
	}
	
	return ALLOK;
}

void* alloc()
{
	int i;
	
	for(i = 0; i<POOLSIZE; i++)
	{
		pthread_mutex_lock(&(locks[i]));
			if((arr+i) == NULL)
			{
				return NULL;
			}
		
			if(!arr[i].flag)
			{
				pthread_mutex_unlock(&(locks[i]));
				break;
			}
		pthread_mutex_unlock(&(locks[i]));
	}
	
	if(i==POOLSIZE)
	{
		return NULL;
	}
	
	pthread_mutex_lock(&(locks[i]));
		arr[i].flag = 1;
	pthread_mutex_unlock(&(locks[i]));
	
	return arr[i].ptr;
}

int ret(void* pt, int blocksize)
{
	int i;
	
	for(i = 0; i<POOLSIZE; i++)
	{
	pthread_mutex_lock(&(locks[i]));
		if((arr+i) == NULL)
		{
			pthread_mutex_unlock(&(locks[i]));
			return POOLDEAD;
		}
		
		if(pt==arr[i].ptr)
		{
			arr[i].ptr = memset(arr[i].ptr, 0, blocksize); 
			if((arr+i) == NULL)
			{
				pthread_mutex_unlock(&(locks[i]));
				return MEMSETFAIL;
			}
			arr[i].flag = 0;
			pthread_mutex_unlock(&(locks[i]));
			return ALLOK;
		}
	pthread_mutex_unlock(&(locks[i]));
	}
	
	if(i==POOLSIZE)
	{
		return NOTFOUND;														//See what can be done if this is returned?
	}
}

/*int main()
{
	init_pool(8);
	//int dummy[10000];
	int j = 0;
	int* i[40];
	
	for(; j<20; j++)
	{
		i[j] = (int *) alloc();
		*(i[j]) = 10000*j;
		printf("%d\n", *(i[j]));
	}
	
	for(; j<40; j++)
	{
		ret(i[j-20], 8);
		i[j] = (int *) alloc();
		//*(i[j]) = 2147483647;  
		printf("%d\n", *(i[j]));
	}
	
}*/
