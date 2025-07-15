#include "Drawable.h"

Drawables InitialiseDrawables()
{
Drawables drabs;
drabs.size = 0;

drabs.trsids = malloc(1024 * sizeof(unsigned int));
drabs.rids = malloc(1024 * sizeof(unsigned int));

return drabs;
}

int AddDrawable(Drawables* drabs, unsigned int trid, unsigned int rid)
{
const unsigned int n = drabs->size;

// make all the arrays bigger by one to accomodate for the new element
/*
ExpandByOne(&drabs->rids, n, sizeof(unsigned int));
ExpandByOne(&drabs->trsids, n, sizeof(unsigned int));
*/

// setting all the new details
drabs->rids[n] = rid;
drabs->trsids[n] = trid;

drabs->size++;    // increase the number of drawables

return n;
}

int findDrawablesTransform(Drawables drabs, unsigned int trid)
{
for (int i = 0; i < drabs.size; i++)
    if (drabs.trsids[i] == trid) // if the ID matches
        return i; // return the index
return -1; // if the ID isn't found return -1
}

int findDrawablesRenderable(Drawables drabs, unsigned int rid)
{
for (int i = 0; i < drabs.size; i++)
    if (drabs.rids[i] == rid) // if the ID matches
        return i; // return the index
return -1; // if the ID isn't found return -1
}

unsigned int* getRenderIDsFromTransformIDs(Drawables drabs, unsigned int* trids, unsigned int size)
{
unsigned int* rids = (unsigned int*)malloc(size * sizeof(unsigned int));

for (int i = 0; i < size; i++)
    rids[i] = drabs.rids[findDrawablesTransform(drabs, trids[i])];  // getting and setting the drawable IDs

return rids;
}

void RemoveDrawable(Drawables* drabs, RenderDetails* rds, TransformationDetails* tds, unsigned int trid)
{
int index = findDrawablesTransform(*drabs, trid); // finding the ID

if(index == -1)
    return; // if the index isn't found just quit

RemoveRenderDetail(rds, drabs->rids[index]); // remove the render detail from the render details object
RemoveTransformation(tds, drabs->trsids[index]); // remove the transformation from the transformation details object

if(index == drabs->size - 1) goto end;   // hehe the naughty goto

// getting temporary stuff
unsigned int tmp_trsid = drabs->trsids[index];
unsigned int tmp_rid = drabs->rids[index];

// setting the to delete to the end values
drabs->trsids[index] = drabs->trsids[drabs->size - 1];
drabs->rids[index] = drabs->rids[drabs->size - 1];

// setting the end to the thing to delete
drabs->trsids[drabs->size - 1] = tmp_trsid;
drabs->rids[drabs->size - 1] = tmp_rid;

ShrinkArrayByOne(&drabs->rids, drabs->size, sizeof(unsigned int));
ShrinkArrayByOne(&drabs->trsids, drabs->size, sizeof(unsigned int));

end:
drabs->size--;    // decrease the size so it is effectively not there
}

void DrawDrawables(const RenderDetails rds, const TransformationDetails tds, const Drawables drabs)
{
// getting all we will need from the transformation objects first
m4* models = (m4*)malloc(drabs.size * sizeof(m4));  // getting all of the transformation matrices

for (int i = 0; i < drabs.size; i++)    // getting  the model matrices
    models[i] = getTransformModelMatrix(tds, drabs.trsids[i]);

for (int i = 0; i < drabs.size; i++)
    {
    const unsigned int prog = rds.shader[getRenderDetailsIDIndex(rds, drabs.rids[i])];  // may as well make this a constant here for efficiency
    SetUniformM4(prog, "model", models[i]); // setting the model matrix
    DrawRenderable(rds, drabs.rids[i]); // finally do the actual drawing
    }
}

void OutputDrawables(Drawables drabs)
{
printf("\nSize: %d", drabs.size);
printf("\n\n%20s", "Drawables Table");
printf("\n%-14s%-14s", "Transform ID", "Render ID");
for (int i = 0; i < drabs.size; i++)
    {
    printf("\n%-14d%-14d",
        drabs.trsids[i],
        drabs.rids[i]);
    }
}

RenderPacket InitialiseRenderPacket()
{
RenderPacket rp;
rp.tds = InitialiseTransformationDetails();
rp.rds = InitialiseRenderDetails();
rp.drabs = InitialiseDrawables();

return rp;
}

int CreateBasicSquare(RenderPacket* rp, vec2 position, float scale, vec4 incol)
{
vec2 scle = (vec2){scale, scale};

unsigned int trsid = AddTransformation(&rp->tds, position, scle, 0.0f);   // creates the transform
unsigned int rid = CreatePlainSquareRenderable(&rp->rds);   // creates the render element
unsigned int prog = rp->rds.shader[getRenderDetailsIDIndex(rp->rds, rid)];  // finds the program

if(incol == NULL)   // if there is no colour set a default one
    SetUniform4f(prog, "colour", (vec4){1.0f, 0.0f, 0.0f, 1.0f});  // setting the colour
else
    SetUniform4f(prog, "colour", incol);  // setting the colour

int index = AddDrawable(&rp->drabs, trsid, rid);    // adds the drawable

return index;
}

void DrawRenderPacket(const RenderPacket rp) { DrawDrawables(rp.rds, rp.tds, rp.drabs); }

void OutputRenderPacketDetails(RenderPacket rp)
{
printf("\n\nOutputting Render Packet");
OutputTransformations(rp.tds);
OutputRenderDetails(rp.rds);
OutputDrawables(rp.drabs);
printf("\n\n");
}