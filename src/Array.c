#include "Array.h"

#ifdef ARRAY_H

static void ExpandArrayByOne(Array* arr)
{
void* ptr = realloc(arr->data, (arr->size + 1) * sizeof(unsigned int));
if(ptr == NULL)
    {
    printf("\nERROR: The reallocation shat the bed");
    free(ptr);
    exit(1);
    }
else
    {
    arr->data = ptr;
    }
arr->size++;
}

#define Expand(arr, type) \
    { \
    void* ptr = realloc(arr->data, (arr->size + 1) * sizeof(type)); \
    if(ptr == NULL) \
        { \
        printf("\nERROR: The reallocation shat the bed"); \
        free(ptr); \
        exit(1); \
        } \
        arr->data = ptr; \
    arr->size++; \
    if(arr->capacity < arr->size) \
        arr->capacity++; \
    }

#define swap(x, y, type) \
    { \
    type a = *x; \
    *x = *y; \
    *y = a; \
    }

#define removeArrIndex(arr, ind, type) \
    { \
    swap(&arr->data[index], &arr->data[arr->size - 1], unsigned int); \
    arr->size--; \
    }

#define initArr(type, ArrT) \
    ArrT arr; \
    arr.size = 0; \
    arr.data = malloc(sizeof(type)); \
    arr.capacity = 1; \
    return arr;

#define appendArray(arr, item, type) \
    Expand(arr, type); \
    arr->data[arr->size - 1] = item;

static void swapu(unsigned int* x, unsigned int* y) { swap(x, y, unsigned int); }

static int findInArray(Array arr, unsigned int item)
{
for (int i = 0; i < arr.size; i++) // finding the element
    if(arr.data[i] == item)
        return i;
return -1;
}


#pragma region Unsigned Int Array

Array InitialiseArray() { initArr(unsigned int, Array); }
void AppendToArray(Array* arr, unsigned int item) { appendArray(arr, item, unsigned int); }

void RemoveFromArrayIndex(Array* arr, int index) { removeArrIndex(arr, index, unsigned int); }

void RemoveFromArray(Array* arr, unsigned int item)
{
int index = findInArray(*arr, item);

if(index != -1)
    RemoveFromArrayIndex(arr, index);
else
    printf("\nError: Cannot find item %d in the array", item);
}

void OutputArray(Array arr)
{
printf("\nArray of size %d: ", arr.size);
for (int i = 0; i < arr.size; i++)
    printf("%d ", arr.data[i]);
}

#pragma endregion

#pragma region Float Array

static int findInfArray(fArray arr, float item)
{
for (int i = 0; i < arr.size; i++) // finding the element
    if(arr.data[i] == item)
        return i;
return -1;
}

fArray InitialisefArray() { initArr(float, fArray); }
void AppendTofArray(fArray* arr, float item) { appendArray(arr, item, float); }

void RemoveFromfArrayIndex(fArray* arr, int index) { removeArrIndex(arr, index, float); }

void RemoveFromfArray(fArray* arr, float item)
{
int index = findInfArray(*arr, item);

if(index != -1)
    RemoveFromfArrayIndex(arr, index);
else
    printf("\nError: Cannot find item %.2f in the array", item);
}

void OutputfArray(fArray arr)
{
printf("\nArray of size %d: ", arr.size);
for (int i = 0; i < arr.size; i++)
    printf("%.2f ", arr.data[i]);
}

#pragma endregion

#endif