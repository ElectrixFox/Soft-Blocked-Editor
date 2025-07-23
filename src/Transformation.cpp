#include "Transformation.h"

TransformationDetails InitialiseTransformationDetails()
{
TransformationDetails tds;  // creating the details

return tds;
}

int getTransformationIDIndex(TransformationDetails tds, unsigned int trsid)
{
if(tds.trsid.size() > trsid)    // if the size is bigger than the ID then it is a valid ID
    if(tds.trsid[trsid] == trsid)   // just in case no manipulation of the table has happened
        return trsid;

for (int i = 0; i < tds.trsid.size(); i++)  // simple linear search
    if(tds.trsid[i] == trsid)
        return i;
return -1;
}

unsigned int AddTransformation(TransformationDetails& tds, vec2 pos, vec2 scale, float theta)
{
unsigned int id = findNextIDAvailable(tds.trsid);

// setting all the new details
tds.trsid.push_back(id);
tds.pos.push_back(pos);
tds.scale.push_back(scale);
tds.angle.push_back(theta);

return id;
}

void RemoveTransformation(TransformationDetails& tds, unsigned int tid)
{
int index = getTransformationIDIndex(tds, tid); // finding the ID

if(index == -1) return; // if the index isn't found just quit

tds.trsid.erase(tds.trsid.begin() + index);
tds.pos.erase(tds.pos.begin() + index);
tds.scale.erase(tds.scale.begin() + index);
tds.angle.erase(tds.angle.begin() + index);
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
const int n = tds.trsid.size(); // for ease of use
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
for (int i = 0; i < trds.trsid.size(); i++)
    if(CheckPressed(trds, trds.trsid[i], curpos)) // checking if anything has been pressed
        total++;

return (total == n) ? 1 : 0;
}

int PressedNothing(TransformationDetails trds, vec2 curpos) { return hasPressedN(trds, curpos, 0); }

int PressedAnother(TransformationDetails trds, vec2 curpos) { return hasPressedN(trds, curpos, 1); }

int PressedArea(TransformationDetails trds, vec2 curpos, float range)
{
for (int i = 0; i < trds.trsid.size(); i++)
    if(abs(trds.pos[i].x - curpos.x) < range && abs(trds.pos[i].y - curpos.y) < range)
        return 1; // if the current transformation has a horizontal and vertical distance is less than the range then it is in the square area

return 0;
}

unsigned int getPressedBlock(TransformationDetails trds, vec2 curpos)
{
for (int i = 0; i < trds.trsid.size(); i++)
    if(CheckPressed(trds, trds.trsid[i], curpos)) // checking if anything has been pressed
        return trds.trsid[i];
return 0;
}


int getTransformAt(TransformationDetails tds, vec2 pos)
{
for (int i = 0; i < tds.trsid.size(); i++)  // loop through all of the transforms
    if(tds.pos[i].x == pos.x && tds.pos[i].y == pos.y)  // if they are the same vector
        return tds.trsid[i];
return -1;
}

void OutputTransformations(TransformationDetails tds)
{
printf("\n\n%32s", "Transformations Table");
printf("\n%-10s%-20s%-13s\t", "ID", "Position", "Scale");
for (int i = 0; i < tds.trsid.size(); i++)
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