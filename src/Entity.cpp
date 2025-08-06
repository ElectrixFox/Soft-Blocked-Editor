#include "Entity.hpp"

static unsigned int id = 0;

#pragma region Blocks

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

int Block_Manager::findBlockIndex(const Block& blk)
{
for (int i = 0; i < blocks.size(); i++)
    if(blocks[i].bl_id == blk.bl_id)
        return i;

return -1;
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

void Block_Manager::removeBlock(unsigned int bl_id)
{
int find = findBlockIndex(getBlock(bl_id));
if(find == -1)
    {
    printf("\nERROR: Could not find and delete the block %d", bl_id);
    exit(1);
    }

blocks.erase(blocks.begin() + find);
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

int Block_Manager::hasPressedBlock(vec2 pos) const { return isBlockAt(pos); }

#pragma endregion

#pragma region UI

static unsigned int id2 = 0;

static int shouldRender(UI_ELEMENT_TYPE type)
{
return 1;
}

UI_Element::UI_Element(UI_ELEMENT_TYPE in_type, vec2 position)
    : type(in_type), ui_id(id2++), pos(position)
{
int rend = shouldRender(type);  // should this be rendered

this->render = rend;    // setting the variable

if(rend == 1)
    {
    this->rend_obj = RenderObject(this->ssi, 0);
    }

scale = {25.0f, 25.0f};
angle = 0.0f;
}

UI_Element::UI_Element(UI_ELEMENT_TYPE in_type, vec2 position, const char* spfp, int nosp, int spr)
    : type(in_type), ui_id(id2++), pos(position)
{
this->ssi = (SpriteSheetInfo){spfp, nosp, spr};

int rend = shouldRender(type);  // should this be rendered
this->render = rend;    // setting the variable

if(rend == 1)
    {
    this->rend_obj = RenderObject(this->ssi, 0);
    }

scale = {25.0f, 25.0f};
angle = 0.0f;
}

void UI_Element::draw()
{
if(!render)
    return;

drawRenderObject(rend_obj, pos, scale, angle);
}

void UI_Manager::drawElements(Camera cam)
{
for(UI_Element ele : elements)
    {
    if(ele.render == 0) // if shouldn't render then just continue
        continue;

    ApplyCamera(cam, ele.rend_obj.prog);
    ApplyProjection(cam, ele.rend_obj.prog);
    ele.draw();
    }
}
        
void UI_Manager::addNewElement(UI_Element ele) { elements.push_back(ele); }

UI_Element& UI_Manager::getElement(unsigned int ui_id)
{
for (UI_Element& ele : elements)
    if(ele.ui_id == ui_id)
        return ele;

printf("\nERROR: Cannot find UI element %d", ui_id);
exit(1);
}

int UI_Manager::hasPressedUI(vec2 cpos) const
{
for (const UI_Element& ele : elements)  // for each element
    if(cpos == ele.pos) // if the positions match
        return 1;

return 0;
}

#pragma endregion

