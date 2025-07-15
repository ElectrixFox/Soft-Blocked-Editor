#include "PressableObject.h"

PressableDetails InitialisePressableDetails()
{
PressableDetails prds;

prds.size = 0;

prds.prid = (unsigned int*)malloc(sizeof(unsigned int));
prds.trsid = (unsigned int*)malloc(sizeof(unsigned int));
prds.pract = (unsigned int*)malloc(sizeof(unsigned int));

return prds;
}

int getPressableIDIndex(PressableDetails prds, unsigned int prid)
{
if (prds.size > prid)
    if (prds.prid[prid] == prid)
        return prid;

for (int i = 0; i < prds.size; i++)
    if (prds.prid[i] == prid)
        return i;
return -1;
}

int findPressableTransfom(PressableDetails prds, unsigned int trid)
{
if (prds.size > trid)
    if (prds.trsid[trid] == trid)
        return trid;

for (int i = 0; i < prds.size; i++)
    if (prds.trsid[i] == trid)
        return i;
return -1;
}

unsigned int getPressableAction(PressableDetails prds, unsigned int prid)
{
return (unsigned int)(prds.pract[getPressableIDIndex(prds, prid)]);
}

unsigned int AddPressable(PressableDetails* prds, unsigned int tid, unsigned int pract)
{
static unsigned int id = 0;
const unsigned int n = prds->size;

ExpandByOne(&prds->prid, n, sizeof(unsigned int));
ExpandByOne(&prds->trsid, n, sizeof(unsigned int));
ExpandByOne(&prds->pract, n, sizeof(unsigned int));

prds->prid[n] = id++;
prds->trsid[n] = tid;
prds->pract[n] = pract;

prds->size++;

return prds->prid[n];
}

void RemovePressable(PressableDetails* prds, unsigned int prid)
{
int index = getPressableIDIndex(*prds, prid);

if (index == -1)
    return;

if (index == prds->size - 1) goto end;

unsigned int tmpid = prds->prid[index];
unsigned int ttrid = prds->trsid[index];
unsigned int tpact = prds->pract[index];

prds->prid[index] = prds->prid[prds->size - 1];
prds->trsid[index] = prds->trsid[prds->size - 1];
prds->pract[index] = prds->pract[prds->size - 1];

prds->prid[prds->size - 1] = tmpid;
prds->trsid[prds->size - 1] = ttrid;
prds->pract[prds->size - 1] = tpact;

end:
prds->size--;
}

unsigned int getActionCount(PressableDetails prds, unsigned int pract)
{
unsigned int count = 0;
for (int i = 0; i < prds.size; i++)
    if(prds.pract[i] == pract)
        count++;
return count;
}

unsigned int* getPressablesWithAction(PressableDetails prds, unsigned int pract)
{
unsigned int* prids = (unsigned int*)malloc(2 * sizeof(unsigned int));
int top = 1;

for (int i = 0; i < prds.size; i++)
    {
    if(prds.pract[i] == pract)
        {
        prids[top] = prds.prid[i];
        ExpandByOne(&prids, top, sizeof(unsigned int));  // increase the size of the array by one to be able to use it
        top++;
        }
    }

prids[0] = top - 1; // assigning the first element to contain the size
return prids;
}

unsigned int* getPressablesTransformWithAction(PressableDetails prds, unsigned int pract)
{
// unsigned int* trids = (unsigned int*)malloc(prds.size * sizeof(unsigned int));
unsigned int* trids = (unsigned int*)malloc((1 + getActionCount(prds, pract)) * sizeof(unsigned int));
int top = 1;

for (int i = 0; i < prds.size; i++)
    {
    if(prds.pract[i] == pract)
        {
        trids[top] = prds.trsid[i];
        top++;
        }
    }

trids[0] = top - 1; // assigning the first element to contain the size
return trids;
}

unsigned int* getPressablesTransformWithoutAction(PressableDetails prds, unsigned int pract)
{
unsigned int* trids = (unsigned int*)malloc((1 + (prds.size - getActionCount(prds, pract))) * sizeof(unsigned int));
int top = 1;

for (int i = 0; i < prds.size; i++)
    {
    if(prds.pract[i] != pract)
        {
        trids[top] = prds.trsid[i];
        top++;
        }
    }

trids[0] = top - 1; // assigning the first element to contain the size
return trids;
}

int _CheckPressed(vec2* poses, vec2* scales, vec2 cursorpos, unsigned int eid)
{
unsigned int index = eid; // some hash function to get where the eid is in the big array

if(PointInSquare(cursorpos, poses[index], scales[index]))
    return 1;
return 0;
}

static int hasPressedN(TransformationDetails trds, vec2 curpos, unsigned int n)
{
int total = 0;  // total things pressed
for (int i = 0; i < trds.size; i++)
    if(CheckPressed(trds, trds.trsid[i], curpos)) // checking if anything has been pressed
        total++;

return (total == n) ? 1 : 0;
}

int PressedNothing(TransformationDetails trds, vec2 curpos) { return hasPressedN(trds, curpos, 0); }

int PressedAnother(TransformationDetails trds, vec2 curpos) { return hasPressedN(trds, curpos, 1); }

int PressedArea(TransformationDetails trds, vec2 curpos, float range)
{
for (int i = 0; i < trds.size; i++)
    if(abs(trds.pos[i].x - curpos.x) < range && abs(trds.pos[i].y - curpos.y) < range)
        return 1; // if the current transformation has a horizontal and vertical distance is less than the range then it is in the square area

return 0;
}

int PressedAreaAction(PressableDetails prds, TransformationDetails trds, vec2 curpos, float range, unsigned int pract)
{
for (int i = 0; i < trds.size; i++)
    if(abs(trds.pos[i].x - curpos.x) < range && abs(trds.pos[i].y - curpos.y) < range)
        if(prds.pract[findPressableTransfom(prds, trds.trsid[i])] == pract)
            return 1; // if the current transformation has a horizontal and vertical distance is less than the range then it is in the square area

return 0;
}

unsigned int getPressedBlock(TransformationDetails trds, vec2 curpos)
{
for (int i = 0; i < trds.size; i++)
    if(CheckPressed(trds, trds.trsid[i], curpos)) // checking if anything has been pressed
        return trds.trsid[i];
return 0;
}

unsigned int* getPressedBlocksArea(PressableDetails prds, TransformationDetails trds, vec2 curpos, float range)
{
int* indices = getPressedArea(trds.pos, trds.size, range);
int size = indices[0];
indices = &indices[1];  // shuffling along the array

unsigned int* pids = malloc((size + 1) * sizeof(unsigned int));
pids[0] = size;
for (int i = 1; i < size; i++)
    pids[i] = findPressableTransfom(prds, trds.trsid[indices[i]]);

return pids;


/*
unsigned int* pids = malloc(2 * sizeof(unsigned int));
int top = 1;

for (int i = 0; i < trds.size; i++)
    if(abs(trds.pos[i].x - curpos.x) < range && abs(trds.pos[i].y - curpos.y) < range)
        {
        pids[top] = prds.prid[findPressableTransfom(prds, trds.trsid[i])];  // setting the ID
        ExpandByOne(&pids, top, sizeof(unsigned int));  // increasing the size of the array
        top++;
        }

pids[0] = top - 1;

return pids;
*/
}

unsigned int getPressedBlockAction(PressableDetails prds, TransformationDetails trds, vec2 curpos)
{
return prds.pract[getPressableIDIndex(prds, getPressedBlock(trds, curpos))];
}

void SetPressableAction(PressableDetails* prds, unsigned int prid, unsigned int pract)
{
int index = getPressableIDIndex(*prds, prid);   // finding the object
prds->pract[index] = pract; // setting the action
}

void OutputPressables(PressableDetails prds)
{
printf("\nSize: %d", prds.size);
printf("\n\n%30s", "Pressables Table");
printf("\n%-15s%-15s%-15s", "ID", "Action", "Transform ID");
for (int i = 0; i < prds.size; i++)
    {
    printf("\n%-15d%-15d%-15d", prds.prid[i], prds.pract[i], prds.trsid[i]);
    }
}