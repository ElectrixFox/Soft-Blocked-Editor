#include "Utils.hpp"

int findNextIDAvailable<unsigned int>(std::vector<unsigned int> arr)
{
int nid = -1;

auto isInArr = [](const std::vector<unsigned int> arr, unsigned int ele)
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