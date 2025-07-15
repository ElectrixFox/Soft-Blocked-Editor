#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int getlne(char** lineptr, unsigned int* size, FILE* stream);

const char* readFile(const char* filePath);
const char* ParseShaderSource(const char* filePath);

/**
 * Writes the given data to the file
 * 
 * @param filePath The path to write the file to
 * @param data The data to write to the file
 */
void writeFile(const char* filePath, const char* data);