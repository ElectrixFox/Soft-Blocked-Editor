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

#ifdef OLD

#pragma region Local Functions

template<typename T>
static int findUIInUITable(const UI_Element_Table<T>& table, unsigned int ui_id)
{
int index = -1;

for (int i = 0; i < table.ui_id.size(); i++)    // loop through the table
    if(table.ui_id[i] == ui_id) // if find the ID then set index equal to i
        {
        index = i;
        break;
        }

if(index == -1) // if ID not found
    {
    printf("\nError: Could not find %d in the UI table", ui_id);
    exit(1);
    }

return index;
}

template<typename T>
static T& getUIElement(UI_Element_Table<T>& table, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);
return table.data[index];
}

template<typename T>
static void _assignElementAction(UI_Element_Table<T>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIInUITable(table, ui_id);
assignUITriggerAction(table.actions[trigger], ui_id, action);
}

static int pressedInRectangle(vec2 pos, vec2 scale)
{
vec2 cpos = getCursorPosition();
return PointInSquare(cpos, pos, scale);
}

template <typename T>
static int isCursorOnUIElement(UI_Element_Table<T> table, RenderPacket rp, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);
if(findDrawablesTransform(rp.drabs, table.trsid[index]) == -1)  // if cannot find the transform in the drawables then it has not been drawn and shouldn't be checked
    return 0;
vec2 pos = getPosition(rp.tds, table.trsid[index]);
vec2 scale = getScale(rp.tds, table.trsid[index]);
return pressedInRectangle(pos, scale);
}

template <>
int isCursorOnUIElement<GUI_Text_Box>(UI_Element_Table<GUI_Text_Box> table, RenderPacket rp, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);
int len = table.data[index].cont.length() * 2;  // getting the string length
if(findDrawablesTransform(rp.drabs, table.trsid[index]) == -1)  // if cannot find the transform in the drawables then it has not been drawn and shouldn't be checked
    return 0;
vec2 pos = getPosition(rp.tds, table.trsid[index]);
vec2 scale = getScale(rp.tds, table.trsid[index]);
scale.x *= len; // stretching the scale along the x to include all of the letters
return pressedInRectangle(pos, scale);
}

template <typename T>
static void _checkUI(UI_Element_Table<T>& table, RenderPacket rp)
{
GLFWwindow* window = getWindow();

for (int i = 0; i < table.ui_id.size(); i++)
    {
    int index = findUITriggerActionIDinTable(table.actions[UI_TRIGGER_HOVER], table.ui_id[i]);
    if(index == -1) // if there is no hover action then skip
        continue;
    
    if(isCursorOnUIElement(table, rp, table.ui_id[i]))
        {
        // printf("\nPerforming hover action for %d", ui.ui_id[i]);
        table.actions[UI_TRIGGER_HOVER].action[index](table.ui_id[i]);
        }
    }


for (int i = 0; i < table.ui_id.size(); i++)
    {
    int index = findUITriggerActionIDinTable(table.actions[UI_TRIGGER_LEAVE_HOVER], table.ui_id[i]);
    if(index == -1) // if there is no hover action then skip
        continue;
    
    if(!isCursorOnUIElement(table, rp, table.ui_id[i]))
        {
        // printf("\nPerforming hover action for %d", ui.ui_id[i]);
        table.actions[UI_TRIGGER_LEAVE_HOVER].action[index](table.ui_id[i]);
        }
    }

if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
    for (int i = 0; i < table.ui_id.size(); i++)
        {
        int index = findUITriggerActionIDinTable(table.actions[UI_TRIGGER_PRESS], table.ui_id[i]);
        if(index == -1) // if there is no hover action then skip
            continue;
        
        if(isCursorOnUIElement(table, rp, table.ui_id[i]))
            {
            printf("\nPerforming action for %d", table.ui_id[i]);
            table.actions[UI_TRIGGER_PRESS].action[index](table.ui_id[i]);
            }
        }
    }
}

#pragma endregion

#pragma region ID Management

unsigned int setUIID(unsigned int id, unsigned int type)
{
unsigned int mask = 0b1111000000000000U;    // the type mask
unsigned int ui_id = id | (mask & (type << 12));    // pushing the type back and adding in the ID
return ui_id;
}

unsigned int getUIID(unsigned int ui_id)
{
unsigned int mask = ~0b1111000000000000U;    // the type mask
return (ui_id & mask);
}

unsigned int getUIType(unsigned int ui_id)
{
unsigned int mask = 0b1111000000000000U;    // the type mask
return (ui_id & mask) >> 12;
}

#pragma endregion

#pragma region Creating Elements

GUI_Button createButton(vec2 pos, float scale, const char* spfp, int nosp, int spr)
{
GUI_Button button {
    .ssi = {spfp, nosp, spr}
};
return button;
}

GUI_Menu createMenu(vec2 pos, unsigned int head_id)
{
GUI_Menu menu {
    .ui_ids = {},
    .men_head_ui_id = head_id
};
return menu;
}

GUI_Text_Box createTextBox(vec2 pos, std::string cont)
{
GUI_Text_Box box {
    .cont = cont
};
return box;
}

#pragma endregion

#pragma region Adding Elements

static unsigned int getNextID(unsigned int type)
{
unsigned int ui_id = setUIID(idtrk, type);
idtrk++;
return ui_id;
}

unsigned int addToElementTable(UI_Element_Table<GUI_Button>& table, RenderPacket& rp, vec2 pos, float scale, GUI_Button button)
{
unsigned int ui_id = getNextID(1);

unsigned int rid = CreateSpriteRenderable(rp.rds, button.ssi.spfp, button.ssi.nosp, button.ssi.spr);
unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);
int ind = AddDrawable(rp.drabs, trsid, rid);

table.ui_id.push_back(ui_id);   // adding the ID
table.trsid.push_back(trsid);   // adding the transform
table.data.push_back(button);   // adding the actual button

return ui_id;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, float scale, GUI_Button button)
{
UI_Element_Table<GUI_Button>& table = ui_man.ui_btn_tab;
return addToElementTable(table, ui_man.ui_rp, pos, scale, button);
}

unsigned int addToElementTable(UI_Element_Table<GUI_Menu>& table, const UI_Element_Table<GUI_Button>& btab, RenderPacket& rp, vec2 pos, GUI_Menu menu)
{
unsigned int ui_id = getNextID(2);
unsigned int trsid;

for(int i = 0; i < btab.ui_id.size(); i++)  // loop through the button table
    if(btab.ui_id[i] == menu.men_head_ui_id)    // if the UI ID of the head is equal to the button table then set it
        {
        trsid = btab.trsid[i];
        break;  // shall not need to continue looping
        }


table.ui_id.push_back(ui_id);   // adding the ID
table.trsid.push_back(trsid);   // adding the transform of the head
table.data.push_back(menu);     // adding the menu

return ui_id;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Menu menu)
{
UI_Element_Table<GUI_Button>& btab = ui_man.ui_btn_tab;
UI_Element_Table<GUI_Menu>& table = ui_man.ui_men_tab;

return addToElementTable(table, btab, ui_man.ui_rp, pos, menu);
}

unsigned int addToElementTable(UI_Element_Table<GUI_Text_Box>& table, const Character_Table& ch_tab, RenderPacket& rp, vec2 pos, GUI_Text_Box txbx)
{
unsigned int ui_id = getNextID(3);
const float scale = 25.0f;

unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);

for (char c : txbx.cont)  // for each character in the string
    {
    unsigned int rid = findCharacterRenderID(ch_tab, c);    // getting the render ID
    AddDrawable(rp.drabs, trsid, rid); // creating the new drawable
    }

table.ui_id.push_back(ui_id);   // adding the ID
table.trsid.push_back(trsid);   // adding the transform
table.data.push_back(txbx);   // adding the actual button

return ui_id;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Text_Box txbx)
{
UI_Element_Table<GUI_Text_Box>& table = ui_man.ui_text_box_tab;
return addToElementTable(table, ui_man.ch_tab, ui_man.ui_rp, pos, txbx);
}

#pragma endregion

GUI_Button& getButton(UI_Element_Table<GUI_Button>& table, unsigned int ui_id) { return getUIElement(table, ui_id); }
GUI_Menu& getMenu(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id) { return getUIElement(table, ui_id); }
GUI_Text_Box& getTextBox(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id) { return getUIElement(table, ui_id); }

void assignElementAction(UI_Element_Table<GUI_Button>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action) { _assignElementAction(table, ui_id, trigger, action); }
void assignElementAction(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action) { _assignElementAction(table, ui_id, trigger, action); }
void assignElementAction(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action) { _assignElementAction(table, ui_id, trigger, action); }

void UI_Manager::checkUI()
{
_checkUI(ui_btn_tab, ui_rp);
_checkUI(ui_men_tab, ui_rp);
_checkUI(ui_text_box_tab, ui_rp);
}

#pragma region Menu

void addToMenu(UI_Element_Table<GUI_Menu>& men_tab, unsigned int men_id, unsigned int ui_id)
{
int index = findUIInUITable(men_tab, men_id);   // finding the menu
men_tab.data[index].ui_ids.push_back(ui_id);
}

template<typename T>
static vec2 getUIElementPosition(UI_Element_Table<T> table, RenderPacket rp, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);  // finding the element
return getPosition(rp.tds, table.trsid[index]); // getting the position
}

template<typename T>
static void setUIElementPosition(UI_Element_Table<T> table, RenderPacket& rp, unsigned int ui_id, vec2 npos)
{
int index = findUIInUITable(table, ui_id);  // finding the element
setPosition(rp.tds, table.trsid[index], npos);  // setting the position
}

void addToMenu(UI_Manager& ui_man, unsigned int men_id, unsigned int ui_id, int fittomenu, int valign)
{
const float spacing = 50.0f;

int index = findUIInUITable(ui_man.ui_men_tab, men_id);   // finding the menu
ui_man.ui_men_tab.data[index].ui_ids.push_back(ui_id);  // adding the new element to the menu

if(fittomenu == 1)
    {
    vec2 headpos = getPosition(ui_man.ui_rp.tds, ui_man.ui_men_tab.trsid[index]);   // getting the head position
    vec2 npos = headpos;
    printf("\nHead position: ");
    OutputVec2(npos);

    vec2 offset = (valign == 1) ? (vec2){0.0f, -spacing} : (vec2){spacing, 0.0f};   // getting the offset

    for (unsigned int id : ui_man.ui_men_tab.data[index].ui_ids)
        {
        npos = npos + offset;

        std::vector<unsigned int> combuids = mergeVectors(ui_man.ui_btn_tab.ui_id, mergeVectors({}, ui_man.ui_text_box_tab.ui_id));
        std::vector<unsigned int> combtrsids = mergeVectors(ui_man.ui_btn_tab.trsid, mergeVectors({}, ui_man.ui_text_box_tab.trsid));

        for (int i = 0; i < combuids.size(); i++)
            if(combuids[i] == ui_id)
                {
                printf("\n%d", ui_id);
                if(getUIType(combuids[i]) == 3) // if the type is a button
                    {
                    printf("\nText");
                    }
                setPosition(ui_man.ui_rp.tds, combtrsids[i], npos);
                printf("\nSetting new position: ");
                OutputVec2(npos);
                break;
                }
        
        /*
        index = findUIInUITable(ui_man.ui_btn_tab, ui_id);  // finding the element
        setUIElementPosition(ui_man.ui_btn_tab, ui_man.ui_rp, id, npos);
        */
        // setPosition(ui_man.ui_rp.tds, ui_man.ui_btn_tab.trsid[index], npos);
        }
    


    }
}


void foldMenu(UI_Element_Table<GUI_Menu>& men_tab, UI_Element_Table<GUI_Button> btn_tab, RenderPacket& rp, unsigned int men_id)
{
int index = findUIInUITable(men_tab, men_id);   // finds the menu head
GUI_Menu men = men_tab.data[index]; // gets the menu

for (unsigned int bt_id : men.ui_ids)
    {
    index = findUIInUITable(btn_tab, bt_id);    // finds the UI ID in the table
    int ind2 = findDrawablesTransform(rp.drabs, btn_tab.trsid[index]);  // finding the index of the drawable record
    RemoveRenderDetail(rp.rds, rp.drabs.rids[ind2]); // remove the render detail from the render details object
    UnassignDrawable(rp.drabs, btn_tab.trsid[index]);   // removes the drawable
    }
men.folded = 1;
}

void foldMenu(UI_Manager& ui_man, unsigned int men_id) { foldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, men_id); }

void unfoldMenu(UI_Element_Table<GUI_Menu>& men_tab, UI_Element_Table<GUI_Button> btn_tab, RenderPacket& rp, unsigned int men_id)
{
int index = findUIInUITable(men_tab, men_id);   // finds the menu head
GUI_Menu men = men_tab.data[index]; // gets the menu

for (unsigned int bt_id : men.ui_ids)
    {
    index = findUIInUITable(btn_tab, bt_id);    // finds the UI ID in the table
    GUI_Button button = btn_tab.data[index];    // getting the button
    unsigned int rid = CreateSpriteRenderable(rp.rds, button.ssi.spfp, button.ssi.nosp, button.ssi.spr);    // creating the new renderable
    unsigned int trsid = btn_tab.trsid[index];  // getting the transform ID
    AddDrawable(rp.drabs, trsid, rid);
    }
men.folded = 0;
}

void unfoldMenu(UI_Manager& ui_man, unsigned int men_id) { unfoldMenu(ui_man.ui_men_tab, ui_man.ui_btn_tab, ui_man.ui_rp, men_id); }

#pragma endregion

#pragma region Utilities

int hasPressedUI(UI_Manager ui_man, vec2 cpos)
{
for (unsigned int ui_id : ui_man.ui_btn_tab.ui_id)
    if(isCursorOnUIElement(ui_man.ui_btn_tab, ui_man.ui_rp, ui_id)) // if the cursor is on an element
        return 1;

for (unsigned int ui_id : ui_man.ui_men_tab.ui_id)
    if(isCursorOnUIElement(ui_man.ui_men_tab, ui_man.ui_rp, ui_id)) // if the cursor is on an element
        return 1;

for (unsigned int ui_id : ui_man.ui_text_box_tab.ui_id)
    if(isCursorOnUIElement(ui_man.ui_text_box_tab, ui_man.ui_rp, ui_id)) // if the cursor is on an element
        return 1;

return 0;
}

#pragma endregion

#endif
