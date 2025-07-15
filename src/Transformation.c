#include "Transformation.h"

TransformationDetails InitialiseTransformationDetails()
{
TransformationDetails tds;  // creating the details

tds.size = 0;  // setting the size to 0

// allocating a small bit of memory
tds.trsid = malloc(1024 * sizeof(unsigned int));
tds.pos = malloc(1024 * sizeof(vec2));
tds.scale = malloc(1024 * sizeof(vec2));
tds.angle = malloc(1024 * sizeof(float));

return tds;
}

int getTransformationIDIndex(TransformationDetails tds, unsigned int trsid)
{
if(tds.size > trsid)    // if the size is bigger than the ID then it is a valid ID
    if(tds.trsid[trsid] == trsid)   // just in case no manipulation of the table has happened
        return trsid;

for (int i = 0; i < tds.size; i++)  // simple linear search
    if(tds.trsid[i] == trsid)
        return i;
return -1;
}

static void increaseTransformations(TransformationDetails* tds)
{
const unsigned int n = tds->size;

/*
ExpandByOne(&tds->trsid, n, sizeof(unsigned int));
ExpandByOne(&tds->pos, n, sizeof(vec2));
ExpandByOne(&tds->scale, n, sizeof(vec2));
ExpandByOne(&tds->angle, n, sizeof(float));
*/

tds->size++;
}

unsigned int AddTransformation(TransformationDetails* tds, vec2 pos, vec2 scale, float theta)
{
// static unsigned int id = 0; // a static incrementing counter to set the new ID as
const unsigned int n = tds->size;
unsigned int id = findNextIDAvailable(tds->trsid, tds->size);

// make all the arrays bigger by one to accomodate for the new element
/*
ExpandByOne(&tds->trsid, n, sizeof(unsigned int));
ExpandByOne(&tds->pos, n, sizeof(vec2));
ExpandByOne(&tds->scale, n, sizeof(vec2));
ExpandByOne(&tds->angle, n, sizeof(float));
*/

increaseTransformations(tds);


// setting all the new details
tds->trsid[n] = id;
tds->pos[n] = pos;
tds->scale[n] = scale;
tds->angle[n] = theta;

// tds->size++;    // increase the number of transforms

return tds->trsid[n];
}

void RemoveTransformation(TransformationDetails *tds, unsigned int tid)
{
int index = getTransformationIDIndex(*tds, tid); // finding the ID
const unsigned int n = tds->size;

if(index == -1)
    return; // if the index isn't found just quit

if(index == tds->size - 1) goto end;   // hehe the naughty goto

// getting temporary stuff
unsigned int tmpid = tds->trsid[index];
vec2 tpos = tds->pos[index];
vec2 tscale = tds->scale[index];
float tangle = tds->angle[index];

// setting the to delete to the end values
tds->trsid[index] = tds->trsid[tds->size - 1];
tds->pos[index] = tds->pos[tds->size - 1];
tds->scale[index] = tds->scale[tds->size - 1];
tds->angle[index] = tds->angle[tds->size - 1];

// setting the end to the thing to delete
tds->trsid[tds->size - 1] = tmpid;
tds->pos[tds->size - 1] = tpos;
tds->scale[tds->size - 1] = tscale;
tds->angle[tds->size - 1] = tangle;

ShrinkArrayByOne(&tds->trsid, n, sizeof(unsigned int));
ShrinkArrayByOne(&tds->pos, n, sizeof(vec2));
ShrinkArrayByOne(&tds->scale, n, sizeof(vec2));
ShrinkArrayByOne(&tds->angle, n, sizeof(float));

end:
tds->size--;    // decrease the size so it is effectively not there

// To-Do: Could add in a sort here to sort by ID in order to realign the table
}

void setPosition(TransformationDetails tds, unsigned int trsid, vec2 newpos)
{
int index = getTransformationIDIndex(tds, trsid);   // find the ID
if(index == -1) return; // if not found return
tds.pos[index] = newpos;    // setting the new position
}

vec2 getPosition(TransformationDetails tds, unsigned int trsid) { return tds.pos[getTransformationIDIndex(tds, trsid)]; }

void applyTranslation(TransformationDetails tds, unsigned int trsid, vec2 trans) { setPosition(tds, trsid, addVec2(getPosition(tds, trsid), trans)); }

void setScale(TransformationDetails tds, unsigned int trsid, vec2 newscale)
{
int index = getTransformationIDIndex(tds, trsid);   // find the ID
if(index == -1) return; // if not found return
tds.scale[index] = newscale;    // setting the new scale
}

vec2 getScale(TransformationDetails tds, unsigned int trsid) { return tds.scale[getTransformationIDIndex(tds, trsid)]; }

void setRotation(TransformationDetails tds, unsigned int trsid, float theta)
{
int index = getTransformationIDIndex(tds, trsid);   // find the ID
if(index == -1) return; // if not found return
tds.angle[index] = theta;   // setting the new angle
}

m4 getTransformModelMatrix(TransformationDetails tds, unsigned int tid)
{
int index = getTransformationIDIndex(tds, tid); // find the transformation ID
if(index != -1)
    return GetModelMatrix(tds.pos[index], tds.scale[index], tds.angle[index]);    // get the model matrix

return getM4ID();   // return the identity so nothing bad goes on
}

void getTransformModelMatrices(TransformationDetails tds, m4* models)
{
const int n = tds.size; // for ease of use
models = (m4*)malloc(n * sizeof(m4));   // allocating the memory to be used

for (int i = 0; i < n; i++)
    models[i] = GetModelMatrix(tds.pos[i], tds.scale[i], tds.angle[i]);   // doing the calculation and setting the value
}

int CheckPressed(TransformationDetails tds, unsigned int tid, vec2 cursorpos)
{
unsigned int index = getTransformationIDIndex(tds, tid); // some hash function to get where the eid is in the big array

if(PointInSquare(cursorpos, tds.pos[index], tds.scale[index]))
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

unsigned int getPressedBlock(TransformationDetails trds, vec2 curpos)
{
for (int i = 0; i < trds.size; i++)
    if(CheckPressed(trds, trds.trsid[i], curpos)) // checking if anything has been pressed
        return trds.trsid[i];
return 0;
}


int getTransformAt(TransformationDetails tds, vec2 pos)
{
for (int i = 0; i < tds.size; i++)  // loop through all of the transforms
    if(tds.pos[i].x == pos.x && tds.pos[i].y == pos.y)  // if they are the same vector
        return tds.trsid[i];
return -1;
}

void OutputTransformations(TransformationDetails tds)
{
printf("\n\n%32s", "Transformations Table");
printf("\n%-10s%-20s%-13s\t", "ID", "Position", "Scale");
for (int i = 0; i < tds.size; i++)
    {
    printf("\n%-10d(%07.2f, %07.2f)%-2s(%05.2f, %05.2f)\t",
        tds.trsid[i],
        tds.pos[i].x,
        tds.pos[i].y,
        "",
        tds.scale[i].x,
        tds.scale[i].y);
    }
}