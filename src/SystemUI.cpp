#include "SystemUI.hpp"

/*
extern Character_Table ch_tab;
extern Text_Table txt_tab;
extern RenderPacket text_rp;
*/

const int datsize = 256;

UI_Element createButton(vec2 pos, float scale, const char* spfp, int nosp, int spr)
{
return UI_Element(UI_ELEMENT_TYPE::UI_BUTTON, pos, spfp, nosp, spr);
}

UI_Element createMenu(vec2 pos)
{
return UI_Element(UI_ELEMENT_TYPE::UI_MENU, pos);
}

void addToMenu(UI_Element& menu, const UI_Element& entry) { menu.entries.push_back(entry.ui_id); }

void foldMenu(UI_Manager& ui_man, UI_Element& menu)
{
menu.folded = 1;
for (int i = 1; i < menu.entries.size(); i++)
    {
    UI_Element& ele = ui_man.getElement(menu.entries[i]);
    ele.render = 0;
    }
}

void unfoldMenu(UI_Manager& ui_man, UI_Element& menu)
{
menu.folded = 0;
for (int i = 1; i < menu.entries.size(); i++)
    {
    UI_Element& ele = ui_man.getElement(menu.entries[i]);
    ele.render = 1;
    }
}

UI_Element createMenu(UI_Manager& ui_man, vec2 pos)
{
UI_Element menu(UI_ELEMENT_TYPE::UI_MENU, pos);
menu.hoveract = 1;
menu.update = [&ui_man](UI_Element& ele)
    {
    if(ele.hoveract && !ele.hovering)
        foldMenu(ui_man, ele);
    else if(ele.hoveract && ele.hovering)
        unfoldMenu(ui_man, ele);
    };

return menu;
}

void Example(UI_Manager& ui_man)
{
UI_Element btn(UI_ELEMENT_TYPE::UI_BUTTON, {500.0f, 500.0f}, "res/sprites/player_spritesheet.png", 2, 1);
UI_Element btn2(UI_ELEMENT_TYPE::UI_BUTTON, {500.0f, 450.0f}, "res/sprites/player_spritesheet.png", 2, 2);

UI_Element menu(UI_ELEMENT_TYPE::UI_MENU, {500.0f, 500.0f});
addToMenu(menu, btn);
addToMenu(menu, btn2);

menu.hoveract = 1;
menu.update = [&ui_man](UI_Element& ele)
    {
    if(ele.hoveract && !ele.hovering)
        foldMenu(ui_man, ele);
    else if(ele.hoveract && ele.hovering)
        unfoldMenu(ui_man, ele);
    };

menu.lrud = 3;
ui_man.addNewElement(btn);
ui_man.addNewElement(btn2);
ui_man.addNewElement(menu);


foldMenu(ui_man, menu);
}
