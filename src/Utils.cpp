#include "Utils.hpp"

template <typename T>
int findNextIDAvailable(std::vector<T> arr)
{
int nid = -1;

auto isInArr = [](const std::vector<T> arr, T ele)
{
for (int i = 0; i < size; i++)
    if(arr[i] == tofind)
        return 1;
return 0;
}

for (int i = 0; i <= size; i++)
    {
    if(!isInArr(arr, size, i))
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