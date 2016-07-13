#include <stdlib.h>
#include <stdio.h>
#define POOLSIZE 20

typedef struct{
int* ptr;
int flag;
} chunk;

chunk* init_pool(int blocksize)
{
	static chunk arr[POOLSIZE];
	
	int i = 0;
	
	for(; i<POOLSIZE; i++)
	{
		arr[i].ptr = (int*) malloc(blocksize);
		arr[i].flag = 0;
	}
	
	return arr;
}

int* alloc(chunk* pool)
{
	int i = 0;
	
	for(; i<POOLSIZE; i++)
	{
		if(!pool[i].flag)
		{
			break;
		}
	}
	
	pool[i].flag = 1;
	
	return pool[i].ptr;
}

void ret(int* pt, chunk* ch, int blocksize)
{
	int i = 0;
	
	for(; i<POOLSIZE; i++)
	{
		if(pt==ch[i].ptr)
		{
			free(ch[i].ptr);
			ch[i].ptr = (int *) malloc(blocksize);
			ch[i].flag = 0;
			break;
		}
	}
}

int main()
{
	chunk *c = init_pool(8);
	int dummy[10000];
	int j = 0;
	int* i[40];
	
	for(; j<20; j++)
	{
		i[j] = alloc(c);
		*(i[j]) = 10000*j;
		printf("%d\n", *(i[j]));
	}
	
	for(; j<40; j++)
	{
		ret(i[j-20], c, 8);
		i[j] = alloc(c);
		*(i[j]) = 2147483647;
		printf("%d\n", *(i[j]));
	}
	
}

