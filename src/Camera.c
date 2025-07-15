#include "Camera.h"

Camera CreateCamera(vec2 pos, vec2 scale, int* scrwid, int* srchig)
{
return (Camera){0, pos, scale, scrwid, srchig};
}

vec2 GetCursorPositionRelative(Camera cam) { return addVec2(getCursorPosition(), ScalarMultVec2(cam.poscomponent, -1)); }

m4 getCameraMatrix(Camera cam)
{
//  the camera matrix is effectively a projection matrix with a position component
return 
    (m4){
        1.0f, 0.0f, 0.0f, (float)cam.poscomponent.x,
        0.0f, 1.0f, 0.0f, (float)cam.poscomponent.y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

m4 getProjectionMatrix(Camera cam) { return getProjection(*cam.scrspx, *cam.scrspy, 1); }

int MoveCamera(Camera* cam)
{
if(isPressedSingle(GLFW_KEY_W)) // move up
    cam->poscomponent.y -= 5;
else if(isPressedSingle(GLFW_KEY_A)) // move left
    cam->poscomponent.x += 5;
else if(isPressedSingle(GLFW_KEY_S)) // move down
    cam->poscomponent.y += 5;
else if(isPressedSingle(GLFW_KEY_D)) // move right
    cam->poscomponent.x -= 5;
else
    return 0;
return 1;
}

void _ApplyProjection(Camera cam, unsigned int* progs, unsigned int size)
{
m4 proj = getProjectionMatrix(cam);

for (int i = 0; i < size; i++)
    SetUniformM4(progs[i], "projection", proj); // setting the projection matrix
}

void _ApplyCamera(Camera cam, unsigned int* progs, unsigned int size)
{
m4 view = getCameraMatrix(cam);

for (int i = 0; i < size; i++)
    {
    SetUniformM4(progs[i], "view", view);
    }
}

void _ApplyStaticCamera(Camera cam, unsigned int* progs, unsigned int size)
{
m4 view = getCameraMatrix(cam);

for (int i = 0; i < size; i++)
    {
    SetUniformM4(progs[i], "view", view);
    }
}