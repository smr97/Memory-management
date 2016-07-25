# Memory-management

The conventional malloc() function in C is both processor intensive and reduces determinism in case of Real Time embedded systems. Hence this implementation of a static sized memory pool will be further modelled into an API for the same.

# Threadpool

This code aims at implementing an asynchronous producer-multi consumer model. The one producer allocates memory from a custom made, threadsafe pool (mempool.c) then stores a random integer in it (<100). This is pushed into a global message queue. 
<br />

The 5 worker threads asynchronously access the queue, popping the pointer references and getting the random integers. They then use locks to read, compute and update a global struct resource that stores the average, min, max, and sample size. The pointer to the data is then freed by the worker thread so that the pool does not overflow. All pool operations are done asynchronously as well.

# Producer-consumer
This is an implementation of asynchronous producer-single consumer model with producer sending integers to consumer and the consumer printing and freeing the pointer that stored the data. All heap memory is managed by the mempool.c and only the pool is accessible to the threads.

# Instructions for testing
Download zip <br />
Compile producer-consumer.c: gcc -pthread producer-consumer.c mempool.c -o producer-consumer <br />
Execute. Read the code to understand how the memory is being allocated.<br />
#### Make sure that extraction of all files is done to a single destination.
