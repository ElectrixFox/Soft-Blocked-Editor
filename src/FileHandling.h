#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int getlne(char** lineptr, unsigned int* size, FILE* stream);

const char* readFile(const char* filePath);
const char* ParseShaderSource(const char* filePath);

/**
 * Parses the full shader file
 * 
 * @param filePath The file path to the shader
 * @param outVss A pointer to the vertex shader source output
 * @param vLen A pointer to the length of the vertex shader source
 * @param outFss A pointer to the fragment shader source output
 * @param fLen A pointer to the length of the fragment shader source
 */
void ParseShader(const char* filePath, char** outVss, int* vLen, char** outFss, int* fLen);

/**
 * Writes the given data to the file
 * 
 * @param filePath The path to write the file to
 * @param data The data to write to the file
 */
void writeFile(const char* filePath, const char* data);