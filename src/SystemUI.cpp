#include "SystemUI.h"

/*
extern Character_Table ch_tab;
extern Text_Table txt_tab;
extern RenderPacket text_rp;
*/

const int datsize = 256;
static unsigned int idtrk = 0;

#pragma region Trigger Action Table

/**
 * Initialises the UI triggers and allocates memory
 * 
 * @returns A new UI trigger table containing all of the actions
 */
static UI_Trigger_Action_Table InitialiseUITriggerActions();

/**
 * Adds a new action record to the table
 * 
 * @param ta A pointer to the trigger action table for a given trigger
 */
static void expandUITriggerActionTable(UI_Trigger_Action_Table& ta);

/**
 * Finds the UI ID within the trigger action table
 * 
 * @param ta The trigger action table
 * @param ui_id The UI ID of the element to find
 * 
 * @returns The index of the UI ID in the action table if found, -1 if not
 */
static int findUITriggerActionIDinTable(UI_Trigger_Action_Table ta, unsigned int ui_id);

/**
 * Adds a trigger action to the trigger action table
 * 
 * @param ta Pointer to the trigger action table
 * @param ui_id The ID of the UI element to add the action to
 * @param action The action to add
 */
static void addUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id, ui_act_fun action);

/**
 * Removes a trigger action from the trigger action table
 * 
 * @param ta Pointer to the trigger action table
 * @param ui_id The ID of the UI element to remove
 */
static void removeUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id);

/**
 * Sets a trigger action in the action table
 * 
 * @param ta Pointer to the trigger action table
 * @param ui_id The ID of the UI element to add the action to
 * @param action The action to add
 */
static void assignUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id, ui_act_fun action);

static UI_Trigger_Action_Table InitialiseUITriggerActions()
{
UI_Trigger_Action_Table ta;

// initialising all of the memory
return ta;
}

static void expandUITriggerActionTable(UI_Trigger_Action_Table& ta)
{
}

static int findUITriggerActionIDinTable(UI_Trigger_Action_Table ta, unsigned int ui_id)
{
for (int i = 0; i < ta.ui_id.size(); i++)   // simple linear search
    if(ta.ui_id[i] == ui_id)
        return i;
return -1;
}

static void addUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id, ui_act_fun action)
{
expandUITriggerActionTable(ta);

ta.ui_id.push_back(ui_id);
ta.action.push_back(action);
}

static void removeUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id)
{
int index = findUITriggerActionIDinTable(ta, ui_id);

if(index == -1)
    return; // if the index isn't found just quit

ta.ui_id.erase(ta.ui_id.begin() + index);
ta.action.erase(ta.action.begin() + index);
}

static void assignUITriggerAction(UI_Trigger_Action_Table& ta, unsigned int ui_id, ui_act_fun action)
{
int index = findUITriggerActionIDinTable(ta, ui_id);

if(index == -1)
    {
    addUITriggerAction(ta, ui_id, action);
    }
else
    ta.action[index] = action;
}

#pragma endregion

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
vec2 pos = getPosition(rp.tds, table.trsid[index]);
vec2 scale = getScale(rp.tds, table.trsid[index]);
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

unsigned int addToElementTable(UI_Element_Table<GUI_Button>& table, RenderPacket& rp, vec2 pos, float scale, GUI_Button button)
{
static unsigned int ui_id = 0;

unsigned int rid = CreateSpriteRenderable(rp.rds, button.ssi.spfp, button.ssi.nosp, button.ssi.spr);
unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);
int ind = AddDrawable(rp.drabs, trsid, rid);

table.ui_id.push_back(ui_id);   // adding the ID
table.trsid.push_back(trsid);   // adding the transform
table.data.push_back(button);   // adding the actual button

ui_id++;

return ui_id - 1;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, float scale, GUI_Button button)
{
UI_Element_Table<GUI_Button>& table = ui_man.ui_btn_tab;
return addToElementTable(table, ui_man.ui_rp, pos, scale, button);
}

unsigned int addToElementTable(UI_Element_Table<GUI_Menu>& table, const UI_Element_Table<GUI_Button>& btab, RenderPacket& rp, vec2 pos, GUI_Menu menu)
{
// static unsigned int ui_id = 0;
unsigned int& ui_id = idtrk;

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

ui_id++;

return ui_id - 1;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Menu menu)
{
UI_Element_Table<GUI_Button>& btab = ui_man.ui_btn_tab;
UI_Element_Table<GUI_Menu>& table = ui_man.ui_men_tab;

return addToElementTable(table, btab, ui_man.ui_rp, pos, menu);
}

unsigned int addToElementTable(UI_Element_Table<GUI_Text_Box>& table, const Character_Table& ch_tab, RenderPacket& rp, vec2 pos, GUI_Text_Box txbx)
{
// static unsigned int ui_id = 0;
unsigned int& ui_id = idtrk;
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

ui_id++;

return ui_id - 1;
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

