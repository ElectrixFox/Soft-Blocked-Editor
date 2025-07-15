#ifndef ARRAY_H
#define ARRAY_H
#include "MathsOperations.h"

typedef struct Array
    {
    size_t size;
    size_t capacity;
    unsigned int* data;
    } Array;

Array InitialiseArray();

void AppendToArray(Array* arr, unsigned int item);

void RemoveFromArrayIndex(Array* arr, int index);

void RemoveFromArray(Array* arr, unsigned int item);

void OutputArray(Array arr);

typedef struct
    {
    size_t size;
    size_t capacity;
    float* data;
    } fArray;

fArray InitialisefArray();

void AppendTofArray(fArray* arr, float item);

void RemoveFromfArrayIndex(fArray* arr, int index);

void RemoveFromfArray(fArray* arr, float item);

void OutputfArray(fArray arr);

#endif
