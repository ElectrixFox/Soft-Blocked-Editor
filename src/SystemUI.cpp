#include "SystemUI.h"

#ifdef C_LIBRARY

const int datsize = 256;

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

#pragma region UI_Table

UI_Table InitialiseUI()
{
UI_Table ui;

// initialising all of the memory
for (int i = 0; i < UI_NO_TRIGGERS; i++)    // initialising the trigger actions
    ui.actions[i] = InitialiseUITriggerActions();

return ui;
}

static void expandUITable(UI_Table& ui)
{
}

static int findUIIDinTable(UI_Table ui, unsigned int ui_id)
{
for (int i = 0; i < ui.ui_id.size(); i++)   // simple linear search
    if(ui.ui_id[i] == ui_id)
        return i;
return -1;
}

unsigned int getUITransform(UI_Table ui, unsigned int ui_id) { return ui.trsid[findUIIDinTable(ui, ui_id)]; }

RenderInformation getUIRenderInformation(UI_Table ui, unsigned int ui_id)
{
// ui.data[findUIIDinTable(ui, ui_id)];
// return RenderInformation(ui.data[findUIIDinTable(ui, ui_id)]);
}

RenderInformation& _getUIRenderInformation(UI_Table& ui, unsigned int ui_id) { return ui.data[findUIIDinTable(ui, ui_id)]; }

unsigned int addButton(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, RenderInformation rendinf)
{
static unsigned int ui_id = 0;
expandUITable(ui);

int index = ui_id;  // temporary and should be replaced with a more optimal function
ui.ui_id[index] = ui_id;

int ind = -1;
if(rendinf.rinf == (GUI_RENDER_INFO)NULL)
    {
    ui.data[index].rinf = (GUI_RENDER_INFO)0;
    ind = CreateBasicSquare(rp, pos, scale, {1.0f, 0.0f, 0.0f, 0.0f});  // creates the square
    }
else if(rendinf.ssi.spfp != NULL)   // if there is a file path
    {
    ui.data[index].ssi = rendinf.ssi;
    unsigned int rid = CreateSpriteRenderable(rp.rds, rendinf.ssi.spfp, rendinf.ssi.nosp, rendinf.ssi.spr);
    vec2 scle = {scale, scale};
    unsigned int trsid = AddTransformation(rp.tds, pos, scle, 0.0f);
    ind = AddDrawable(rp.drabs, trsid, rid);
    }
else if(rendinf.meni.ui_ids.size() != 0)  // if there are some IDs in the menu
    {
    }

unsigned int trsid = rp.drabs.trsids[ind]; // gets the transformation ID
ui.trsid[index] = trsid;   // sets the new transformation ID

return ui.ui_id[index];
}

void assignButtonAction(UI_Table& ui, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIIDinTable(ui, ui_id);

if(index == -1)
    {
    printf("\nError: Could not find %d in the UI table", ui_id);
    exit(1);
    }

assignUITriggerAction(ui.actions[trigger], ui_id, action);
}

unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, UI_ELEMENT_TYPE type, RenderInformation rendinf)
{
static unsigned int ui_id = 0;
const unsigned int n = ui.ui_id.size();

expandUITable(ui);

ui.ui_id[n] = ui_id++;    // set and increase the ID

int ind = -1;

switch (type)   // doing the appropriate thing for each type
    {
    case UI_TYPE_NULL:
        {
        ui.data[n].rinf = (GUI_RENDER_INFO)0;
        ind = CreateBasicSquare(rp, pos, scale, {1.0f, 0.0f, 0.0f, 0.0f});  // creates the square
        break;
        }
    case UI_TYPE_BUTTON:
        {
        ui.data[n].ssi = rendinf.ssi;
        unsigned int rid = CreateSpriteRenderable(rp.rds, rendinf.ssi.spfp, rendinf.ssi.nosp, rendinf.ssi.spr);
        unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);
        ind = AddDrawable(rp.drabs, trsid, rid);
        break;
        }
    case UI_TYPE_MENU:
        {
        ui.data[n].meni = rendinf.meni;    // set the menu data
        int tindex = findUIIDinTable(ui, rendinf.meni.men_head_ui_id);
        ind = findDrawablesTransform(rp.drabs, ui.trsid[tindex]);
        break;
        }
    default:
        {
        break;
        }
    }

unsigned int trsid = rp.drabs.trsids[ind]; // gets the transformation ID
ui.trsid[n] = trsid;   // sets the new transformation ID

return ui.ui_id[n];    // returning the new UI ID
}

unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, SpriteSheetInfo rendinf)
{
static unsigned int ui_id = ui.ui_id.size();
const unsigned int n = ui.ui_id.size();

ui.ui_id[n] = ui_id++;    // set and increase the ID

int ind = -1;

ui.data[n].ssi = rendinf;
unsigned int rid = CreateSpriteRenderable(rp.rds, rendinf.spfp, rendinf.nosp, rendinf.spr);
unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);
ind = AddDrawable(rp.drabs, trsid, rid);

unsigned int trsid = rp.drabs.trsids[ind]; // gets the transformation ID
ui.trsid[n] = trsid;   // sets the new transformation ID

return ui.ui_id[n];    // returning the new UI ID
}

unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, GUI_MENU rendinf)
{
static unsigned int ui_id = ui.ui_id.size();
const unsigned int n = ui.ui_id.size();

ui.ui_id[n] = ui_id++;    // set and increase the ID

int ind = -1;

ui.data[n].meni = rendinf;    // set the menu data
int tindex = findUIIDinTable(ui, rendinf.men_head_ui_id);
ind = findDrawablesTransform(rp.drabs, ui.trsid[tindex]);


unsigned int trsid = rp.drabs.trsids[ind]; // gets the transformation ID
ui.trsid[n] = trsid;   // sets the new transformation ID

return ui.ui_id[n];    // returning the new UI ID
}

static void removeUIElementSingle(UI_Table& ui, RenderPacket& rp, unsigned int ui_id)
{
int index = findUIIDinTable(ui, ui_id);

if(index == -1)
    {
    printf("\nWarning: Cannot find UI element %d to delete", ui_id);
    return; // if the index isn't found just quit
    }

const int size = ui.ui_id.size();

for (int i = 0; i < UI_NO_TRIGGERS; i++)    // removes all the possible actions
    removeUITriggerAction(ui.actions[i], ui_id);   // removes the actions

RemoveDrawable(rp.drabs, rp.rds, rp.tds, ui.trsid[index]);   // removing the drawable element

ui.ui_id.erase(ui.ui_id.begin() + index);
ui.trsid.erase(ui.trsid.begin() + index);
ui.data.erase(ui.data.begin() + index);
}

void removeUIElement(UI_Table& ui, RenderPacket& rp, UI_ELEMENT_TYPE type, unsigned int ui_id)
{
switch (type)   // doing the appropriate thing for each type
    {
    case UI_TYPE_NULL:
        {
        removeUIElementSingle(ui, rp, ui_id);
        break;
        }
    case UI_TYPE_BUTTON:
        {
        removeUIElementSingle(ui, rp, ui_id);
        break;
        }
    case UI_TYPE_MENU:
        {
        RenderInformation& menu = ui.data[findUIIDinTable(ui, ui_id)];
        for(auto el_id : menu.meni.ui_ids)
            removeUIElementSingle(ui, rp, el_id);
        removeUIElementSingle(ui, rp, menu.meni.men_head_ui_id);    // removing the head element
        break;
        }
    default:
        {
        break;
        }
    }

}

/**
 * Checks if the UI ID is in the menu
 * 
 * @param menu The menu
 * @param ui_id The ID of the UI element to check
 * 
 * @returns 1 if it is in there, 0 if not
 */
static int isInMenu(GUI_MENU menu, unsigned int ui_id);

static int isInMenu(GUI_MENU menu, unsigned int ui_id)
{
for (int i = 0; i < menu.ui_ids.size(); i++)
    if(menu.ui_ids[i] == ui_id)
        return 1;
return 0;
}

unsigned int addToMenu(UI_Table& ui, RenderPacket& rp, unsigned int ui_id, UI_ELEMENT_TYPE type, RenderInformation rendinf)
{
const float padding = 10.0f;
int index = findUIIDinTable(ui, ui_id);    // getting the UI element in the UI table
vec2 pos = getPosition(rp.tds, ui.trsid[index]); // getting the position
vec2 scale = getScale(rp.tds, ui.trsid[index]); // getting the scale

RenderInformation& ri = ui.data[findUIIDinTable(ui, ui_id)];  // getting the render information
int mensize = ri.meni.ui_ids.size();  // getting the size of the menu

pos = {pos.x - ((mensize + 1) * 50.0f + padding), pos.y};   // getting the new position

unsigned int nui_id = createUIElement(ui, rp, pos, scale.x, rendinf.ssi); // creating the new element
ui.data[index].meni.ui_ids.push_back(nui_id);

return nui_id;
}

unsigned int addToMenu(UI_Table& ui, RenderPacket& rp, unsigned int ui_id, SpriteSheetInfo rendinf)
{
const float padding = 10.0f;
int index = findUIIDinTable(ui, ui_id);    // getting the UI element in the UI table
vec2 pos = getPosition(rp.tds, ui.trsid[index]); // getting the position
vec2 scale = getScale(rp.tds, ui.trsid[index]); // getting the scale

RenderInformation& ri = ui.data[findUIIDinTable(ui, ui_id)];  // getting the render information
int mensize = ri.meni.ui_ids.size();  // getting the size of the menu

pos = {pos.x - ((mensize + 1) * 50.0f + padding), pos.y};   // getting the new position

unsigned int nui_id = createUIElement(ui, rp, pos, scale.x, rendinf); // creating the new element
ui.data[index].meni.ui_ids.push_back(nui_id);

return nui_id;
}

void removeFromMenu(UI_Table& ui, RenderPacket& rp, unsigned int menid, unsigned int ui_id)
{
int index = findUIIDinTable(ui, menid);
GUI_MENU meni = ui.data[index].meni;
if(!isInMenu(meni, ui_id)) // quick bit of error checking
    {
    printf("\nError: %d is not in the menu", ui_id);
    return;
    }

// To-Do: Type?
removeUIElement(ui, rp, UI_TYPE_BUTTON, ui_id); // remove each of the buttons
meni.ui_ids.erase(meni.ui_ids.begin() + ui_id); // remove it from the element array
}

void clearMenu(UI_Table& ui, RenderPacket& rp, unsigned int menid)
{
GUI_MENU& meni = _getUIRenderInformation(ui, menid).meni;
// printf("\nClearing menu:");

for (auto men_ele : meni.ui_ids)
    removeFromMenu(ui, rp, menid, men_ele);
}

void removeButton(UI_Table& ui, RenderPacket& rp, unsigned int ui_id)
{
int index = findUIIDinTable(ui, ui_id);

if(index == -1) return; // if the index isn't found just quit

for (int i = 0; i < UI_NO_TRIGGERS; i++)    // removes all the possible actions
    removeUITriggerAction(ui.actions[i], ui_id);   // removes the actions

RemoveDrawable(rp.drabs, rp.rds, rp.tds, ui.trsid[index]);   // removing the drawable element

ui.ui_id.erase(ui.ui_id.begin() + index);
ui.trsid.erase(ui.trsid.begin() + index);
ui.data.erase(ui.data.begin() + index);
}

static int pressedInRectangle(vec2 pos, vec2 scale)
{
vec2 cpos = getCursorPosition();
return PointInSquare(cpos, pos, scale);
}

static int isCursorOnUIElement(UI_Table ui, RenderPacket rp, unsigned int ui_id)
{
int index = findUIIDinTable(ui, ui_id);
vec2 pos = getPosition(rp.tds, ui.trsid[index]);
vec2 scale = getScale(rp.tds, ui.trsid[index]);
return pressedInRectangle(pos, scale);
}

void checkUI(UI_Table ui, RenderPacket rp)
{
GLFWwindow* window = getWindow();

for (int i = 0; i < ui.ui_id.size(); i++)
    {
    int index = findUITriggerActionIDinTable(ui.actions[UI_TRIGGER_HOVER], ui.ui_id[i]);
    if(index == -1) // if there is no hover action then skip
        continue;
    
    if(isCursorOnUIElement(ui, rp, ui.ui_id[i]))
        {
        // printf("\nPerforming hover action for %d", ui.ui_id[i]);
        ui.actions[UI_TRIGGER_HOVER].action[index](ui.ui_id[i]);
        }
    }


if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
    for (int i = 0; i < ui.ui_id.size(); i++)
        {
        int index = findUITriggerActionIDinTable(ui.actions[UI_TRIGGER_PRESS], ui.ui_id[i]);
        if(index == -1) // if there is no hover action then skip
            continue;
        
        if(isCursorOnUIElement(ui, rp, ui.ui_id[i]))
            {
            printf("\nPerforming action for %d", ui.ui_id[i]);
            ui.actions[UI_TRIGGER_PRESS].action[index](ui.ui_id[i]);
            }
        }
    }
}

#pragma endregion

#endif

#ifdef CPP_LIBRARY

const int datsize = 256;

static int idtrk = 0;

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

/*
unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, UI_ELEMENT_TYPE type, RenderInformation rendinf)
{
static unsigned int ui_id = 0;
const unsigned int n = ui.ui_id.size();

expandUITable(ui);

ui.ui_id[n] = ui_id++;    // set and increase the ID

int ind = -1;

switch (type)   // doing the appropriate thing for each type
    {
    case UI_TYPE_NULL:
        {
        ui.data[n].rinf = (GUI_RENDER_INFO)0;
        ind = CreateBasicSquare(rp, pos, scale, {1.0f, 0.0f, 0.0f, 0.0f});  // creates the square
        break;
        }
    case UI_TYPE_BUTTON:
        {
        ui.data[n].ssi = rendinf.ssi;
        unsigned int rid = CreateSpriteRenderable(rp.rds, rendinf.ssi.spfp, rendinf.ssi.nosp, rendinf.ssi.spr);
        unsigned int trsid = AddTransformation(rp.tds, pos, {scale, scale}, 0.0f);
        ind = AddDrawable(rp.drabs, trsid, rid);
        break;
        }
    case UI_TYPE_MENU:
        {
        ui.data[n].meni = rendinf.meni;    // set the menu data
        int tindex = findUIIDinTable(ui, rendinf.meni.men_head_ui_id);
        ind = findDrawablesTransform(rp.drabs, ui.trsid[tindex]);
        break;
        }
    default:
        {
        break;
        }
    }

unsigned int trsid = rp.drabs.trsids[ind]; // gets the transformation ID
ui.trsid[n] = trsid;   // sets the new transformation ID

return ui.ui_id[n];    // returning the new UI ID
}
*/

GUI_Button createButton(vec2 pos, float scale, const char* spfp, int nosp, int spr)
{
GUI_Button button {
    .ssi = {spfp, nosp, spr}
};
return button;
}

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

GUI_Menu createMenu(vec2 pos, unsigned int head_id)
{
GUI_Menu menu {
    .ui_ids = {},
    .men_head_ui_id = head_id
};
return menu;
}

unsigned int addToElementTable(UI_Element_Table<GUI_Menu>& table, const UI_Element_Table<GUI_Button>& btab, RenderPacket& rp, vec2 pos, GUI_Menu menu)
{
static unsigned int ui_id = 0;
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

unsigned int addToElementTable(UI_Element_Table<GUI_Text_Box>& table, RenderPacket& rp, vec2 pos, GUI_Text_Box txbx)
{
static unsigned int ui_id = 0;

unsigned int rid = AddCharacter(rp, txbx.cont[0], pos, 25.0f);
unsigned int trsid = rp.tds.trsid[findDrawablesRenderable(rp.drabs, rid)];

table.ui_id.push_back(ui_id);   // adding the ID
table.trsid.push_back(trsid);   // adding the transform
table.data.push_back(txbx);   // adding the actual button

ui_id++;

return ui_id - 1;
}

unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Text_Box txbx)
{
UI_Element_Table<GUI_Text_Box>& table = ui_man.ui_text_box_tab;
return addToElementTable(table, ui_man.ui_rp, pos, txbx);
}

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

GUI_Menu& getMenu(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);
return table.data[index];
}

GUI_Button& getButton(UI_Element_Table<GUI_Button>& table, unsigned int ui_id)
{
int index = findUIInUITable(table, ui_id);
return table.data[index];
}

GUI_Text_Box& getTextBox(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id) { return getUIElement(table, ui_id); }

template<typename T>
static void _assignElementAction(UI_Element_Table<T>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIInUITable(table, ui_id);
assignUITriggerAction(table.actions[trigger], ui_id, action);
}

void assignElementAction(UI_Element_Table<GUI_Button>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIInUITable(table, ui_id);
assignUITriggerAction(table.actions[trigger], ui_id, action);
}

void assignElementAction(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIInUITable(table, ui_id);
assignUITriggerAction(table.actions[trigger], ui_id, action);
}

void assignElementAction(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action) { _assignElementAction(table, ui_id, trigger, action); }

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


#endif