#pragma once
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
int findNextIDAvailable(std::vector<T> arr)
{
int nid = -1;

auto isInArr = [](const std::vector<T> arr, T ele)
{
for (int i = 0; i < arr.size(); i++)
    if(arr[i] == ele)
        return 1;
return 0;
};

for (int i = 0; i <= arr.size(); i++)
    {
    if(!isInArr(arr, i))
        {
        nid = i;
        break;
        }
    }

if(nid == -1)
    {
    printf("Error: Cannot find a new ID");
    exit(1);
    }
return nid;
}
