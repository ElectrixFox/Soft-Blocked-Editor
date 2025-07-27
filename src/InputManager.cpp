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

void updateMouse(GLFWwindow* window, int button, int action, int mods)
{
switch (action)
    {
    case GLFW_PRESS:
        inpman.mbtns[button] = 1;
        break;
    case GLFW_RELEASE:
        inpman.mbtns[button] = 0;
        break;
    case GLFW_REPEAT:
        inpman.mbtns[button] = 2;
        break;
    default:
        break;
    }
}

void updateCurpos(GLFWwindow* window, double xpos, double ypos)
{
inpman.cpos = {(float)xpos, (float)ypos};
}

vec2 _getCursorPosition(GLFWwindow* window)
{
double x, y;
int wid, hig;
glfwGetCursorPos(window, &x, &y);
glfwGetWindowSize(window, &wid, &hig);
vec2 point = GetMousePositionRelative({(float)x, (float)y}, wid, hig);

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


int isMouseButtonPressed(int button)
{
static int pbtn = -1;
int action = glfwGetMouseButton(inpman.window, button);

if(pbtn != button && action == GLFW_PRESS)
    {
    pbtn = button;
    return 1;
    }
else if(pbtn == button && action == GLFW_RELEASE)
    {
    pbtn = -1;
    }

return 0;
}

int isMouseButtonHeld(int button)
{
if(inpman.mbtns[button] == 1)
    {
    return 1;
    }
else
    {
    return 0;
    }
}

std::vector<int> _getPressedArea(std::vector<vec2> poses, vec2 curpos, float range)
{
std::vector<int> indices;

for (int i = 0; i < poses.size(); i++)
    if(abs(poses[i].x - curpos.x) < range && abs(poses[i].y - curpos.y) < range)
        indices.push_back(i);

return indices;
}

std::vector<int> getPressedArea(std::vector<vec2> poses, float range)
{
return _getPressedArea(poses, getCursorPosition(), range);
}

GLFWwindow* getWindow()
{
return inpman.window;
}