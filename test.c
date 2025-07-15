#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void OutputTable(const char* title, const char* headers[], unsigned int nHeaders, ...)
{
int theadlen = 0;
int spacin = 2; // spaces between headers
for (int i = 0; i < nHeaders; i++)
    theadlen += strlen(headers[i]);

int titpad = (int)((strlen(title) + theadlen) / 2); // title padding
printf("\n\n%-*s", titpad, title);

char* format = (char*)malloc(theadlen * sizeof(char));  // allocating the size of the format

printf("\n");
for (int i = 0; i < nHeaders; i++)
    {
    strcat(format, "%-*s");
    printf("%-*s", strlen(headers[i]) + spacin, headers[i]);    // setting up the headers
    }


va_list valst;

va_start(valst, nHeaders);

void*** data = (void***)malloc(256);

for (int i = 0; i < nHeaders; i++)
    {
    void** arg = va_arg(valst, void**);
    data[i] = arg;
    }

va_end(valst);

}


int main()
{
/*
float tst = 56.3135132f;
printf("Output: %06.2f", tst);


printf("\n\n%32s", "Transformations Table");
printf("\n%-10s%-20s%-13s\t", "ID", "Position", "Scale");
printf("\n%-10d(%07.2f, %07.2f)%-2s(%05.2f, %05.2f)\t");
*/

const char* titles[] = {"ID", "Position", "Scale"};
OutputTable("Transformations Table", titles, 3, "1", "3", "4");

return 0;
}