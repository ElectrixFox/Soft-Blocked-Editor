#include "Queue.h"

Queue InitQueue(unsigned int size)
{
Queue q;
q.head = -1;
q.tail = -1;
q.size = size;
q.data = (void*)malloc(size * sizeof(unsigned int));

return q;
}

int isEmpty(Queue queue)
{
if(queue.head == -1)
    return 1;
return 0;
}

int isFull(Queue queue)
{
if(queue.tail == queue.size - 1)
    return 1;
return 0;
}

void Enqueue(Queue* q, unsigned int item)
{
if (isFull(*q))
    return 0;
if (isEmpty(*q))
    q->head = 0;
q->tail++;
q->data[q->tail] = item;
}

unsigned int Dequeue(Queue* q)
{
if(!isEmpty(*q))
    return 0;
q->head++;
if(q->head > q->tail)
    q->head = q->tail = -1;
return q->data[q->head];
}

unsigned int Peek(Queue q)
{
if(!isEmpty(q))
    return NULL;
return q.data[q.head];
}