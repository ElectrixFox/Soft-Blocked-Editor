#include "Entity.hpp"

static unsigned int id = 0;

SpriteSheetInfo getBlockSSI(BLOCK_TYPE btype)
{
switch (btype)
    {
    case BLOCK_PLAYER:
        return (SpriteSheetInfo){"res/sprites/player_spritesheet.png", 2, 1};
        break;
    case BLOCK_MOVABLE_BARRIER:
        return (SpriteSheetInfo){"res/sprites/barriers_tilesheet_short.png", 2, 1};
        break;
    case BLOCK_MOVABLE_BLOCK:
        return (SpriteSheetInfo){"res/sprites/movable_spritesheet_short.png", 2, 1};
        break;
    case BLOCK_IMMOVABLE_BLOCK:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 1};
        break;
    case BLOCK_COUNTABLE_BLOCK:
        return (SpriteSheetInfo){"res/sprites/countable_movable_spritesheet_short.png", 3, 1};
        break;
    case BLOCK_MOVABLE_DESTINATION:
        return (SpriteSheetInfo){"res/sprites/movable_spritesheet_short.png", 2, 2};
        break;
    case BLOCK_PLAYER_DESTINATION:
        return (SpriteSheetInfo){"res/sprites/player_spritesheet.png", 2, 2};
        break;
    case BLOCK_COUNTABLE_DESTINATION:
        return (SpriteSheetInfo){"res/sprites/countable_movable_spritesheet_short.png", 3, 2};
        break;
    case BLOCK_PLAYER_BARRIER:
        return (SpriteSheetInfo){"res/sprites/barriers_tilesheet_short.png", 2, 2};
        break;
    case BLOCK_TELEPORTER_SOURCE:
        return (SpriteSheetInfo){"res/sprites/teleporter_tilesheet.png", 2, 1};
        break;
    case BLOCK_TELEPORTER_DESTINATION:
        return (SpriteSheetInfo){"res/sprites/teleporter_tilesheet.png", 2, 2};
        break;
    default:
        printf("\nERROR: Incorrect block type");
        exit(1);
        break;
    }
}

SpriteSheetInfo getImmovableBlock(BLOCK_IM_STATE state)
{
// To-Do: This could be changed to just use the state as the parameter as it counts from 1 to 6
switch (state)
    {
    case BLOCK_IM_STATE_ALONE:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 1};
        break;
    case BLOCK_IM_STATE_LINE_END:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 2};
        break;
    case BLOCK_IM_STATE_LINE_STRAIGHT:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 3};
        break;
    case BLOCK_IM_STATE_THREE_INTERSECT:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 4};
        break;
    case BLOCK_IM_STATE_FOUR_INTERSECT:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 5};
        break;
    case BLOCK_IM_STATE_CORNER:
        return (SpriteSheetInfo){"res/sprites/immovable_tilesheet_short.png", 6, 6};
        break;
    default:
        return (SpriteSheetInfo){};
        break;
    }
}

Block::Block(BLOCK_TYPE btype, vec2 position)
    : type(btype), bl_id(id++), pos(position)
{
this->ssi = getBlockSSI(type);
this->rend_obj = RenderObject(this->ssi, 0);
scale = {25.0f, 25.0f};
angle = 0.0f;
}

void Block::draw()
{
switch (type)
    {
    case BLOCK_TYPE::BLOCK_TELEPORTER_SOURCE:
        {
        break;
        }
    default:
        {
        drawRenderObject(rend_obj, pos, scale, angle);
        break;
        }
    }
}

void Block_Manager::drawBlocks(Camera cam)
{
for(Block block : blocks)
    {
    ApplyCamera(cam, block.rend_obj.prog);
    ApplyProjection(cam, block.rend_obj.prog);
    block.draw();
    }
}
        
void Block_Manager::addNewBlock(Block blk)
{
blocks.push_back(blk);
}

Block& Block_Manager::getBlock(unsigned int bl_id)
{
for (Block& blk : blocks)
    if(blk.bl_id == bl_id)
        return blk;

printf("\nERROR: Cannot find block %d", bl_id);
exit(1);
}

int Block_Manager::isBlockAt(vec2 position) const
{
for (const Block& blk : blocks)
    if(position == blk.pos)
        return 1;
return 0;
}

const Block& Block_Manager::getBlockAt(vec2 position) const
{
for (const Block& blk : blocks)
    if(position == blk.pos)
        return blk;

printf("\nERROR: Cannot find block at ");
OutputVec2(position);
exit(1);
}

        
Block& Block_Manager::getBlockAt(vec2 position)
{
if(isBlockAt(position))    // if there is not a block at the position
    for (Block& blk : blocks)
        if(blk.pos == position)
            return blk;

printf("\nERROR: Cannot find block at ");
OutputVec2(position);
exit(1);

}