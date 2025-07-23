#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <tuple>

unsigned int getActiveTexture(unsigned int texture);
unsigned int getTexture(unsigned int texture);

unsigned int CreateTexture(const char*);

void BindTexture(unsigned int texture);

const char* getTextureFilePath(unsigned int texture);