#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Queue
    {
    int head, tail;
    unsigned int* data;
    unsigned int size;
    } Queue;

Queue InitQueue(unsigned int size);

int isEmpty(Queue queue);
int isFull(Queue queue);

void Enqueue(Queue* q, unsigned int item);
unsigned int Dequeue(Queue* q);
unsigned int Peek(Queue q);
