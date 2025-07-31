#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <thread>

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#include "src/Transformation.h"
#include "src/RenderObject.h"

#include "src/Camera.h"
#include "src/InputManager.h"
#include "src/Drawable.h"
#include "src/Editor.h"
#include "src/SystemUI.h"
#include "src/Text.h"

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
InputManager inpman;
RenderPacket ui_rp;
UI_Manager ui_man(ui_rp);

RenderPacket text_rp;
Character_Table ch_tab(text_rp.rds);
Text_Table txt_tab;

// RenderPacket text_rp;
// Text_Manager text_man(text_rp);


static void toggleMenu(int ui_id)
{
static int prevuid = -1;

if(prevuid != ui_id) // if the previous ID isn't the menu to unfold and the menu is folded
    {
    if(prevuid != -1)
        {
        printf("\nFolding %d", prevuid);
        foldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, prevuid);
        }

    printf("\nUnfolding %d", ui_id);
    unfoldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, ui_id);
    }
}

void UpdateAllImmovables(RenderPacket block_rp)
{
int** grid;
int w, h;
getLevel(block_rp, &w, &h, &grid);
UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
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

Camera cam = CreateCamera({0, 0}, {(float)gwid, (float)ghig}, &gwid, &ghig);
ui_rp = InitialiseRenderPacket();
InitialiseInputManager(window);

RenderPacket block_rp = InitialiseRenderPacket();
ui_man.initialise();
InitialiseBlockDetails();

// #define TEST_TOP_UI_TEXT
#ifdef TEST_TOP_UI_TEXT

// vec2 topleft = {25.0f, 695.0f};
vec2 topleft = {500.0f, 695.0f};
const float padding = 10.0f;
vec2 position = {topleft.x, topleft.y - (0 * 50.0f + padding)}; // placing the items in a vertical line on the right side of the screen

GUI_Button menhead = createButton(position, 25.0f, "res/sprites/movable_spritesheet_short.png", 2, 1);
unsigned int head_id = addToElementTable(ui_man, position, 25.0f, menhead);

GUI_Menu men = createMenu(position, head_id);
unsigned int men_id = addToElementTable(ui_man, position, men);
// assignElementAction(ui_man.ui_men_tab, men_id, UI_TRIGGER_HOVER, &ufld);
// assignElementAction(ui_man.ui_men_tab, men_id, UI_TRIGGER_LEAVE_HOVER, &fld);

GUI_Button entbt = createButton(position, 25.0f, "res/sprites/movable_spritesheet_short.png", 2, 2);

// position = {topleft.x, topleft.y - (1 * 50.0f + padding)};  // placing the items in a vertical line on the right side of the screen
// vec2 posi = {500.0f, 500.0f};
unsigned int entry = addToElementTable(ui_man, {0.0f, 0.0f}, 25.0f, entbt);
assignElementAction(ui_man.ui_btn_tab, entry, (GUI_ACTION_TRIGGER)0, &output);

// addToMenu(ui_man.ui_men_tab, men_id, entry);
addToMenu(ui_man, men_id, entry, 1, 1);

GUI_Text_Box entbx = createTextBox(position, "HELLO");

position = {topleft.x, topleft.y - (2 * 50.0f + padding)};  // placing the items in a vertical line on the right side of the screen
unsigned int bxentry = addToElementTable(ui_man, {0.0f, 0.0f}, entbx);
assignElementAction(ui_man.ui_text_box_tab, bxentry, (GUI_ACTION_TRIGGER)0, &output);

// addToMenu(ui_man.ui_men_tab, men_id, bxentry);
addToMenu(ui_man, men_id, bxentry, 1, 1);

/*
for (int i = 0; i < 26; i++)
    {
    AddCharacter(text_rp, (char)((int)'A' + i), {(i * 50.0f), 2000.0f}, 25.0f);
    }

for (int i = 0; i < 11; i++)
    {
    AddCharacter(text_rp, (char)((int)'0' + i), {((26 + i) * 50.0f), 2000.0f}, 25.0f);
    }
*/

// foldMenu(ui_man, men_id);

// assignElementAction(ui_man.ui_men_tab, men_id, (GUI_ACTION_TRIGGER)0, &output);
#endif

BuildSelectBar();

const char* deffp = "res/levels/level3.txt";
    {
    int** grid;
    int w, h;
    ReadLevel(deffp, &w, &h, &grid);
    
    OutputLevel((const int**)grid, w, h);
    if(w != 0 && h != 0)
        {
        DrawLevel(block_rp, w, h, (const int**)grid);
        
        getLevel(block_rp, &w, &h, &grid);
        UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
        }
    }

int** grid;
int w, h;
int chk;
while(!glfwWindowShouldClose(window))   // main loop
    {
    int updims = 0;
    ui_man.checkUI();
    CheckEditorInput(block_rp, cam, updims, w, h, grid);

    glfwWaitEventsTimeout(0.1); // wait for a short time to prevent multiple placements
    std::thread t1(getLevel, block_rp, &w, &h, &grid);

    MoveCamera(cam);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // setting the background colour
    glClear(GL_COLOR_BUFFER_BIT);   // clears colour buffer

    if(updims)
        {
        printf("\nJoining thread");
        t1.join();
        UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
        }

    ApplyCamera(cam, block_rp.rds);
    ApplyProjection(cam, block_rp.rds);
    ApplyProjection(cam, ui_rp.rds);

    DrawRenderPacket(block_rp);
    ClearCamera(ui_rp.rds);

    DrawDrawablesInstancedPosition(ui_rp.rds, ui_rp.tds, ui_rp.drabs, 50.0f);
   
    glfwSwapBuffers(window);
    glfwPollEvents();

    GLenum err = glGetError();
    if(err != 0)
        printf("\nERROR: Code %d", err);

    if(t1.joinable())
        t1.detach();
    }

glfwDestroyWindow(window);
glfwTerminate();    // cleans up all the glfw objects

return 0;
}
