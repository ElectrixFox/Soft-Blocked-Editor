#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#define GLFW_KEY_FULL_STOP GLFW_KEY_PERIOD

#include "src/Transformation.h"
#include "src/RenderObject.h"

#include "src/Camera.h"
#include "src/InputManager.h"
#include "src/Drawable.h"
#include "src/Editor.h"
#include "src/SystemUI.h"

int gwid = 1280, ghig = 720;

void on_window_resize(GLFWwindow* window, int width, int height)
{
gwid = width;
ghig = height;
printf("\n%dx%d", gwid, ghig);
}

void output(int i)
{
printf("\nPressed %d", i);
}

// UI_Table ui;
RenderPacket ui_rp;
InputManager inpman;
UI_Manager ui_man(ui_rp);

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

Camera cam = CreateCamera({0, 0}, {(float)gwid, (float)ghig}, &gwid, &ghig);
ui_rp = InitialiseRenderPacket();
InitialiseInputManager(window);

RenderPacket block_rp = InitialiseRenderPacket();

InitialiseBlockDetails();

/*
vec2 topright = {1255.0f, 695.0f};
const float padding = 10.0f;
vec2 position = {topright.x, topright.y - (0 * 50.0f + padding)}; // placing the items in a vertical line on the right side of the screen

GUI_Button btn = createButton(position, 25.0f, "res/sprites/movable_spritesheet_short.png", 2, 1);
GUI_Button btn2 = createButton(position, 25.0f, "res/sprites/movable_spritesheet_short.png", 2, 2);
unsigned int btn1id = addToElementTable(ui_man, position, 25.0f, btn);
unsigned int btn2id = addToElementTable(ui_man, {position.x - 50.0f, position.y}, 25.0f, btn2);

position = {topright.x, topright.y - (1 * 50.0f + padding)}; // placing the items in a vertical line on the right side of the screen
GUI_Menu men = createMenu(position, btn1id);
unsigned int menid = addToElementTable(ui_man, position, men);

addToMenu(ui_man.ui_men_tab, menid, btn2id);

assignElementAction(ui_man.ui_btn_tab, btn1id, (GUI_ACTION_TRIGGER)0, &output);
assignElementAction(ui_man.ui_men_tab, menid, (GUI_ACTION_TRIGGER)1, &fld);
*/


BuildSelectBar();

const char* deffp = "res/levels/level3.txt";
/*
char* levelfp = (char*)malloc(128 * sizeof(char));
strcpy(levelfp, deffp);
*/

/*
if(argc > 1)
    {
    strcpy(levelfp, argv[1]);
    printf("\n%s", levelfp);
    }
*/

    {
    int** grid;
    int w, h;
    ReadLevel(deffp, &w, &h, &grid);
    
    OutputLevel((const int**)grid, w, h);
    if(w != 0 && h != 0)
        {
        DrawLevel(block_rp, w, h, (const int**)grid);
        
        getLevel(block_rp, &w, &h, &grid);
        // UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
        }
    }

while(!glfwWindowShouldClose(window))   // main loop
    {
    ui_man.checkUI();

    glfwWaitEventsTimeout(0.1); // wait for a short time to prevent multiple placements
    
    if(isPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, 1);
    
    if(isPressedSingle(GLFW_KEY_FULL_STOP))
        {
        int** grid;
        int w, h;
        getLevel(block_rp, &w, &h, &grid);
        UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
        }
    
    if(isHeldDown(GLFW_KEY_LEFT_CONTROL) && isPressedSingle(GLFW_KEY_S))
        {
        int** grid;
        int w, h;
        printf("\nSaving");
        getLevel(block_rp, &w, &h, &grid);
        WriteLevel("res/levels/level3.txt", w, h, (const int**)grid);
        }

    if(isPressedSingle(GLFW_KEY_TAB))
        {
        int** grid;
        int w, h;
        OutputRenderPacketDetails(block_rp);
        // OutputRenderPacketDetails(ui_rp);

        getLevel(block_rp, &w, &h, &grid);
        OutputLevel((const int**)grid, w, h);
        }
    else if(isPressed(GLFW_KEY_0))
        {
        int** grid;
        int w, h;
        getLevel(block_rp, &w, &h, &grid);
        }
    else if(isPressed(GLFW_KEY_1))
        {
        printf("\nCausing break");
        }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
        vec2 cpos = GetCursorPositionRelative(cam);
        vec2 ncpos = getCursorPosition();
        cpos.x = 50 * roundf(cpos.x / 50);
        cpos.y = 50 * roundf(cpos.y / 50);

        if(!PressedArea(block_rp.tds, cpos, 50.0f) && !PressedArea(ui_rp.tds, ncpos, 50.0f))
            {
            printf("\nPlacing block");
            unsigned int rid = _PlaceBlockCustom(block_rp, getActiveBlock(), cpos, 0.0f);
            /*
            if(getBlockFromRenderID(rid) == BLOCK_IMMOVABLE_BLOCK)
                {
                int** grid;
                int w, h;
                getLevel(block_rp, &w, &h, &grid);
                UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
                }
            */
            }
        }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
        vec2 cpos = GetCursorPositionRelative(cam);
        if(PressedAnother(block_rp.tds, cpos))
            {
            printf("\nTrying to remove");
            unsigned int ttrsid = getPressedBlock(block_rp.tds, cpos);
            unsigned int trid = block_rp.drabs.rids[findDrawablesTransform(block_rp.drabs, ttrsid)];
            int check = 0;  // the check to see if the immovables should be updated

            if(getBlockFromRenderID(trid) == BLOCK_IMMOVABLE_BLOCK) check = 1;

            RemoveBlock(block_rp, trid);

            if(check == 1)
                {
                int** grid;
                int w, h;
                getLevel(block_rp, &w, &h, &grid);
                // UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
                }
            }
        }

    MoveCamera(cam);
    ApplyCamera(cam, block_rp.rds);
    ApplyProjection(cam, block_rp.rds);
    ApplyProjection(cam, ui_rp.rds);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // setting the background colour
    glClear(GL_COLOR_BUFFER_BIT);   // clears colour buffer

    DrawRenderPacket(block_rp);
    ClearCamera(ui_rp.rds);
    DrawRenderPacket(ui_rp);
    
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
