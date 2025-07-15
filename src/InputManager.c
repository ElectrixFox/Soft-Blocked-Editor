#include "InputManager.h"

extern InputManager inpman;

void InitialiseInputManager(GLFWwindow* window)
{
inpman.window = window;
}

void updateInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
switch (action)
    {
    case GLFW_PRESS:
        inpman.keys[key] = 1;
        break;
    case GLFW_RELEASE:
        inpman.keys[key] = 0;
        break;
    case GLFW_REPEAT:
        inpman.keys[key] = 2;
        break;
    default:
        break;
    }
}

vec2 _getCursorPosition(GLFWwindow* window)
{
double x, y;
int wid, hig;
glfwGetCursorPos(window, &x, &y);
glfwGetWindowSize(window, &wid, &hig);
vec2 point = GetMousePositionRelative((vec2){(float)x, (float)y}, wid, hig);

return point;
}

vec2 getCursorPosition() { return _getCursorPosition(inpman.window); } 

int isPressed(int key)
{
return (inpman.keys[key] == 1) ? 1 : 0;
}

int isPressedSingle(int key)
{
static int pkey = -1;
int action = glfwGetKey(inpman.window, key);

if(pkey != key && action == GLFW_PRESS)
    {
    pkey = key;
    return 1;
    }
else if(pkey == key && action == GLFW_RELEASE)
    {
    pkey = -1;
    }

return 0;
}

int isHeldDown(int key)
{
if(inpman.keys[key] == 2)
    {
    return 1;
    }
else
    {
    return 0;
    }
}

int* _getPressedArea(vec2* poses, int size, vec2 curpos, float range)
{
int* indices = (int*)malloc(2 * sizeof(int));
int top = 1;

for (int i = 0; i < size; i++)
    if(abs(poses[i].x - curpos.x) < range && abs(poses[i].y - curpos.y) < range)
        {
        indices[top] = i;
        ExpandByOne(&indices, top, sizeof(int));
        top++;
        }

indices[0] = top - 1;   // setting the size of the array

return indices;
}

int* getPressedArea(vec2* poses, int size, float range)
{
return _getPressedArea(poses, size, getCursorPosition(), range);
}

GLFWwindow* getWindow()
{
return inpman.window;
}