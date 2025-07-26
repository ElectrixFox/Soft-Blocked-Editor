#include "Drawable.h"

Drawables InitialiseDrawables()
{
Drawables drabs;
return drabs;
}

int AddDrawable(Drawables& drabs, const unsigned int trid, const unsigned int rid)
{
// setting all the new details
drabs.rids.push_back(rid);
drabs.trsids.push_back(trid);

return drabs.rids.size();
}

int findDrawablesTransform(const Drawables& drabs, unsigned int trid)
{
for (int i = 0; i < drabs.rids.size(); i++)
    if (drabs.trsids[i] == trid) // if the ID matches
        return i; // return the index
return -1; // if the ID isn't found return -1
}

int findDrawablesRenderable(const Drawables& drabs, unsigned int rid)
{
for (int i = 0; i < drabs.rids.size(); i++)
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

void UnassignDrawable(Drawables& drabs, unsigned int trid)
{
int index = findDrawablesTransform(drabs, trid); // finding the ID

if(index == -1)
    return; // if the index isn't found just quit

drabs.rids.erase(drabs.rids.begin() + index);
drabs.trsids.erase(drabs.trsids.begin() + index);
}

void RemoveDrawable(Drawables& drabs, RenderDetails& rds, TransformationDetails& tds, unsigned int trid)
{
int index = findDrawablesTransform(drabs, trid); // finding the ID

if(index == -1)
    return; // if the index isn't found just quit

RemoveRenderDetail(rds, drabs.rids[index]); // remove the render detail from the render details object
RemoveTransformation(tds, drabs.trsids[index]); // remove the transformation from the transformation details object

drabs.rids.erase(drabs.rids.begin() + index);
drabs.trsids.erase(drabs.trsids.begin() + index);
}

void DrawDrawables(const RenderDetails rds, const TransformationDetails tds, const Drawables drabs)
{
// getting all we will need from the transformation objects first
std::vector<m4> models;

for(unsigned int trid : drabs.trsids)
    models.push_back(getTransformModelMatrix(tds, trid));

for (int i = 0; i < drabs.rids.size(); i++)
    {
    const unsigned int prog = rds.shader[getRenderDetailsIDIndex(rds, drabs.rids[i])];  // may as well make this a constant here for efficiency
    SetUniformM4(prog, "model", models[i]); // setting the model matrix
    DrawRenderable(rds, drabs.rids[i]); // finally do the actual drawing
    }
}

void OutputDrawables(Drawables drabs)
{
printf("\n\n%20s", "Drawables Table");
printf("\n%-14s%-14s", "Transform ID", "Render ID");
for (int i = 0; i < drabs.rids.size(); i++)
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

int CreateBasicSquare(RenderPacket& rp, vec2 position, float scale, vec4 incol)
{
vec2 scle = {scale, scale};

unsigned int trsid = AddTransformation(rp.tds, position, scle, 0.0f);   // creates the transform
unsigned int rid = CreatePlainSquareRenderable(rp.rds);   // creates the render element
unsigned int prog = rp.rds.shader[getRenderDetailsIDIndex(rp.rds, rid)];  // finds the program

SetUniform4f(prog, "colour", incol);  // setting the colour

int index = AddDrawable(rp.drabs, trsid, rid);    // adds the drawable

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