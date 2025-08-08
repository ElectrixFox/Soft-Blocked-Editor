#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <thread>

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#include "src/InputManager.h"
#include "src/Renderer.hpp"
#include "src/Camera.hpp"
#include "src/Entity.hpp"
#include "src/Level.hpp"
#include "src/Editor.hpp"
#include "src/Text.hpp"

extern const int snap_to_grid;
extern const int grid_size;

int gwid = 1280, ghig = 720;

InputManager inpman;

void on_window_resize(GLFWwindow* window, int width, int height)
{
gwid = width;
ghig = height;
printf("\n%dx%d", gwid, ghig);
}

int main(int argc, char const *argv[])
{
unsigned int width = gwid;
unsigned int height = ghig;

setbuf(stdout, NULL);   // MUST REMOVE!!!

glfwInit();
// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // change to borederless
GLFWwindow* window = glfwCreateWindow(width, height, "Title", 0, 0); // creates the window of size width x height
glfwSetWindowAspectRatio(window, 16, 9);
glViewport(0, 0, gwid, ghig);

glfwMakeContextCurrent(window); // sets the context of the window

glfwSetWindowSizeCallback(window, on_window_resize);
glfwSetKeyCallback(window, updateInput);
glfwSetMouseButtonCallback(window, updateMouse);
glfwSetCursorPosCallback(window, updateCurpos);

glewInit();
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

InitialiseInputManager(window);

Camera cam = CreateCamera({0, 0}, {(float)gwid, (float)ghig}, &gwid, &ghig);

Block_Manager blk_man;
Editor ed;

int w, h;
int** grid;
ReadLevel("res/levels/level3.txt", &w, &h, &grid);
OutputLevel((const int**)grid, w, h);
DrawLevel(blk_man, w, h, (const int**)grid);
getLevel(blk_man, &w, &h, &grid);

const vec2 tpos = blk_man.blocks[0].pos;
vec2 opos = getLevelGridCoordinates(blk_man, w, h, (const int**)grid, tpos);
printf("\nTop left: ");
OutputVec2(opos);

int** scope;
getSmallScope(blk_man, tpos, &scope);
outputScope(3, (const int**)scope);



OutputLevel((const int**)grid, w, h);
UpdateImmovableBlocks(blk_man);

BuildSelectBar(blk_man, ed);

unsigned int textst = initfont();

unsigned int prog, vao, ibo, vbo;
{
viBundle vbund = GetShapeVertices(SHAPE::SHAPE_SQUARE, 1, 1);  // the bundle containing the vertices and count
viBundle ibund = GetShapeIndices(SHAPE::SHAPE_SQUARE);  // the bundle containing the indices and count

// creating the shader
prog = CreateShader("res/textvert.shader", "res/textfrag.shader");    // creates the shader object
SetUniformM4(prog, "projection", getProjection(1280, 720, 1));  // setting up the projection
SetUniformM4(prog, "view", getM4ID());  // setting up the view

// creating the texture
SetUniform1i(prog, "intexture", 0); // set the texture to be used (the 0th active texture)

vao = CreateVAO();  // creating the vao
ibo = CreateIBO(ibund.vi, ibund.n); // creating the ibo
BindIBO(ibo);  // binding the ibo to the vao
vbo = CreateVBO(vbund.vi, vbund.n); // creating the vbo
BindVBO(vbo);  // binding the vbo to the vao

unsigned int ilay[1] = {3};
VAOLayout layout = CreateVertexLayout(ilay, 5, 1);  // setting up the layout to receive
AddToVertexLayout(layout, 2);  // adding the texture coords to the layout
InitialiseVertexLayout(layout); // initialising the layout to be used
}

while(!glfwWindowShouldClose(window))   // main loop
    {
    glfwWaitEventsTimeout(0.1); // wait for a short time to prevent multiple placements
    MoveCamera(cam);

    CheckEditorInput(ed, blk_man, cam);
    // ed.ui_man.checkUIInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // setting the background colour
    glClear(GL_COLOR_BUFFER_BIT);   // clears colour buffer

    blk_man.drawBlocks(cam);
    ed.ui_man.drawElements(cam);
    
    m4 model = GetModelMatrix((vec2){500.0f, 500.0f}, (vec2){25.0f, 25.0f}, 0.0f);
    ApplyCamera(cam, prog);
    ApplyProjection(cam, prog);

    SetUniformM4(prog, "model", model); // setting the model matrix

    BindTexture(textst);

    BindShader(prog);
    BindVAO(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    GLenum err = glGetError();
    if(err != 0)
        printf("\nERROR: Code %d", err);
    }

glfwDestroyWindow(window);
glfwTerminate();    // cleans up all the glfw objects

return 0;
}
