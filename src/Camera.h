#pragma once
#include "MathsOperations.h"
#include "InputManager.h"
#include "Rendering/Shader.h"

struct Camera
    {
    unsigned int camid; // i.e. entity ID
    vec2 poscomponent;  // i.e. pos
    vec2 zoomcomponent; // i.e. scaled
    int* scrspx;    // screen dimension y
    int* scrspy;    // screen dimension x
    };
typedef struct Camera Camera;

Camera CreateCamera(vec2 pos, vec2 scale, int* scrwid, int* srchig);

/**
 * Gets the relative cursor position of the cursor compared to the camera
 * 
 * @param cam The camera
 * 
 * @returns The position of the cursor relative to the camera
 */
vec2 GetCursorPositionRelative(Camera cam);


m4 getCameraMatrix(Camera cam);

/**
 * Gets the projection matrix to be used
 * 
 * @param cam The camera
 * 
 * @returns A 4x4 matrix with the projection
 */
m4 getProjectionMatrix(Camera cam);

/**
 * Moves the camera using the keys pressed
 * 
 * @param cam A pointer to the camera
 * 
 * @returns 1 if the camera has been moved otherwise 0
 */
int MoveCamera(Camera* cam);

/**
 * Sets all of the projection uniforms
 * 
 * @param cam The camera
 * @param progs The array of shader programs
 * @param size The number of programs
 */
void _ApplyProjection(Camera cam, unsigned int* progs, unsigned int size);

/**
 * Applies the view matrix to the program
 * 
 * @param cam The camera in the scene
 * @param progs The shader programs
 * @param size The number of programs
 */
void _ApplyCamera(Camera cam, unsigned int* progs, unsigned int size);

/**
 * Applies a matrix to keep the given programs static on the screen with relation to the camera
 * 
 * @param cam The camera
 * @param progs The shader programs
 * @param size The number of programs
 */
void _ApplyStaticCamera(Camera cam, unsigned int* progs, unsigned int size);