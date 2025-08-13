#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int getlne(char** lineptr, unsigned int* size, FILE* stream)
{
if(feof(stream))   // if the end of the file exit
   return -1;
   
char line[256];

fgets(line, 256, stream);   // gets the line from the stream
unsigned int len = strlen(line);
lineptr[0] = (char*)malloc(len * sizeof(char*));
strcpy(*lineptr, line); // copies the line back into the pointer
*size += len;

return (len);
}

const char* readFile(const char* filePath)
{
FILE* file = fopen(filePath, "r");  // open the file to read
if (file == NULL)   // error handling
    {
    printf("ERROR: File %s is not found\n", filePath);
    return NULL;
    }

const int size = sizeof(char) * (1024);    // num of chars expected
char* output = (char*)malloc(size);  // setting allocating some memory
output[0] = '\0'; // setting the start as the end

char* line = NULL;
int i = 0;
unsigned int bufsize = 0;
while(getlne(&line, &bufsize, file) != -1)  // while not at the end of the file
   {
   strcat(output, line);  // add the line to the pointer
   i++;
   }
const char* res = output;

fclose(file);

return res;
}


void ParseShader(const char* filePath, char** outVss, int* vLen, char** outFss, int* fLen)
{
char* cont = (char*)readFile(filePath);
char shsrcs[2][1024];  // the shader sources

int type = -1; // start with -1 for none, vertex shader is 0, fragment shader is 1
char* line = strtok(cont, "\n"); // starting the tokenisation

while(line != NULL)  // until there are no more tokens
   {
   if(strstr(line, "#shader"))   // if the shader part is there
      {
      if(strstr(line, "Vertex"))
         type = 0;
      else if(strstr(line, "Fragment"))
         type = 1;
      shsrcs[type][0] = '\0';   // adding a new line in
      }
   else if(type != -1)
      {
      strcat(shsrcs[type], line);   // appending the line
      strcat(shsrcs[type], "\n");   // adding a new line in
      }
   
   line = strtok(NULL, "\n");
   }

*outVss = (char*)malloc(sizeof(char) * strlen(shsrcs[0]));  // allocating the source some memory
strcpy(*outVss, shsrcs[0]);   // copying the source into the output

printf("\nTesting: \n%s", shsrcs[1]);
*outFss = (char*)malloc(sizeof(char) * strlen(shsrcs[1]));  // allocating the source some memory
strcpy(*outFss, shsrcs[1]);   // copying the source into the output
}

int main(int argc, char const *argv[])
{
int vlen, flen;
char* vsrc;
char* fsrc;


printf("\nStart");

ParseShader("res/shader.glsl", &vsrc, &vlen, &fsrc, &flen);  // parsing the shader

printf("\nVertex Shader: %s\n\nFragment Shader: %s", vsrc, fsrc);

printf("\nEnd");

return 0;
}
