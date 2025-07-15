#include "Level.h"

int getOccurences(const char* string, const char* substr)
{
int count = 0;
const char* tmp = string;
while((tmp = strstr(tmp, substr)))
    {
    count++;
    tmp++;
    }
return count;
}

void OutputLevel(const int** grid, int w, int h)
{
printf("\nLevel\n");
for (int y = 0; y < h; y++)
    {
    for (int x = 0; x < w; x++)
        {
        printf("%d ", grid[y][x]);
        }
    printf("\n");
    }
}

void ReadLevel(const char* levelfp, int* w, int* h, int*** grid)
{
const char* filcont = readFile(levelfp);
int l = strlen(filcont);
if(l == 0)  // if the file is empty
    {
    // set the width and height to 0
    *w = 0;
    *h = 0;
    return; // early return
    }
int gridw = 0;
int gridh = 0;

char* fcont = (char*)malloc(l * sizeof(char));
strcpy(fcont, filcont);
char* fline = strtok(fcont, "\n");
gridw = getOccurences(fline, " ") + 1;    // the width is the number of spaces add one (accounting for the last entry) 
gridh = getOccurences(filcont, "\n") + 1;   // the height is the number of new lines add one (accounting for the last line)

int** tgrid = (int**)malloc(sizeof(int*) * gridw * gridh);    // initialising the grid

*w = gridw;
*h = gridh;

char** lines = (char**)malloc(strlen(filcont) * sizeof(char));
for(int i = 0; fline != NULL; i++)
    {
    lines[i] = (char*)malloc(strlen(fline) * sizeof(char));
    strcpy(lines[i], fline);
    fline = strtok(NULL, "\n");
    }

for (int i = 0; i < gridh; i++)
    {
    tgrid[i] = (int*)calloc(gridw, sizeof(int));    // allocate the memory for the row
    char* tstr = strtok(lines[i], " "); // getting the next line part
    int j = 0;
    while(tstr != NULL)
        {
        tgrid[i][j] = atoi(tstr);   // changing the current grid entry to an integer
        tstr = strtok(NULL, " ");   // getting the next bit of the line
        j++;
        }
    }

*grid = tgrid;
}

void WriteLevel(const char* levelfp, const int w, const int h, const int** grid)
{
char* output = (char*)malloc(w * h * sizeof(int) * sizeof(char));
output[0] = '\0';   // setting the string as empty by setting the first character to be the end 

for (int i = 0; i < h; i++)
    {
    for (int j = 0; j < w; j++)
        {
        char tmp[32];
        sprintf(tmp, "%d ", grid[i][j]);    // convert the int to a string
        strcat(output, tmp);    // append the string to the output
        }
    output[strlen(output) - 1] = '\n';  // set the last character of the line to be the new line instead of a space
    }
output[strlen(output) - 1] = '\0';  // set the last character of the output to be the null character (signalling the end of the string)


writeFile(levelfp, output);
}

void getLevel(const RenderPacket rp, int* w, int* h, int*** grid)
{
// find the bottom left and top right blocks (the extremes)
float minx = 0, maxx = 0, miny = 0, maxy = 0;

if(rp.tds.size == 0)   // stop if there are no transforms
    return;

for (int i = 0; i < rp.tds.size; i++)  // find the first deletable
    {
    minx = rp.tds.pos[i].x;
    maxx = rp.tds.pos[i].x;
    miny = rp.tds.pos[i].y;
    maxy = rp.tds.pos[i].y;
    }


for (int i = 0; i < rp.tds.size; i++)
    {
    // getting the extreme points
    if(rp.tds.pos[i].x < minx)
        minx = rp.tds.pos[i].x;

    if(maxx < rp.tds.pos[i].x)
        maxx = rp.tds.pos[i].x;
    
    if(rp.tds.pos[i].y < miny)
        miny = rp.tds.pos[i].y;
    
    if(maxy < rp.tds.pos[i].y)
        maxy = rp.tds.pos[i].y;
    }

int gridw = (maxx - minx) / grid_size + 1;  // one longer as we include the final position too
int gridh = (maxy - miny) / grid_size + 1;

int** tgrid = (int**)malloc(sizeof(int*) * gridw * gridh);    // initialising the grid

*w = gridw;
*h = gridh;

for (int i = 0; i < gridh; i++)
    tgrid[i] = (int*)calloc(gridw, sizeof(int));    // allocate the memory for the row

for (float y = maxy; miny <= y; y -= (float)grid_size)
    {
    int ygrid = (int)((maxy - y) / grid_size);

    for (float x = minx; x <= maxx; x += (float)grid_size)  // go along the row
        {
        int xgrid = (int)((x - minx) / grid_size);
        vec2 tpos = (vec2){x, y};   // the temporary position
        int tid = getTransformAt(rp.tds, tpos);    // get the transform at the position to check
        if(tid == -1)   // if nothing is found then go to the next grid coordinate to check
            continue;
        unsigned int trid = rp.drabs.rids[findDrawablesTransform(rp.drabs, tid)]; // gets the render ID from the renderables
        int btype = (int)getBlockFromRenderID(trid) + 1;  // finding the type of block and adding 1
        tgrid[ygrid][xgrid] = btype;    // setting the block
        }
    }

*grid = tgrid;
}

void DrawLevel(RenderPacket* rp, int w, int h, const int** grid)
{
for (int y = h; 0 < y; y--)
    {
    vec2 pos = {0.0f, (y - 1) * grid_size};
    for (int x = 0; x < w; x++)
        {
        pos.x = x * grid_size;

        int btype = grid[h - y][x];

        if(btype != 0)
            PlaceBlock(rp, (BLOCK)(btype - 1), pos);
        }
    }


}