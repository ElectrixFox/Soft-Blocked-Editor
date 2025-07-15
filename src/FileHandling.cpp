#include "FileHandling.h"

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

const char* ParseShaderSource(const char* filePath)
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

void writeFile(const char* filePath, const char* data)
{
FILE* fptr;

fptr = fopen(filePath, "w");

if(fptr == NULL)
   {
   printf("\nNULL FILE");
   exit(1);
   }

fprintf(fptr, "%s", data);
fclose(fptr);
}