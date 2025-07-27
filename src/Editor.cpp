#include "Editor.h"

static BlockInfo cblock = {"res/sprites/player_spritesheet.png", 2, 1};

const int snap_to_grid = 1;
const int grid_size = 50;

// extern RenderPacket ui_rp;
// extern UI_Table ui;

extern UI_Manager ui_man;
/*
extern UI_Table ui;
extern RenderPacket ui_rp;
*/

#pragma region Main

BlockInfo getActiveBlock() { return cblock; }

void setActiveBlock(BlockInfo block) { cblock = block; }

void SelectBlock(Drawables drabs, unsigned int trsid)
{
/*
int index = findDrawablesTransform(drabs, trsid); // find the drawable from the transform
setActiveBlock(getBlockFromRenderID(drabs.rids[index]));    // sets the active block
*/
}

void ApplyCamera(Camera cam, RenderDetails rds) { _ApplyCamera(cam, rds.shader); }

void ClearCamera(RenderDetails rds)
{
for (int i = 0; i < rds.rid.size(); i++)
    {
    SetUniformM4(rds.shader[i], "view", getM4ID());
    }
}

void ApplyProjection(Camera cam, RenderDetails rds) { _ApplyProjection(cam, rds.shader); }

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