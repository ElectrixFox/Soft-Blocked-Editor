#include "Block.h"

static BlockDetails blds;

void setBlockSprite(unsigned int* block, unsigned int spr) { SetActiveSprite(block, spr); }

unsigned int getBlockSprite(unsigned int block) { return GetActiveSprite(block); }

void setBlockSpriteCount(unsigned int *block, unsigned int nospr) { SetNumberOfSprites(block, nospr); }

unsigned int getBlockSpriteCount(unsigned int block) { return GetNumberOfSprites(block); }

void setBlockType(unsigned int *block, unsigned int type) { SetActiveShape(block, type); }

unsigned int getBlockType(unsigned int block) { return GetActiveShape(block); }

void InitialiseBlockDetails()
{
blds.size = 0;

// allocating a small bit of memory
blds.rids = malloc(sizeof(unsigned int) * 1024);
blds.blocks = malloc(sizeof(BLOCK) * 1024);
}

unsigned int getBlockCount() { return BLOCK_COUNT; }

int getBlockRenderIndex(unsigned int rid)
{
if(blds.size > rid)    // if the size is bigger than the ID then it is a valid ID
    if(blds.rids[rid] == rid)   // just in case no manipulation of the table has happened
        return rid;

for (int i = 0; i < blds.size; i++)  // simple linear search
    if(blds.rids[i] == rid)
        return i;
return -1;
}

BLOCK getBlockFromRenderID(unsigned int rid) { return blds.blocks[getBlockRenderIndex(rid)]; }

void AssignBlock(unsigned int rid, BLOCK block)
{
const unsigned int n = blds.size;

// make all the arrays bigger by one to accomodate for the new element
/*
ExpandByOne(&blds.rids, n, sizeof(unsigned int));
ExpandByOne(&blds.blocks, n, sizeof(BLOCK));
*/

// setting all the new details
blds.rids[n] = rid;
blds.blocks[n] = block;

blds.size++;    // increase the number of blocks
}

void UnassignBlock(unsigned int rid)
{
int index = getBlockRenderIndex(rid); // finding the ID
const unsigned int n = blds.size;

if(index == -1)
    return; // if the index isn't found just quit

if(index == blds.size - 1) goto end;   // hehe the naughty goto

// getting temporary stuff
unsigned int tmpid = blds.rids[index];
unsigned int tbl = blds.blocks[index];

// setting the to delete to the end values
blds.rids[index] = blds.rids[blds.size - 1];
blds.blocks[index] = blds.blocks[blds.size - 1];

// setting the end to the thing to delete
blds.rids[blds.size - 1] = tmpid;
blds.blocks[blds.size - 1] = tbl;

end:
blds.size--;    // decrease the size so it is effectively not there

// ShrinkArrayByOne(&blds.rids, n, sizeof(unsigned int));

/*
BLOCK* tmp = (BLOCK*)malloc((n - 1) * sizeof(BLOCK));
memcpy(tmp, blds.blocks, (n - 1) * sizeof(BLOCK));
blds.blocks = tmp;
*/

// blds.blocks = realloc(blds.blocks, (n - 1) * sizeof(BLOCK));

// blds.blocks = realloc(blds.blocks, (n - 1) * sizeof(BLOCK));
// ShrinkArrayByOne(&blds.blocks, n, sizeof(BLOCK));
// ShrinkArrayByOne(&(blds) + n * sizeof(unsigned int), n, sizeof(BLOCK));

// To-Do: Could add in a sort here to sort by ID in order to realign the table
}

BlockInfo getBlockInfo(BLOCK block)
{
switch (block)
    {
    case BLOCK_PLAYER:
        return (BlockInfo){"res/sprites/player_spritesheet.png", 2, 1};
        break;
    case BLOCK_MOVABLE_BARRIER:
        return (BlockInfo){"res/sprites/movable_barrier_tilesheet.png", 1, 1};
        break;
    case BLOCK_MOVABLE_BLOCK:
        return (BlockInfo){"res/sprites/movable_spritesheet_short.png", 2, 1};
        break;
    case BLOCK_IMMOVABLE_BLOCK:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 1};
        break;
    case BLOCK_COUNTABLE_BLOCK:
        return (BlockInfo){"res/sprites/countable_movable_spritesheet_short.png", 3, 1};
        break;
    case BLOCK_MOVABLE_DESTINATION:
        return (BlockInfo){"res/sprites/movable_spritesheet_short.png", 2, 2};
        break;
    case BLOCK_PLAYER_DESTINATION:
        return (BlockInfo){"res/sprites/player_spritesheet.png", 2, 2};
        break;
    default:
        break;
    }
return (BlockInfo){NULL};
}

BlockInfo getImmovableBlock(BLOCK_IM_STATE state)
{
// To-Do: This could be changed to just use the state as the parameter as it counts from 1 to 6
switch (state)
    {
    case BLOCK_IM_STATE_ALONE:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 1};
        break;
    case BLOCK_IM_STATE_LINE_END:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 2};
        break;
    case BLOCK_IM_STATE_LINE_STRAIGHT:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 3};
        break;
    case BLOCK_IM_STATE_THREE_INTERSECT:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 4};
        break;
    case BLOCK_IM_STATE_FOUR_INTERSECT:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 5};
        break;
    case BLOCK_IM_STATE_CORNER:
        return (BlockInfo){"res/sprites/immovable_tilesheet_short.png", 6, 6};
        break;
    default:
        return (BlockInfo){NULL};
        break;
    }
}

int getSpriteCount(BLOCK block) { return getBlockInfo(block).nosp; }

BLOCK getBlockFromFilePath(const char* fp)
{
for (int i = 0; i < getBlockCount(); i++)
    {
    if(strcmp(getBlockInfo(i).spfp, fp) == 0)   // if they are the same file path they have the same base block
        return (BLOCK)i;
    }

return -1;
}

BLOCK getBlockFromDetails(const char* spfp, unsigned int nosp, unsigned int spr)
{
for (int i = 0; i < getBlockCount(); i++)
    {
    BlockInfo bi = getBlockInfo((BLOCK)i);

    if(strcmp(bi.spfp, spfp) != 0)  // if the file path isn't correct
        continue;

    if(bi.nosp != nosp) // if the number of sprites is incorrect
        continue;

    if(bi.spr == spr)   // if there is the correct sprite
        return (BLOCK)i;
    }

return -1;
}