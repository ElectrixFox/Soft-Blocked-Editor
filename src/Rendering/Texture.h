#pragma once
#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <tuple>

unsigned int SetTextureVariable(unsigned int tex, int active);

unsigned int getActiveTexture(unsigned int texture);
unsigned int getTexture(unsigned int texture);

unsigned int CreateTexture(const char*, int active = 0, int flip = 0);

void BindTexture(unsigned int texture);

const char* getTextureFilePath(unsigned int texture);