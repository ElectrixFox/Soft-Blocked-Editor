#include "Block.h"

static BlockDetails blds;

void setBlockSprite(unsigned long int* block, unsigned int spr) { SetActiveSprite(block, spr); }

unsigned int getBlockSprite(unsigned long int block) { return GetActiveSprite(block); }

void setBlockSpriteCount(unsigned long int *block, unsigned int nospr) { SetNumberOfSprites(block, nospr); }

unsigned int getBlockSpriteCount(unsigned long int block) { return GetNumberOfSprites(block); }

void setBlockType(unsigned long int *block, unsigned int type) { SetActiveShape(block, type); }

BLOCK getBlockType(BLOCK block) { return (BLOCK)GetActiveShape((unsigned long int)block); }


BLOCK getBlockType(unsigned int in_bl_id)
{
for (int i = 0; i < blds.bl_id.size(); i++)
    if(blds.bl_id[i] == in_bl_id)
        return blds.blocks[i];
printf("\nERROR: Could not find block type");
exit(1);
}

unsigned int getBlockRenderID(unsigned int in_bl_id)
{
for (int i = 0; i < blds.bl_id.size(); i++)
    if(blds.bl_id[i] == in_bl_id)
        return blds.rids[i];
printf("\nERROR: Could not find block render ID");
exit(1);
}

unsigned int getBlockIDFromRenderID(unsigned int rid)
{
for (int i = 0; i < blds.rids.size(); i++)
    if(blds.rids[i] == rid)
        return blds.bl_id[i];
printf("\nERROR: Could not find block ID");
exit(1);
}

void InitialiseBlockDetails()
{

}

unsigned int getBlockCount() { return BLOCK_COUNT; }

int getBlockRenderIndex(unsigned int rid)
{
const int n = blds.rids.size();
if(n > rid)    // if the size is bigger than the ID then it is a valid ID
    if(blds.rids[rid] == rid)   // just in case no manipulation of the table has happened
        return rid;

for (int i = 0; i < n; i++)  // simple linear search
    if(blds.rids[i] == rid)
        return i;
return -1;
}

BLOCK getBlockFromRenderID(unsigned int rid) { return blds.blocks[getBlockRenderIndex(rid)]; }

unsigned int AssignBlock(unsigned int rid, BLOCK block)
{
static unsigned int bl_id = 0;

// setting all the new details
blds.rids.push_back(rid);
blds.blocks.push_back(block);
blds.bl_id.push_back(bl_id++);

return bl_id - 1;
}

void UnassignBlock(unsigned int rid)
{
int index = getBlockRenderIndex(rid); // finding the ID

if(index == -1)
    return; // if the index isn't found just quit

blds.rids.erase(blds.rids.begin() + index);
blds.blocks.erase(blds.blocks.begin() + index);
blds.bl_id.erase(blds.bl_id.begin() + index);
}

BlockInfo getBlockInfo(BLOCK block)
{
switch (block)
    {
    case BLOCK_PLAYER:
        return (BlockInfo){"res/sprites/player_spritesheet.png", 2, 1};
        break;
    case BLOCK_MOVABLE_BARRIER:
        return (BlockInfo){"res/sprites/barriers_tilesheet_short.png", 2, 1};
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
    case BLOCK_COUNTABLE_DESTINATION:
        return (BlockInfo){"res/sprites/countable_movable_spritesheet_short.png", 3, 2};
        break;
    case BLOCK_PLAYER_BARRIER:
        return (BlockInfo){"res/sprites/barriers_tilesheet_short.png", 2, 2};
        break;
    case BLOCK_TELEPORTER_SOURCE:
        return (BlockInfo){"res/sprites/teleporter_tilesheet.png", 2, 1};
        break;
    case BLOCK_TELEPORTER_DESTINATION:
        return (BlockInfo){"res/sprites/teleporter_tilesheet.png", 2, 2};
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
    if(strcmp(getBlockInfo((BLOCK)i).spfp, fp) == 0)   // if they are the same file path they have the same base block
        return (BLOCK)i;
    }

return (BLOCK)-1;
}

BLOCK getBlockFromDetails(const char* spfp, unsigned int nosp, unsigned int spr)
{
if(getBlockFromFilePath(spfp) == BLOCK::BLOCK_IMMOVABLE_BLOCK)  // check if it is immovable here as they don't get picked up too well
    return BLOCK::BLOCK_IMMOVABLE_BLOCK;

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

return (BLOCK)-1;
}