#ifndef mempool.h
#define mempool.h

enum interrorcodes{ALLOK = 0, NOTFOUND = 1, MALLOCFAIL = 2, POOLDEAD = 3, MEMSETFAIL = 4};
//enum ptrerrorcodes{BUFFERFULL = NULL, DEADPOOL = NULL};

typedef struct{
int* ptr;
int flag;
} chunk;

int init_pool(int blocksize);

void* alloc();

int ret(void* pt, int blocksize);

#endif
