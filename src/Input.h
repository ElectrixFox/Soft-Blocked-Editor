#ifdef INPUT_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#include "MathsOperations.h"

typedef struct InputPacket
    {
    int key;
    int action;
    int mods;
    } InputPacket;

void InitialiseInput(GLFWwindow* window);

InputPacket getCurrentInputInformation();
void resetInputPacket();

int isPressedSingle(int key);

int isHeldDown(int key);

/**
 * Gets the position for the window
 * 
 * @param window The window to find the position in
 * 
 * @returns The cursor position on the screen
 */
vec2 _getCursorPosition(GLFWwindow* window);

/**
 * Gets the position for the initialised window
 * 
 * @returns The cursor position on the screen
 */
vec2 getCursorPosition();

/**
 * Returns an array of pressed positions
 * 
 * @param poses An array of positions to check
 * @param scales An array of the block scales
 * @param size The size of the arrays
 * @param cpos The cursor position
 * 
 * @returns An array of the pressed indices for the positions
 * 
 * @warning The first element is the size of the array and should be accounted for
 */
int* _getPressedwScale(vec2* poses, vec2* scales, int size, vec2 cpos);

/**
 * Returns an array of pressed positions
 * 
 * @param poses An array of positions to check
 * @param size The size of the array
 * @param cpos The cursor position
 * 
 * @returns An array of the pressed indices for the positions
 * 
 * @warning The first element is the size of the array and should be accounted for
 * 
 * @note This uses the standard scale of (25, 25)
 */
int* _getPressed(vec2* poses, int size, vec2 cpos);


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