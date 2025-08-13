#include "Camera.hpp"

Camera CreateCamera(vec2 pos, vec2 scale, int* scrwid, int* srchig)
{
return {0, pos, scale, scrwid, srchig};
}

vec2 GetCursorPositionRelative(Camera cam) { return addVec2(getCursorPosition(), ScalarMultVec2(cam.poscomponent, -1)); }

m4 getCameraMatrix(Camera cam)
{
//  the camera matrix is effectively a projection matrix with a position component
return 
    {
        1.0f, 0.0f, 0.0f, (float)cam.poscomponent.x,
        0.0f, 1.0f, 0.0f, (float)cam.poscomponent.y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

m4 getProjectionMatrix(Camera cam) { return getProjection(*cam.scrspx, *cam.scrspy, 1); }

int MoveCamera(Camera& cam)
{
static vec2 prevpos = getCursorPosition();  // initially it should be the cursor posision

if(isMouseButtonHeld(GLFW_MOUSE_BUTTON_MIDDLE))
    {
    vec2 cpos = getCursorPosition(); // getting where the cursor is
    if(prevpos.x != cpos.x || prevpos.y != cpos.y)  // if the vectors are different
        {
        float dx = prevpos.x - cpos.x;  // find the change in the x coordinate
        float dy = prevpos.y - cpos.y;  // find the change in the y coordinate
        
        // apply the "drag"
        cam.poscomponent.x -= dx;   // subtract the difference as its a drag
        cam.poscomponent.y -= dy;
        }
    prevpos = cpos; // setthe previous position to the current
    }
else
    prevpos = getCursorPosition();   // reinitialise the previous position
/*
if(isPressedSingle(GLFW_KEY_W)) // move up
    cam.poscomponent.y -= 5;
else if(isPressedSingle(GLFW_KEY_A)) // move left
    cam.poscomponent.x += 5;
else if(isPressedSingle(GLFW_KEY_S)) // move down
    cam.poscomponent.y += 5;
else if(isPressedSingle(GLFW_KEY_D)) // move right
    cam.poscomponent.x -= 5;
else
    return 0;
*/
return 1;
}

void ApplyCamera(Camera cam, unsigned int prog) 
{
m4 view = getCameraMatrix(cam);
SetUniformM4(prog, "view", view);
}

void ClearCamera(unsigned int prog) { SetUniformM4(prog, "view", getM4ID()); }

void ApplyProjection(Camera cam, unsigned int prog)
{
const m4 proj = getProjectionMatrix(cam);
SetUniformM4(prog, "projection", proj); // setting the projection matrix
}
