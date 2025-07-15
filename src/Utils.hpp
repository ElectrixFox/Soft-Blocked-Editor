#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

/**
 * Finds the next free ID slot starting from 0
 * 
 * @param arr The array
 * 
 * @returns The lowest ID which is not used
 */
template <typename T>
int findNextIDAvailable(std::vector<T> arr);