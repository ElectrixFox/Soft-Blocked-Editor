#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#include "MathsOperations.h"

typedef struct
    {
    /**
     * -1 is null
     * 0 is released
     * 1 is pressed
     * 2 is repeating
     */
    int keys[1024];

    GLFWwindow* window;
    } InputManager;

void InitialiseInputManager(GLFWwindow* window);

void updateInput(GLFWwindow* window, int key, int scancode, int action, int mods);

vec2 getCursorPosition();

int isPressed(int key);

int isPressedSingle(int key);

int isHeldDown(int key);

/**
 * Gets the indices of the positions which are in the area
 * 
 * @param poses An array of the positions
 * @param size The number of positions
 * @param curpos The position of the cursor
 * @param range The range to check
 * 
 * @returns An array of indices of the positions which are in the area
 * 
 * @warning The first element is the size of the array and should be accounted for
 */
int* _getPressedArea(vec2* poses, int size, vec2 curpos, float range);

/**
 * Gets the indices in the area which has the cursor in it
 * 
 * @param poses An array of the positions
 * @param size The number of positions
 * @param range The range to check
 * 
 * @returns An array of indices of the positions which are in the area
 * 
 * @warning The first element is the size of the array and should be accounted for
 */
int* getPressedArea(vec2* poses, int size, float range);

/**
 * Gets the input window
 * 
 * @returns A pointer to the input window
 * 
 * @warning This should not be used unless there are extreme circumstances
 */
GLFWwindow* getWindow();

#endif
