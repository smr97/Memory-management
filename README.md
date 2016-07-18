# Memory-management

The conventional malloc() function in C is both processor intensive and reduces determinism in case of Real Time embedded systems. Hence this implementation of a static sized memory pool will be further modelled into an API for the same.

# Instructions for testing
Download zip <br />
Compile producer-consumer.c: gcc -pthread producer-consumer.c mempool.c -o producer-consumer <br />
Execute. Read the code to understand how the memory is being allocated.<br />
#### Make sure that extraction of all files is done to a single destination.
