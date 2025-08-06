#include "Editor.hpp"

const int snap_to_grid = 1;
const int grid_size = 50;

extern UI_Manager ui_man;

#pragma region Functions

BLOCK_TYPE getActiveBlock(const Editor& ed) { return ed.actblk; }

void setActiveBlock(Editor& ed, BLOCK_TYPE block) { ed.actblk = block; }

static unsigned int PlaceBlock(Block_Manager& blk_man, BLOCK_TYPE type, vec2 position)
{
Block blk((BLOCK_TYPE)(type), position);
if(blk.type == BLOCK_TYPE::BLOCK_IMMOVABLE_BLOCK)
    {
    blk.update = [](Block& blk)
        {
        printf("\nUpdating");
        blk.rend_obj = RenderObject(blk.ssi);
        };
    }
blk_man.addNewBlock(blk);
return blk.bl_id;
}

void CheckEditorInput(Editor& ed, Block_Manager& blk_man, Camera& cam)
{
GLFWwindow* window = getWindow();

// keyboard actions
if(isPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(window, 1);
else if(isPressedSingle(GLFW_KEY_FULL_STOP))    // not needed as the blocks now automatically update themselves
    {
    // UpdateImmovableBlocks(block_rp, w, h, (const int**)grid);
    }
else if(isHeldDown(GLFW_KEY_LEFT_CONTROL) && isPressedSingle(GLFW_KEY_S))
    {
    printf("\nSaving");
    int** grid;
    int w, h;
    getLevel(blk_man, &w, &h, &grid);
    WriteLevel("res/levels/level3.txt", w, h, (const int**)grid);
    }
else if(isPressedSingle(GLFW_KEY_TAB))
    {
    /*
    OutputRenderPacketDetails(block_rp);
    OutputRenderPacketDetails(ui_rp);

    getLevel(block_rp, &w, &h, &grid);
    OutputLevel((const int**)grid, w, h);
    */
    }

// mouse actions
if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
    vec2 cpos = GetCursorPositionRelative(cam);
    vec2 ncpos = getCursorPosition();
    cpos.x = grid_size * roundf(cpos.x / grid_size);
    cpos.y = grid_size * roundf(cpos.y / grid_size);

    if(!blk_man.hasPressedBlock(cpos) && !ed.ui_man.hasPressedUI(ncpos))
        {
        PlaceBlock(blk_man, getActiveBlock(ed), cpos);  // placing the block
        UpdateImmovableBlocks(blk_man);
        }
    else if(blk_man.hasPressedBlock(cpos))    // if a block has been clicked
        {
        Block& blk = blk_man.getBlockAt(cpos);  // get the block
        blk.clicked = 1;    // say the block has been clicked
        blk.update(blk);    // do the click action
        blk.clicked = 0;    // say it has no longer been clicked
        }
    }
else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
    vec2 cpos = GetCursorPositionRelative(cam);
    cpos.x = grid_size * roundf(cpos.x / grid_size);
    cpos.y = grid_size * roundf(cpos.y / grid_size);

    if(blk_man.hasPressedBlock(cpos))
        {
        Block& blk = blk_man.getBlockAt(cpos);
        blk_man.removeBlock(blk.bl_id);
        UpdateImmovableBlocks(blk_man);
        }
    }


}

#pragma endregion

#pragma region EditorUI

#ifdef EDITOR_UI

static void changeBlock(int ui_id)
{
GUI_Button btn = getButton(ui_man.ui_btn_tab, ui_id);
SpriteSheetInfo ssi = btn.ssi;
BlockInfo bi = { ssi.spfp, ssi.nosp, ssi.spr };
setActiveBlock(bi);  // sets the active block
}

static void unfoldBlockOptions(int ui_id)
{
static int prevuid = -1;

if(prevuid != ui_id) // if the previous ID isn't the menu to unfold and the menu is folded
    {
    if(prevuid != -1)
        {
        printf("\nFolding %d", prevuid);
        foldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, prevuid);
        }

    printf("\nUnfolding %d", ui_id);
    unfoldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, ui_id);
    
    /*
    GUI_MENU meni = getUIRenderInformation(ui, ui_id).meni;   // getting the render information
    unsigned int head_id = meni.men_head_ui_id;
    
    SpriteSheetInfo ssi = getUIRenderInformation(ui, head_id).ssi;  // getting the sprite sheet info about the head
    BLOCK block = getBlockFromFilePath(ssi.spfp);   // getting the block
    BlockInfo bi = getBlockInfo(block);

    for (int i = 2; i <= bi.nosp; i++)
        {
        RenderInformation ri = SpriteSheetInfo(bi.spfp, bi.nosp, (unsigned int)i);
        unsigned int menentry = addToMenu(ui, ui_rp, ui_id, ri.ssi);
        assignButtonAction(ui, menentry, UI_TRIGGER_PRESS, &changeBlock);
        }
    */

    prevuid = ui_id;
    }
}

void BuildSelectBar()
{
vec2 topright = {1255.0f, 695.0f};
const unsigned int nblocks = getBlockCount();
const float padding = 10.0f;

BLOCK menuDetails[] =   // the base blocks
    {
    BLOCK_PLAYER,
    BLOCK_MOVABLE_BARRIER,
    BLOCK_MOVABLE_BLOCK,
    BLOCK_IMMOVABLE_BLOCK,
    BLOCK_COUNTABLE_BLOCK,
    BLOCK_TELEPORTER_SOURCE
    };
const int nblk = sizeof(menuDetails) / sizeof(BLOCK);


for (int i = 0; i < nblk; i++)
    {
    vec2 position = {topright.x, topright.y - (i * 50.0f + padding)}; // placing the items in a vertical line on the right side of the screen
    BlockInfo bi = getBlockInfo(menuDetails[i]);
    SpriteSheetInfo ssi = {bi.spfp, bi.nosp, bi.spr};
    GUI_Button btn = createButton(position, 25.0f, ssi.spfp, ssi.nosp, ssi.spr);

    unsigned int entry = addToElementTable(ui_man, position, 25.0f, btn);
    assignElementAction(ui_man.ui_btn_tab, entry, (GUI_ACTION_TRIGGER)0, &changeBlock);

    if(ssi.nosp > 1 && getBlockFromFilePath(bi.spfp) != BLOCK_IMMOVABLE_BLOCK)   // if there is more than one sprite and the block isn't the immovable type
        {
        GUI_Menu menu = createMenu(position, entry);
        unsigned int men_id = addToElementTable(ui_man, position, menu);
        assignElementAction(ui_man.ui_men_tab, men_id, UI_TRIGGER_HOVER, &unfoldBlockOptions);

        printf("\nCreating the Menu");
        for (int j = 2; j <= bi.nosp; j++)
            {
            vec2 tpos = {position.x - ((j - 1) * 50.0f + padding), position.y};
            SpriteSheetInfo tssi = SpriteSheetInfo(bi.spfp, bi.nosp, (unsigned int)j);
            GUI_Button tbtn = createButton(tpos, 25.0f, tssi.spfp, tssi.nosp, tssi.spr);
            unsigned int menentry = addToElementTable(ui_man, tpos, 25.0f, tbtn);
            assignElementAction(ui_man.ui_btn_tab, menentry, UI_TRIGGER_PRESS, &changeBlock);
            addToMenu(ui_man.ui_men_tab, men_id, menentry);
            }
        foldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, men_id);
        }
    }
}

#endif

#pragma endregion