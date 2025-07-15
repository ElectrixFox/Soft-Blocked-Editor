#include "Array.h"

void AppendToArray(Array* arr, unsigned int item)
{
void* nptr = malloc(sizeof(unsigned int*) * (*arr).size); // allocating the new memory
nptr = realloc((*arr).data, sizeof(unsigned int) * ((*arr).size + 1));  // making the array bigger
(*arr).data = nptr; // setting the new array

(*arr).data[(*arr).size] = item;    // adding the new item to the array
}