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
/*Block blk1(BLOCK_TYPE::BLOCK_PLAYER, {400.0f, 500.0f});
Block blk2(BLOCK_TYPE::BLOCK_MOVABLE_BLOCK, {450.0f, 500.0f});
Block blk3(BLOCK_TYPE::BLOCK_MOVABLE_DESTINATION, {500.0f, 500.0f});

blk_man.addNewBlock(blk1);
blk_man.addNewBlock(blk2);
blk_man.addNewBlock(blk3);
*/

Editor ed;

int w, h;
int** grid;
ReadLevel("res/levels/level1.txt", &w, &h, &grid);
OutputLevel((const int**)grid, w, h);
DrawLevel(blk_man, w, h, (const int**)grid);
getLevel(blk_man, &w, &h, &grid);
OutputLevel((const int**)grid, w, h);
UpdateImmovableBlocks(blk_man);

while(!glfwWindowShouldClose(window))   // main loop
    {
    glfwWaitEventsTimeout(0.1); // wait for a short time to prevent multiple placements
    MoveCamera(cam);

    CheckEditorInput(ed, blk_man, cam);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // setting the background colour
    glClear(GL_COLOR_BUFFER_BIT);   // clears colour buffer

    blk_man.drawBlocks(cam);
    
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
