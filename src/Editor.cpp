#include "Editor.hpp"

const int snap_to_grid = 1;
const int grid_size = 50;

extern UI_Manager ui_man;

#pragma region Functions

BLOCK_TYPE getActiveBlock(const Editor& ed) { return ed.actblk; }

void setActiveBlock(Editor& ed, BLOCK_TYPE block) { ed.actblk = block; }

static unsigned int PlaceBlock(Block_Manager& blk_man, BLOCK_TYPE type, vec2 position)
{
Block blk(type, position);
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
ed.ui_man.checkUIInput();

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
        if(blk.update != nullptr)
            {
            blk.clicked = 1;    // say the block has been clicked
            blk.update(blk);    // do the click action
            blk.clicked = 0;    // say it has no longer been clicked
            }
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

void BuildSelectBar(const Block_Manager& blk_man, Editor& ed)
{
vec2 topright = {1255.0f, 695.0f};
const float padding = 10.0f;

UI_Manager& ui_man = ed.ui_man;

BLOCK_TYPE menuDetails[] =   // the base blocks
    {
    BLOCK_PLAYER,
    BLOCK_MOVABLE_BARRIER,
    BLOCK_MOVABLE_BLOCK,
    BLOCK_IMMOVABLE_BLOCK,
    BLOCK_COUNTABLE_BLOCK,
    BLOCK_TELEPORTER_SOURCE
    };
const int nblk = sizeof(menuDetails) / sizeof(BLOCK_TYPE);

auto chblk_fun = [&ed](UI_Element& ele)
        {
        printf("\nChanging active block");
        ed.actblk = getBlockTypeFromSSI(ele.ssi);
        printf("\n%d Active", (int)ed.actblk);
        };

auto mentog_fun = [&ed](UI_Element& ele)
    {
    if(ele.hoveract && !ele.hovering)
        foldMenu(ed.ui_man, ele);
    else if(ele.hoveract && ele.hovering)
        unfoldMenu(ed.ui_man, ele);
    };

for (int i = 0; i < nblk; i++)
    {
    vec2 position = {topright.x, topright.y - (i * 50.0f + padding)}; // placing the items in a vertical line on the right side of the screen
    SpriteSheetInfo ssi = getBlockSSI(menuDetails[i]);
    UI_Element btn = createButton(position, 25.0f, ssi.spfp, ssi.nosp, ssi.spr);
    btn.clickable = 1;
    btn.onclick = chblk_fun;

    ui_man.addNewElement(btn);

    BLOCK_TYPE btype = getBlockTypeFromSSI(btn.ssi);

    if(ssi.nosp > 1 && btype != BLOCK_TYPE::BLOCK_IMMOVABLE_BLOCK)
        {
        UI_Element menu = createMenu(position);
        addToMenu(menu, btn);
        menu.hoveract = 1;
        menu.lrud = 0;
        menu.update = mentog_fun;
        
        for (int j = 2; j <= ssi.nosp; j++)
            {
            vec2 tpos = {position.x - ((j - 1) * 50.0f + padding), position.y};
            SpriteSheetInfo tssi = (SpriteSheetInfo){ssi.spfp, ssi.nosp, (unsigned int)j};
            UI_Element tbtn = createButton(tpos, 25.0f, tssi.spfp, tssi.nosp, tssi.spr);

            tbtn.clickable = 1;
            tbtn.onclick = chblk_fun;
            
            addToMenu(menu, tbtn);
            ed.ui_man.addNewElement(tbtn);
            }
        ed.ui_man.addNewElement(menu);
        foldMenu(ed.ui_man, menu);
        }
    /*
    ui_man.addNewElement(btn);
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
    */
    }
}

#pragma endregion