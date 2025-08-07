#include "Level.hpp"

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
        printf("%-2d ", grid[y][x]);
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

void getLevel(const Block_Manager& blk_man, int* w, int* h, int*** grid)
{
// find the bottom left and top right blocks (the extremes)
float minx = 0, maxx = 0, miny = 0, maxy = 0;

const int tsize = blk_man.getBlockCount();

if(tsize == 0)  // stop if there are no transforms
    {
    *w = 0;
    *h = 0;
    return;
    }

// initialising with the first block
minx = blk_man.blocks[0].pos.x;
maxx = blk_man.blocks[0].pos.x;
miny = blk_man.blocks[0].pos.y;
maxy = blk_man.blocks[0].pos.y;

for (const Block& blk : blk_man.blocks) // getting the extreme points
    {
    const vec2 pos = blk.pos;

    if(pos.x < minx)
        minx = pos.x;
    if(maxx < pos.x)
        maxx = pos.x;
    if(pos.y < miny)
        miny = pos.y;
    if(maxy < pos.y)
        maxy = pos.y;
    }

int gridw = (maxx - minx) / grid_size + 1;  // one longer as we include the final position too
int gridh = (maxy - miny) / grid_size + 1;
printf("\nMinimum (%.2f, %.2f)\tMaximum (%.2f, %.2f)", minx, miny, maxx, maxy);

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
        int find = blk_man.isBlockAt(tpos); // get the transform at the position to check
        if(find == 0)   // if nothing is found then go to the next grid coordinate to check
            continue;
        int btype = blk_man.getBlockAt(tpos).type + 1;
        tgrid[ygrid][xgrid] = btype;    // setting the block
        }
    }

*grid = tgrid;
}

void DrawLevel(Block_Manager& blk_man, int w, int h, const int** grid)
{
for (int y = h; 0 < y; y--)
    {
    vec2 pos = {0.0f, (float)((y - 1) * grid_size)};
    for (int x = 0; x < w; x++)
        {
        pos.x = x * grid_size;

        int btype = grid[h - y][x];

        if(btype != 0)
            {
            Block blk((BLOCK_TYPE)(btype - 1), pos);
            if(blk.type == BLOCK_TYPE::BLOCK_IMMOVABLE_BLOCK)
                {
                blk.update = [](Block& blk)
                    {
                    // printf("\nUpdating");
                    blk.rend_obj = RenderObject(blk.ssi);
                    };
                }
            blk_man.addNewBlock(blk);
            }
        }
    }

}

static void getScope(const int w, const int h, const int** grid, vec2 pos, const int scpesc, int*** scope)
{
int x = pos.x, y = pos.y;

const int sze = sizeof(int) * pow(scpesc, 2);

*scope = new int*[sze];

// *scope = (int**)malloc(sizeof(int) * pow(scpesc, 2));
// int** tscope = (int**)malloc(sizeof(int*) * scpesc);
int** tscope = new int*[sze];

if(scpesc % 2 == 1)
    {
    int interv = (scpesc - 1) / 2;
    for (int j = y - interv; j <= y + interv; j++)
        {
        int jind = j - (y - interv);    // the actual j from 0 to scpesc
        tscope[jind] = new int[sizeof(int) * scpesc];
        // tscope[jind] = (int*)malloc(sizeof(scpesc));
        
        if(j < 0 || h <= j)
            {
            for (int i = 0; i < scpesc; i++)
                tscope[jind][i] = 0;
            continue;
            }

        for (int i = x - scpesc; i <= x + scpesc; i++)
            {
            int iind = i - (x - interv);    // the actual i from 0 to scpesc

            if(i < 0 || w <= i)
                {
                tscope[jind][iind] = 0;
                continue;
                }
            tscope[jind][iind] = grid[j][i];
            }
        }
    }
else
    {
    int interv = scpesc / 2;
    for (int j = y - interv; j < y + interv; j++)
        {
        int jind = j - (y - interv);    // the actual j from 0 to scpesc
        tscope[jind] = new int[sizeof(int) * scpesc];
        // tscope[jind] = (int*)malloc(sizeof(scpesc));
        
        if(j < 0 || h <= j)
            {
            for (int i = 0; i < scpesc; i++)
                tscope[jind][i] = 0;
            continue;
            }

        for (int i = x - interv; i < x + interv; i++)
            {
            int iind = i - (x - interv);    // the actual i from 0 to scpesc

            if(i < 0 || w <= i)
                {
                tscope[jind][iind] = 0;
                continue;
                }
            tscope[jind][iind] = grid[j][i];
            }
        }
    }
memcpy(*scope, tscope, sizeof(int) * scpesc * scpesc);
}

void outputScope(const int scale, const int** scope)
{
printf("\n");
for (int i = 0; i < scale; i++)
    {
    for (int j = 0; j < scale; j++)
        printf("%d ", scope[i][j]);
    printf("\n");
    }
}

static void outputLayout(int* layout)
{
printf("\n%d %d %d %d", layout[0], layout[1], layout[2], layout[3]);
}

static int getFullLineCount(const int scpscale, int** scope)
{
int x = (scpscale - 1) / 2, y = (scpscale - 1) / 2; // the x, y coordinate of the centre
const int imblk = 4;    // the immovable block tag

int lft = (scope[y][x - 1] == imblk);   // checking the left
int rgt = (scope[y][x + 1] == imblk);   // checking the right

int tpt = (scope[y + 1][x] == imblk);   // checking the top
int btt = (scope[y - 1][x] == imblk);   // checking the bottom

return (lft + rgt + tpt + btt);
}

// the layout goes top, right, down, up
static void getLineLayout(const int scpscale, const int** scope, int* layout)
{
int x = (scpscale - 1) / 2, y = (scpscale - 1) / 2; // the x, y coordinate of the centre
const int imblk = 4;    // the immovable block tag

int tpt = (scope[y - 1][x] == imblk);   // checking the top
int rgt = (scope[y][x + 1] == imblk);   // checking the right

int btt = (scope[y + 1][x] == imblk);   // checking the bottom
int lft = (scope[y][x - 1] == imblk);   // checking the left

int lay[4] = {tpt, rgt, btt, lft};

for (int i = 0; i < 4; i++) // setting the output layout
    layout[i] = lay[i];
}

void getBlockScopeAt(const Block_Manager& blk_man, vec2 pos, int*** scope)
{
// allocate 3x3 grid
int** tscope = (int**)malloc(3 * sizeof(int*));
for (int i = 0; i < 3; i++)
    tscope[i] = (int*)malloc(3 * sizeof(int));

// fill the 3x3 grid centered at pos
for (int y = -1; y <= 1; y++)
    {
    for (int x = -1; x <= 1; x++)
        {
        vec2 ipos = {pos.x + x * grid_size, pos.y - y * grid_size};
        if (!blk_man.isBlockAt(ipos))
            {
            tscope[y + 1][x + 1] = 0;
            continue;
            }
        tscope[y + 1][x + 1] = blk_man.getBlockAt(ipos).type + 1;
        }
    }

*scope = tscope;
}

BLOCK_IM_STATE getImmovableTypeScope(const int** scope, vec2 pos, float* angle)
{
int lnecnt = getFullLineCount(3, (int**)scope);
*angle = 0;

// {tpt, rgt, btt, lft};
int layout[4] = {0, 0, 0, 0};
switch (lnecnt)
    {
    case 0: // if it is zero then it is on its own
        return BLOCK_IM_STATE_ALONE;
        break;
    case 1: // if it is 1 then it is an end line
        getLineLayout(3, (const int**)scope, layout);   // getting the layout to test if it is a corner or a full line
        if(layout[0])   // if there is a block above
            *angle = 0 * M_PI / 180;
        else if(layout[1])  // if there is a block to the right
            *angle = 270 * M_PI / 180;
        else if(layout[2])  // if there is a block below
            *angle = 180 * M_PI / 180;
        else if(layout[3])  // if there is a block to the left
            *angle = 90 * M_PI / 180;
        return BLOCK_IM_STATE_LINE_END;
        break;
    case 2: // if it is 2 then it is either a corner or a full line
        getLineLayout(3, (const int**)scope, layout);   // getting the layout to test if it is a corner or a full line
        if((layout[0] && layout[2]) || (layout[1] && layout[3]))  // a full line
            {
            if(layout[1])   // if there is a block to the left or right rotate
                *angle = 90 * M_PI / 180;
            
            return BLOCK_IM_STATE_LINE_STRAIGHT;
            }
        else if((layout[0] || layout[2]) && (layout[1] || layout[3]))   // then it is a corner
            {
            // outputLayout(layout);
            if(layout[0] && layout[1])  // top and right
                {
                // printf("\nTop right");
                *angle = 90 * M_PI / 180;
                }
            else if(layout[0] && layout[3]) // top and left
                {
                // printf("\nTop left");
                *angle = 180 * M_PI / 180;
                }
            else if(layout[2] && layout[3]) // bottom and left
                {
                // printf("\nBottom left");
                *angle = 270 * M_PI / 180;
                }
            else if(layout[2] && layout[1]) // bottom and right
                {
                // printf("\nBottom right");
                *angle = 0 * M_PI / 180;
                }

            return BLOCK_IM_STATE_CORNER;
            }

        break;
    case 3: // if it is 3 then it is a three intersection
        getLineLayout(3, (const int**)scope, layout);    // gets the layout
        if(layout[0] && layout[3] && layout[2]) // if there is up-down-left 
            *angle = 0 * M_PI / 180;
        else if(layout[0] && layout[1] && layout[2])    // if there is up-down-right
            *angle = 180 * M_PI / 180;
        else if(layout[0] && layout[1] && layout[3])    // if there is up-left-right
            *angle = 270 * M_PI / 180;
        else if(layout[2] && layout[1] && layout[3])    // if there is down-left-right
            *angle = 90 * M_PI / 180;
        return BLOCK_IM_STATE_THREE_INTERSECT;
        break;
    case 4: // if it is 4 then it is a four way intersection
        return BLOCK_IM_STATE_FOUR_INTERSECT;
        break;
    default:
        break;
    }

return BLOCK_IM_STATE_ALONE;

}

void UpdateImmovableBlock(Block_Manager& blk_man, Block& blk)
{
float theta = 0.0f;
int** scope;

if(blk.type != BLOCK_TYPE::BLOCK_IMMOVABLE_BLOCK)
    return;

getBlockScopeAt(blk_man, blk.pos, &scope);
outputScope(3, (const int**)scope);
BLOCK_IM_STATE imstate = getImmovableTypeScope((const int**)scope, blk.pos, &theta);

blk.ssi = getImmovableBlock(imstate);
blk.angle = theta;
blk.update(blk);

}

BLOCK_IM_STATE getImmovableType(const int w, const int h, const int** grid, vec2 pos, float* angle)
{
int x = pos.x, y = pos.y;
const int imblk = 4;    // the immovable block tag

int** scope;

if(grid[y][x] != imblk)
    {
    printf("\nError: Trying to find type for non-immovable");
    return (BLOCK_IM_STATE)-1;
    }

getScope(w, h, grid, pos, 3, &scope);   // gets the scope
int lnecnt = getFullLineCount(3, scope);
*angle = 0;

// {tpt, rgt, btt, lft};
int layout[4] = {0, 0, 0, 0};
switch (lnecnt)
    {
    case 0: // if it is zero then it is on its own
        return BLOCK_IM_STATE_ALONE;
        break;
    case 1: // if it is 1 then it is an end line
        getLineLayout(3, (const int**)scope, layout);   // getting the layout to test if it is a corner or a full line
        if(layout[0])   // if there is a block above
            *angle = 0 * M_PI / 180;
        else if(layout[1])  // if there is a block to the right
            *angle = 270 * M_PI / 180;
        else if(layout[2])  // if there is a block below
            *angle = 180 * M_PI / 180;
        else if(layout[3])  // if there is a block to the left
            *angle = 90 * M_PI / 180;
        return BLOCK_IM_STATE_LINE_END;
        break;
    case 2: // if it is 2 then it is either a corner or a full line
        getLineLayout(3, (const int**)scope, layout);   // getting the layout to test if it is a corner or a full line
        if((layout[0] && layout[2]) || (layout[1] && layout[3]))  // a full line
            {
            if(layout[1])   // if there is a block to the left or right rotate
                *angle = 90 * M_PI / 180;
            
            return BLOCK_IM_STATE_LINE_STRAIGHT;
            }
        else if((layout[0] || layout[2]) && (layout[1] || layout[3]))   // then it is a corner
            {
            // outputLayout(layout);
            if(layout[0] && layout[1])  // top and right
                {
                // printf("\nTop right");
                *angle = 90 * M_PI / 180;
                }
            else if(layout[0] && layout[3]) // top and left
                {
                // printf("\nTop left");
                *angle = 180 * M_PI / 180;
                }
            else if(layout[2] && layout[3]) // bottom and left
                {
                // printf("\nBottom left");
                *angle = 270 * M_PI / 180;
                }
            else if(layout[2] && layout[1]) // bottom and right
                {
                // printf("\nBottom right");
                *angle = 0 * M_PI / 180;
                }

            return BLOCK_IM_STATE_CORNER;
            }

        break;
    case 3: // if it is 3 then it is a three intersection
        getLineLayout(3, (const int**)scope, layout);    // gets the layout
        if(layout[0] && layout[3] && layout[2]) // if there is up-down-left 
            *angle = 0 * M_PI / 180;
        else if(layout[0] && layout[1] && layout[2])    // if there is up-down-right
            *angle = 180 * M_PI / 180;
        else if(layout[0] && layout[1] && layout[3])    // if there is up-left-right
            *angle = 270 * M_PI / 180;
        else if(layout[2] && layout[1] && layout[3])    // if there is down-left-right
            *angle = 90 * M_PI / 180;
        return BLOCK_IM_STATE_THREE_INTERSECT;
        break;
    case 4: // if it is 4 then it is a four way intersection
        return BLOCK_IM_STATE_FOUR_INTERSECT;
        break;
    default:
        break;
    }

return BLOCK_IM_STATE_ALONE;
}

static vec2 getMinimumPosition(const Block_Manager& blk_man)
{
// find the bottom left and top right blocks (the extremes)
float minx = 0, maxx = 0, miny = 0, maxy = 0;

if(blk_man.getBlockCount() == 0)   // stop if there are no transforms
    return {-1.0f, -1.0f};

// initialising with the first block
minx = blk_man.blocks[0].pos.x;
maxx = blk_man.blocks[0].pos.x;
miny = blk_man.blocks[0].pos.y;
maxy = blk_man.blocks[0].pos.y;

for (const Block& blk : blk_man.blocks) // getting the extreme points
    {
    const vec2 pos = blk.pos;

    if(pos.x < minx)
        minx = pos.x;
    else if(maxx < pos.x)
        maxx = pos.x;
    else if(pos.y < miny)
        miny = pos.y;
    else if(maxy < pos.y)
        maxy = pos.y;
    }

return {minx, miny};
}

unsigned int UpdateImmovableBlocks(Block_Manager& blk_man)
{
printf("\n\n\nImmovables update");
vec2 minpos = getMinimumPosition(blk_man);

int w, h;
int** grid;
getLevel(blk_man, &w, &h, &grid);
OutputLevel((const int**)grid, w, h);

for (int i = 0; i < h; i++)
    {
    for (int j = 0; j < w; j++)
        {
        if(grid[i][j] == (int)BLOCK_IMMOVABLE_BLOCK + 1) // if there is an immovable block there
            {
            float theta = 0.0f;
            BLOCK_IM_STATE imstate = getImmovableType(w, h, (const int**)grid, {(float)j, (float)i}, &theta);
            vec2 posi = {minpos.x + j * grid_size, minpos.y + (h - (i + 1)) * grid_size};   // h - (i + 1) as i never reaches h so the expression never checks the minimum y

            int find = blk_man.isBlockAt(posi); // get the transform at the position to check
            if(find == 0)   // if nothing is found then go to the next grid coordinate to check
                continue;

            Block& blk = blk_man.getBlockAt(posi);
            blk.ssi = getImmovableBlock(imstate);
            blk.angle = theta;
            blk.update(blk);
            }
        }
    }

return 0;
}