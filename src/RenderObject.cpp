#include "RenderObject.h"

RenderDetails InitialiseRenderDetails()
{
RenderDetails rd;   // creating the details
return rd;
}

int getRenderDetailsIDIndex(RenderDetails rd, unsigned int rid)
{
if(rd.rid.size() > rid)  // if the size is bigger than the ID then it is a valid ID
    if(rd.rid[rid] == rid)  // just in case no manipulation of the table has happened
        return rid;

for (int i = 0; i < rd.rid.size(); i++)   // simple linear search
    if(rd.rid[i] == rid)
        return i;
return -1;
}

static void BubbleSortRenderDetails(RenderDetails* rd);

unsigned int AddRenderDetail(RenderDetails& rd, unsigned int vao, unsigned int vbo, unsigned int ibo, unsigned int shader, unsigned int texture)
{
unsigned int id = findNextIDAvailable(rd.rid);  // finding the new ID

SetUniformM4(shader, "view", getM4ID());    // setting the view shader as the identity

// adding all the details to the table
rd.rid.push_back(id);
rd.vao.push_back(vao);
rd.vbo.push_back(vbo);
rd.ibo.push_back(ibo);
rd.shader.push_back(shader);
rd.texture.push_back(texture);

return id;
}

void RemoveRenderDetail(RenderDetails& rd, unsigned int rid)
{
int index = getRenderDetailsIDIndex(rd, rid);  // finding the ID

if(index == -1)
    return; // if the index isn't found just quit

glDeleteVertexArrays(1, &rd.vao[index]);
glDeleteBuffers(1, &rd.vbo[index]);
glDeleteBuffers(1, &rd.ibo[index]);
glDeleteProgram(rd.shader[index]);
glDeleteTextures(1, &rd.texture[index]);

rd.rid.erase(rd.rid.begin() + index);
rd.vao.erase(rd.vao.begin() + index);
rd.vbo.erase(rd.vbo.begin() + index);
rd.ibo.erase(rd.ibo.begin() + index);
rd.shader.erase(rd.shader.begin() + index);
rd.texture.erase(rd.texture.begin() + index);
}

static unsigned int _CreateRenderable(RenderDetails& rd, unsigned int shape, const char* vsfp, const char* fsfp, const char* texfp)
{
unsigned int vao, vbo, ibo, prog, tex = 0;

viBundle vbund = GetShapeVertices(shape);   // the bundle containing the vertices and count
float* vertices = vbund.vi;

viBundle ibund = GetShapeIndices(shape);    // the bundle containing the indices and count
unsigned int* indices = ibund.vi;

vao = CreateVAO();  // creating the vao
ibo = CreateIBO(indices, ibund.n); // creating the ibo
BindIBO(ibo);  // binding the ibo to the vao
vbo = CreateVBO(vertices, vbund.n);  // creating the vbo
BindVBO(vbo);  // binding the vbo to the vao

unsigned int ilay[1] = {3};
VAOLayout layout = CreateVertexLayout(ilay, 5, 1);  // setting up the layout to receive
AddToVertexLayout(layout, 2);  // adding the texture coords to the layout
InitialiseVertexLayout(layout); // initialising the layout to be used

prog = CreateShader(vsfp, fsfp);    // creates the shader object
SetUniformM4(prog, "projection", getProjection(1020, 960, 1));  // setting up the projection

if(texfp != NULL)   // if there is a texture
    {
    tex = CreateTexture(texfp);
    SetUniform1i(prog, "intexture", 0); // set the texture to be used (the 0th active texture)
    }

return AddRenderDetail(rd, vao, vbo, ibo, prog, tex);
}

static unsigned int _CreateSpriteSheetRenderable(RenderDetails& rd, unsigned int nosp, unsigned int spr, const char* vsfp, const char* fsfp, const char* texfp)
{
unsigned int vao, vbo, ibo, prog, tex = 0;

// sorting out the vertices and indices

unsigned int shape = 0;
GeneralInitialise(&shape, nosp, spr, SQUARE);

viBundle vbund = GetShapeVertices(shape);   // the bundle containing the vertices and count
float* vertices = vbund.vi;

viBundle ibund = GetShapeIndices(shape);    // the bundle containing the indices and count
unsigned int* indices = ibund.vi;

// creating the shader
prog = CreateShader(vsfp, fsfp);    // creates the shader object
SetUniformM4(prog, "projection", getProjection(1020, 960, 1));  // setting up the projection

// creating the texture
if(texfp != NULL)   // if there is a texture
    {
    tex = CreateTexture(texfp);
    SetUniform1i(prog, "intexture", 0); // set the texture to be used (the 0th active texture)
    }

vao = CreateVAO();  // creating the vao
ibo = CreateIBO(indices, ibund.n); // creating the ibo
BindIBO(ibo);  // binding the ibo to the vao
vbo = CreateVBO(vertices, vbund.n);  // creating the vbo
BindVBO(vbo);  // binding the vbo to the vao

unsigned int ilay[1] = {3};
VAOLayout layout = CreateVertexLayout(ilay, 5, 1);  // setting up the layout to receive
AddToVertexLayout(layout, 2);  // adding the texture coords to the layout
InitialiseVertexLayout(layout); // initialising the layout to be used

return AddRenderDetail(rd, vao, vbo, ibo, prog, tex);
}

unsigned int CreateSpriteRenderable(RenderDetails& rd, const char* spfp, unsigned int nosp, unsigned int spr)
{
return _CreateSpriteSheetRenderable(rd, nosp, spr, "res/texvert.shader", "res/texfrag.shader", spfp);
}

unsigned int CreateSquareRenderable(RenderDetails& rd)
{
unsigned int shape = 0;
GeneralInitialise(&shape, 1, 1, SQUARE);

return _CreateRenderable(rd, shape, "res/vertex.shader", "res/fragment.shader", NULL);
}

unsigned int CreatePlainSquareRenderable(RenderDetails& rd)
{
unsigned int shape = SQUARE;
GeneralInitialise(&shape, 1, 1, SQUARE);

return _CreateRenderable(rd, shape, "res/vertex.shader", "res/fragment.shader", NULL);
}

void DrawRenderable(RenderDetails rds, unsigned int rid)
{
int index = getRenderDetailsIDIndex(rds, rid);
if(index == -1) return; // if it is not found stop

if(rds.texture[index] != 0)
    {
    BindTexture(rds.texture[index]);
    }

BindShader(rds.shader[index]);
BindVAO(rds.vao[index]);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

unsigned int* getRenderablePrograms(RenderDetails rds, unsigned int* rids, int size)
{
unsigned int* progs = (unsigned int*)malloc(size * sizeof(unsigned int));

for (int i = 0; i < size; i++)
    progs[i] = rds.shader[getRenderDetailsIDIndex(rds, rids[i])];  // getting and setting the programs

return progs;
}

void OutputRenderDetails(RenderDetails rds)
{
printf("\n\n%32s", "Render Details Table");
printf("\n%-10s%-20s%-13s%-13s\t", "ID", "VAO", "Shader", "Texture");
for (int i = 0; i < rds.rid.size(); i++)
    {
    printf("\n%-10d%-20d%-13d%-13d",
        rds.rid[i],
        rds.vao[i],
        rds.shader[i],
        getActiveTexture(rds.texture[i])
        );
    }
}