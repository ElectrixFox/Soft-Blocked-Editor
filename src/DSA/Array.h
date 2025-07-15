#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Array
    {
    unsigned int* data;
    unsigned int size;
    } Array;

void AppendToArray(Array* arr, unsigned int item);
