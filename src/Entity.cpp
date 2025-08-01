#include "Entity.hpp"

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

Block::Block(BLOCK_TYPE btype, vec2 position)
    : pos(pos), type(btype)
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