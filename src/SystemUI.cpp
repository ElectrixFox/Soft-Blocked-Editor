#include "SystemUI.h"

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
static void expandUITriggerActionTable(UI_Trigger_Action_Table* ta);

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
static void addUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id, ui_act_fun action);

/**
 * Removes a trigger action from the trigger action table
 * 
 * @param ta Pointer to the trigger action table
 * @param ui_id The ID of the UI element to remove
 */
static void removeUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id);

/**
 * Sets a trigger action in the action table
 * 
 * @param ta Pointer to the trigger action table
 * @param ui_id The ID of the UI element to add the action to
 * @param action The action to add
 */
static void assignUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id, ui_act_fun action);

static UI_Trigger_Action_Table InitialiseUITriggerActions()
{
UI_Trigger_Action_Table ta;

// initialising all of the memory
ta.ui_id = (unsigned int*)malloc(sizeof(unsigned int));
ta.action = (ui_act_fun*)malloc(10 * sizeof(ui_act_fun));
ta.size = 0;

return ta;
}

static void expandUITriggerActionTable(UI_Trigger_Action_Table* ta)
{
const int n = ta->size;

ExpandByOne(&ta->action, n, sizeof(ui_act_fun));
ExpandByOne(&ta->ui_id, n, sizeof(unsigned int));

ta->size++;
}

static int findUITriggerActionIDinTable(UI_Trigger_Action_Table ta, unsigned int ui_id)
{
for (int i = 0; i < ta.size; i++)   // simple linear search
    if(ta.ui_id[i] == ui_id)
        return i;
return -1;
}

static void addUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id, ui_act_fun action)
{
expandUITriggerActionTable(ta);

int index = ta->size - 1;  // temporary and should be replaced with a more optimal function
ta->ui_id[index] = ui_id;
ta->action[index] = action;
}

static void removeUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id)
{
int index = findUITriggerActionIDinTable(*ta, ui_id);

if(index == -1)
    return; // if the index isn't found just quit

const int size = ta->size;
if(index == size - 1) goto end;   // if the index is the last thing just decrease the size

// getting temporary stuff
unsigned int tmpid = ta->ui_id[index];
ui_act_fun tact = ta->action[index];

// setting the to delete to the end values
ta->ui_id[index] = ta->ui_id[size - 1];
ta->action[index] = ta->action[size - 1];

// setting the end to the thing to delete
ta->ui_id[size - 1] = tmpid;
ta->action[size - 1] = tact;

end:
ta->size--; // decrease the size so it is effectively not there
}

static void assignUITriggerAction(UI_Trigger_Action_Table* ta, unsigned int ui_id, ui_act_fun action)
{
int index = findUITriggerActionIDinTable(*ta, ui_id);

if(index == -1)
    {
    addUITriggerAction(ta, ui_id, action);
    }
else
    ta->action[index] = action;
}

#pragma endregion

UI_Table InitialiseUI()
{
UI_Table ui;

// initialising all of the memory
ui.ui_id = (unsigned int*)malloc(sizeof(unsigned int));
ui.trsid = (unsigned int*)malloc(sizeof(unsigned int));

for (int i = 0; i < UI_NO_TRIGGERS; i++)    // initialising the trigger actions
    ui.actions[i] = InitialiseUITriggerActions();

ui.data = (RenderInformation*)malloc(sizeof(RenderInformation));
ui.size = 0;

return ui;
}

static void expandUITable(UI_Table* ui)
{
const int n = ui->size;

ExpandByOne(&ui->ui_id, n, sizeof(unsigned int));
ExpandByOne(&ui->trsid, n, sizeof(unsigned int));
ExpandByOne(&ui->data, n, datsize);

ui->size++; // increasing the size
}

static int findUIIDinTable(UI_Table ui, unsigned int ui_id)
{
for (int i = 0; i < ui.size; i++)   // simple linear search
    if(ui.ui_id[i] == ui_id)
        return i;
return -1;
}

unsigned int getUITransform(UI_Table ui, unsigned int ui_id) { return ui.trsid[findUIIDinTable(ui, ui_id)]; }

RenderInformation* _getUIRenderInformation(UI_Table* ui, unsigned int ui_id) { return &ui->data[findUIIDinTable(*ui, ui_id)]; }

RenderInformation getUIRenderInformation(UI_Table ui, unsigned int ui_id) { return *_getUIRenderInformation(&ui, ui_id); }

unsigned int addButton(UI_Table* ui, RenderPacket* rp, vec2 pos, float scale, RenderInformation rendinf)
{
static unsigned int ui_id = 0;
expandUITable(ui);

int index = ui_id;  // temporary and should be replaced with a more optimal function
ui->ui_id[index] = ui_id++;    // set and increase the ID

int ind = -1;
if(rendinf.rinf == (GUI_RENDER_INFO)NULL)
    {
    ui->data[index].rinf = (GUI_RENDER_INFO)0;
    ind = CreateBasicSquare(rp, pos, scale, NULL);  // creates the square
    }
else if(rendinf.ssi.spfp != NULL)   // if there is a file path
    {
    ui->data[index].ssi = rendinf.ssi;
    unsigned int rid = CreateSpriteRenderable(&rp->rds, rendinf.ssi.spfp, rendinf.ssi.nosp, rendinf.ssi.spr);
    unsigned int trsid = AddTransformation(&rp->tds, pos, (vec2){scale, scale}, 0.0f);
    ind = AddDrawable(&rp->drabs, trsid, rid);
    }
else if(rendinf.meni.ui_ids.size != 0)  // if there are some IDs in the menu
    {
    }

unsigned int trsid = rp->drabs.trsids[ind]; // gets the transformation ID
ui->trsid[index] = trsid;   // sets the new transformation ID

return ui->ui_id[index];
}

void assignButtonAction(UI_Table* ui, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action)
{
int index = findUIIDinTable(*ui, ui_id);

if(index == -1)
    {
    printf("\nError: Could not find %d in the UI table", ui_id);
    exit(1);
    }

assignUITriggerAction(&ui->actions[trigger], ui_id, action);
}

unsigned int createUIElement(UI_Table* ui, RenderPacket* rp, vec2 pos, float scale, UI_ELEMENT_TYPE type, RenderInformation rendinf)
{
static unsigned int ui_id = 0;
const unsigned int n = ui->size;

expandUITable(ui);

ui->ui_id[n] = ui_id++;    // set and increase the ID

int ind = -1;

switch (type)   // doing the appropriate thing for each type
    {
    case UI_TYPE_NULL:
        ui->data[n].rinf = (GUI_RENDER_INFO)0;
        ind = CreateBasicSquare(rp, pos, scale, NULL);  // creates the square
        break;
    case UI_TYPE_BUTTON:
        ui->data[n].ssi = rendinf.ssi;
        unsigned int rid = CreateSpriteRenderable(&rp->rds, rendinf.ssi.spfp, rendinf.ssi.nosp, rendinf.ssi.spr);
        unsigned int trsid = AddTransformation(&rp->tds, pos, (vec2){scale, scale}, 0.0f);
        ind = AddDrawable(&rp->drabs, trsid, rid);
        break;
    case UI_TYPE_MENU:
        ui->data[n].meni = rendinf.meni;    // set the menu data
        int tindex = findUIIDinTable(*ui, rendinf.meni.men_head_ui_id);
        ind = findDrawablesTransform(rp->drabs, ui->trsid[tindex]);
        break;
    default:
        break;
    }

unsigned int trsid = rp->drabs.trsids[ind]; // gets the transformation ID
ui->trsid[n] = trsid;   // sets the new transformation ID

return ui->ui_id[n];    // returning the new UI ID
}

static void removeUIElementSingle(UI_Table* ui, RenderPacket* rp, unsigned int ui_id)
{
int index = findUIIDinTable(*ui, ui_id);

if(index == -1)
    {
    printf("\nWarning: Cannot find UI element %d to delete", ui_id);
    return; // if the index isn't found just quit
    }

const int size = ui->size;

for (int i = 0; i < UI_NO_TRIGGERS; i++)    // removes all the possible actions
    removeUITriggerAction(&ui->actions[i], ui_id);   // removes the actions

RemoveDrawable(&rp->drabs, &rp->rds, &rp->tds, ui->trsid[index]);   // removing the drawable element

if(index == size - 1) goto end;   // if the index is the last thing just decrease the size

// getting the temporary variables
unsigned int tmpid = ui->ui_id[index];
unsigned int tmptrsid = ui->trsid[index];
RenderInformation tmpdata = ui->data[index];

// setting the to delete to the end values
ui->ui_id[index] = ui->ui_id[size - 1];
ui->trsid[index] = ui->trsid[size - 1];
ui->data[index] = ui->data[size - 1];

// overwriting the end values so they can be ignored 
ui->ui_id[size - 1] = tmpid;
ui->trsid[size - 1] = tmptrsid;
ui->data[size - 1] = tmpdata;

end:
ui->size--; // decrease the size so it is effectively not there
}

void removeUIElement(UI_Table* ui, RenderPacket* rp, UI_ELEMENT_TYPE type, unsigned int ui_id)
{
switch (type)   // doing the appropriate thing for each type
    {
    case UI_TYPE_NULL:
        removeUIElementSingle(ui, rp, ui_id);
        break;
    case UI_TYPE_BUTTON:
        removeUIElementSingle(ui, rp, ui_id);
        break;
    case UI_TYPE_MENU:
        GUI_MENU* menu = &_getUIRenderInformation(ui, ui_id)->meni;
        for (int i = 0; i < menu->ui_ids.size; i++)
            {
            removeUIElementSingle(ui, rp, menu->ui_ids.data[i]);
            }
        menu->ui_ids.size = 0;  // resetting the size
        removeUIElementSingle(ui, rp, menu->men_head_ui_id);    // removing the head element
        break;
    default:
        break;
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
for (int i = 0; i < menu.ui_ids.size; i++)
    if(menu.ui_ids.data[i] == ui_id)
        return 1;
return 0;
}

unsigned int addToMenu(UI_Table* ui, RenderPacket* rp, unsigned int ui_id, UI_ELEMENT_TYPE type, RenderInformation rendinf)
{
const float padding = 10.0f;
int index = findUIIDinTable(*ui, ui_id);    // getting the UI element in the UI table
vec2 pos = getPosition(rp->tds, ui->trsid[index]); // getting the position
vec2 scale = getScale(rp->tds, ui->trsid[index]); // getting the scale

RenderInformation ri = getUIRenderInformation(*ui, ui_id);  // getting the render information
int mensize = ri.meni.ui_ids.size;  // getting the size of the menu

pos = (vec2){pos.x - ((mensize + 1) * 50.0f + padding), pos.y};   // getting the new position

unsigned int nui_id = createUIElement(ui, rp, pos, scale.x, type, rendinf); // creating the new element

AppendToArray(&ui->data[index].meni.ui_ids, nui_id);    // adding the new ID to the array

return nui_id;
}

void removeFromMenu(UI_Table* ui, RenderPacket* rp, unsigned int menid, unsigned int ui_id)
{
int index = findUIIDinTable(*ui, menid);
GUI_MENU* meni = &ui->data[index].meni;
if(!isInMenu(*meni, ui_id)) // quick bit of error checking
    {
    printf("\nError: %d is not in the menu", ui_id);
    return;
    }

// To-Do: Type?
removeUIElement(ui, rp, UI_TYPE_BUTTON, ui_id); // remove each of the buttons
RemoveFromArray(&meni->ui_ids, ui_id);  // remove it from the element array
}

void clearMenu(UI_Table* ui, RenderPacket* rp, unsigned int menid)
{
GUI_MENU meni = getUIRenderInformation(*ui, menid).meni;
Array uids = meni.ui_ids;
// printf("\nClearing menu:");
int size = getUIRenderInformation(*ui, menid).meni.ui_ids.size;
do
    {
    removeFromMenu(ui, rp, menid, uids.data[0]);
    size = getUIRenderInformation(*ui, menid).meni.ui_ids.size;
    } while (size != 0);
}

void removeButton(UI_Table* ui, RenderPacket* rp, unsigned int ui_id)
{
int index = findUIIDinTable(*ui, ui_id);

if(index == -1)
    return; // if the index isn't found just quit

const int size = ui->size;
if(index == size - 1) goto end;   // if the index is the last thing just decrease the size

for (int i = 0; i < UI_NO_TRIGGERS; i++)    // removes all the possible actions
    removeUITriggerAction(&ui->actions[i], ui_id);   // removes the actions

RemoveDrawable(&rp->drabs, &rp->rds, &rp->tds, ui->trsid[index]);   // removing the drawable element

// getting the temporary variables
unsigned int tmpid = ui->ui_id[index];
unsigned int tmptrsid = ui->trsid[index];
RenderInformation tmpdata = ui->data[index];

// setting the to delete to the end values
ui->ui_id[index] = ui->ui_id[size - 1];
ui->trsid[index] = ui->trsid[size - 1];
ui->data[index] = ui->data[size - 1];

// overwriting the end values so they can be ignored 
ui->ui_id[size - 1] = tmpid;
ui->trsid[size - 1] = tmptrsid;
ui->data[size - 1] = tmpdata;

end:
ui->size--; // decrease the size so it is effectively not there
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

for (int i = 0; i < ui.size; i++)
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
    for (int i = 0; i < ui.size; i++)
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
