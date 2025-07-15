#pragma once
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>

#include "src/FileHandling.h"
#include "src/MathsOperations.h"


unsigned int createShader(const char* vertsrc, const char* fragsrc);

unsigned int CreateShader(const char* vertfp, const char* fragfp);

void BindShader(unsigned int program);

void SetUniform1f(unsigned int program, const char* varname, float value);
void SetUniform4f(unsigned int program, const char* varname, vec4 value);
void SetUniformM4(unsigned int program, const char* varname, m4 matr);
void SetUniform1i(unsigned int program, const char* varname, unsigned int value);