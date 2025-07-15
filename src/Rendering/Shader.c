#include "Shader.h"

unsigned int createShader(const char* vertsrc, const char* fragsrc)
{
char errmsg[512];
int success;

unsigned int vertshad;
vertshad = glCreateShader(GL_VERTEX_SHADER);    // creating the shader
glShaderSource(vertshad, 1, &vertsrc, NULL);    // sets the source of the shader
glCompileShader(vertshad);  // compiles the shader

glGetShaderiv(vertshad, GL_COMPILE_STATUS, &success);
if (!success)
    {
    glGetShaderInfoLog(vertshad, 512, NULL, errmsg);
    printf("ERROR: Vertex Shader %s", errmsg);
    }

/*
do error management here
*/

unsigned int fragshad;
fragshad = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragshad, 1, &fragsrc, NULL);
glCompileShader(fragshad);

/*
do error management here
*/
glGetShaderiv(fragshad, GL_COMPILE_STATUS, &success);
if (!success)
    {
    glGetShaderInfoLog(fragshad, 512, NULL, errmsg);
    printf("ERROR: Fragment Shader %s", errmsg);
    }

unsigned int program;   // the program linking both shaders
program = glCreateProgram();
glAttachShader(program, vertshad);
glAttachShader(program, fragshad);
glLinkProgram(program);

glGetProgramiv(program, GL_LINK_STATUS, &success);
if (!success)
    {
    glGetProgramInfoLog(program, 512, NULL, errmsg);
    printf("ERROR: %s", errmsg);
    }
/*
do error checking here
*/

glUseProgram(program);  // actually using the shaders for all future objects

glDeleteShader(vertshad);   // delete the unnecessary shaders
glDeleteShader(fragshad);

return program;
}

unsigned int CreateShader(const char* vertfp, const char* fragfp)
{
char errmsg[512];
int success;
const char* vertsrc = ParseShaderSource(vertfp);

unsigned int vertshad;
vertshad = glCreateShader(GL_VERTEX_SHADER);    // creating the shader
glShaderSource(vertshad, 1, &vertsrc, NULL);    // sets the source of the shader
glCompileShader(vertshad);  // compiles the shader

glGetShaderiv(vertshad, GL_COMPILE_STATUS, &success);
if (!success)
    {
    glGetShaderInfoLog(vertshad, 512, NULL, errmsg);
    printf("ERROR: Vertex Shader %s", errmsg);
    }

const char* fragsrc = ParseShaderSource(fragfp);
unsigned int fragshad;
fragshad = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragshad, 1, &fragsrc, NULL);
glCompileShader(fragshad);

glGetShaderiv(fragshad, GL_COMPILE_STATUS, &success);
if (!success)
    {
    glGetShaderInfoLog(fragshad, 512, NULL, errmsg);
    printf("ERROR: Fragment Shader %s", errmsg);
    }

unsigned int program;   // the program linking both shaders
program = glCreateProgram();
glAttachShader(program, vertshad);
glAttachShader(program, fragshad);
glLinkProgram(program);

glGetProgramiv(program, GL_LINK_STATUS, &success);
if (!success)
    {
    glGetProgramInfoLog(program, 512, NULL, errmsg);
    printf("ERROR: %s", errmsg);
    }

glUseProgram(program);  // actually using the shaders for all future objects

glDeleteShader(vertshad);   // delete the unnecessary shaders
glDeleteShader(fragshad);

return program;
}

void BindShader(unsigned int program)
{
glUseProgram(program);  // binds the shader program
}

void SetUniform1f(unsigned int program, const char* varname, float value)
{
BindShader(program);
int uniloc = glGetUniformLocation(program, varname);
glUniform1f(uniloc, value);
}

void SetUniform4f(unsigned int program, const char* varname, vec4 value)
{
BindShader(program);
int uniloc = glGetUniformLocation(program, varname);
glUniform4f(uniloc, value[0], value[1], value[2], value[3]);
}

void SetUniformM4(unsigned int program, const char* varname, m4 matr)
{
BindShader(program);
int uniloc = glGetUniformLocation(program, varname);
glUniformMatrix4fv(uniloc, 1, GL_FALSE, &matr.mat[0][0]);
}

void SetUniform1i(unsigned int program, const char* varname, unsigned int value)
{
BindShader(program);
int uniloc = glGetUniformLocation(program, varname);
glUniform1i(uniloc, value);
}