#ifndef SYSTEMUI_H
#define SYSTEMUI_H

#pragma once
#include "Drawable.h"
#include "InputManager.h"
#include "Text.h"
#include <iostream>
#include <algorithm>
#include <string>

#define CPP_LIBRARY

#ifdef C_LIBRARY

typedef enum
    {
    UI_TRIGGER_PRESS,
    UI_TRIGGER_HOVER,
    UI_NO_TRIGGERS
    } GUI_ACTION_TRIGGER;

typedef void (*ui_act_fun)(int);

struct GUI_MENU
    {
    std::vector<unsigned int> ui_ids;
    unsigned int men_head_ui_id;
    
    // GUI_MENU(const GUI_MENU& meni) { this->ui_ids = meni.ui_ids; this->men_head_ui_id = meni.men_head_ui_id; }
    GUI_MENU() {};
    GUI_MENU(std::vector<unsigned int> ui_ids, unsigned int men_head_ui_id)
        : ui_ids(ui_ids), men_head_ui_id(men_head_ui_id)
        {
        }
    GUI_MENU(unsigned int men_head_ui_id)
        : ui_ids(), men_head_ui_id(men_head_ui_id)
        {
        }
    GUI_MENU operator=(const GUI_MENU meni)
        {
        this->ui_ids = meni.ui_ids;
        this->men_head_ui_id = meni.men_head_ui_id;
        }

    };

typedef enum
    {
    DEFAULT_SQUARE = 1
    } GUI_RENDER_INFO;

typedef enum
    {
    UI_TYPE_NULL,
    UI_TYPE_BUTTON,
    UI_TYPE_MENU
    } UI_ELEMENT_TYPE;

union RenderInformation
    {
    SpriteSheetInfo ssi;    // sprite sheet information if it is a sprite sheet
    GUI_RENDER_INFO rinf;   // an enum to say if it is a basic shape
    GUI_MENU meni;  // a container of UI IDs which holds the menu contents and the actual menu head

    /*
    RenderInformation() { };
    ~RenderInformation() { };
    RenderInformation(const GUI_MENU men) { this->meni = men; };
    RenderInformation(const SpriteSheetInfo _ssi) { this->ssi = _ssi; };
    RenderInformation(const RenderInformation&) = delete;
    RenderInformation(RenderInformation&&) = delete;
    */
    /*
    RenderInformation(const RenderInformation& ri)
        : meni(ri.meni), rinf(ri.rinf), ssi(ri.ssi)
        {
        }
    */
    // RenderInformation& operator=(const RenderInformation& ri)  { this->meni = ri.meni; this->rinf = ri.rinf; this->ssi = ri.ssi; }
    };

#pragma region Trigger Action Table

struct UI_Trigger_Action_Table
    {
    std::vector<unsigned int> ui_id;
    std::vector<ui_act_fun> action; // the actions for this trigger
    };
typedef struct UI_Trigger_Action_Table UI_Trigger_Action_Table;

#pragma endregion

struct UI_Table
    {
    std::vector<unsigned int> ui_id;    // primary key
    std::vector<unsigned int> trsid;    // foreign key to link to transform

    UI_Trigger_Action_Table actions[UI_NO_TRIGGERS];  // array of all different actions, since indexed by action it should be easy to find the correct ones
    
    // the first part of the data for each element should be an indicator to what it contains and its size
    std::vector<RenderInformation> data;    // the data for each element
    };
typedef struct UI_Table UI_Table;

/**
 * Initialises the UI and allocates memory
 * 
 * @returns A new UI table containing all of the information
 */
UI_Table InitialiseUI();

/**
 * Gets the transform ID for the given UI element
 * 
 * @param ui The UI table
 * @param ui_id The ID of the UI element
 * 
 * @returns The transform ID for the UI element
 */
unsigned int getUITransform(UI_Table ui, unsigned int ui_id);

/**
 * Gets the render information for the given UI element
 * 
 * @param ui The UI table
 * @param ui_id The ID of the UI element
 * 
 * @returns The a pointer to the render information for the UI element
 */
RenderInformation& _getUIRenderInformation(UI_Table& ui, unsigned int ui_id);

/**
 * Gets the render information for the given UI element
 * 
 * @param ui The UI table
 * @param ui_id The ID of the UI element
 * 
 * @returns The render information for the UI element
 */
RenderInformation getUIRenderInformation(UI_Table ui, unsigned int ui_id);

/**
 * Creates a button at the given position with the given details
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet for the UI
 * @param pos The position of the button
 * @param scale The scale factor of the square for the button
 * @param rendinf The information to be used to initialise the button
 * 
 * @returns The ID of the new UI element
 * 
 * @deprecated
 */
unsigned int addButton(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, RenderInformation rendinf);

/**
 * Assigns an action to a button which is performed when the trigger happens
 * 
 * @param ui A reference to the UI table
 * @param ui_id The ID of the UI element to assign the action to
 * @param trigger The trigger for the action
 * @param action A pointer to a function for the action
 */
void assignButtonAction(UI_Table& ui, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action);

/**
 * Creates a UI element at the given position with the given details
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet for the UI
 * @param pos The position of the element
 * @param scale The scale factor of the square for the element
 * @param type The type of the UI element
 * @param rendinf The information to be used to initialise the element
 * 
 * @returns The ID of the new UI element
 */
unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, UI_ELEMENT_TYPE type, RenderInformation rendinf);

unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, SpriteSheetInfo rendinf);

unsigned int createUIElement(UI_Table& ui, RenderPacket& rp, vec2 pos, float scale, GUI_MENU rendinf);

/**
 * Removes the UI element with the given ID from the table
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet
 * @param type The type of the element so that it can be deleted properly
 * @param ui_id The ID of the UI element to delete
 */
void removeUIElement(UI_Table& ui, RenderPacket& rp, UI_ELEMENT_TYPE type, unsigned int ui_id);

/**
 * Adds an element to the menu
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet
 * @param ui_id The ID of the menu UI element
 * @param type The type of the new element to create
 * @param rendinf The render information of the new element to create
 * 
 * @returns The UI ID of the new element created
 */
unsigned int addToMenu(UI_Table& ui, RenderPacket& rp, unsigned int ui_id, UI_ELEMENT_TYPE type, RenderInformation rendinf);

unsigned int addToMenu(UI_Table& ui, RenderPacket& rp, unsigned int ui_id, SpriteSheetInfo rendinf);

/**
 * Removes the given UI element from the menu
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet
 * @param menid The ID of the menu element
 * @param ui_id The ID of the element to remove
 */
void removeFromMenu(UI_Table& ui, RenderPacket& rp, unsigned int menid, unsigned int ui_id);

/**
 * Removes the all of the entries from the menu
 * 
 * @param ui A reference to the UI table
 * @param rp A reference to the render packet
 * @param menid The ID of the menu element
 */
void clearMenu(UI_Table& ui, RenderPacket& rp, unsigned int menid);

/**
 * Removes the button with the given id from the table
 * 
 * @param ui A reference to the ui table
 * @param rp A reference to the render packet
 * @param ui_id The id of the ui element to delete
 * 
 * @deprecated
 */
void removeButton(UI_Table& ui, RenderPacket& rp, unsigned int ui_id);

/**
 * Checks and performs the relevant actions on the UI elements according to their triggers
 * 
 * @param ui The UI table
 * @param rp The render packet for the UI
 */
void checkUI(UI_Table ui, RenderPacket rp);

#endif

#ifdef CPP_LIBRARY

#pragma region Trigger Action Table

enum GUI_ACTION_TRIGGER
    {
    UI_TRIGGER_PRESS,
    UI_TRIGGER_HOVER,
    UI_TRIGGER_LEAVE_HOVER,
    UI_NO_TRIGGERS
    };

typedef void (*ui_act_fun)(int);

struct UI_Trigger_Action_Table
    {
    std::vector<unsigned int> ui_id;
    std::vector<ui_act_fun> action; // the actions for this trigger
    };

#pragma endregion

#pragma region Structs

struct GUI_Menu
    {
    std::vector<unsigned int> ui_ids;
    unsigned int men_head_ui_id;
    int folded = 0;
    };

struct GUI_Button
    {
    SpriteSheetInfo ssi;
    };

struct GUI_Text_Box
    {
    std::string cont;
    };

template<typename T>
struct UI_Element_Table
    {
    std::vector<unsigned int> ui_id;
    std::vector<unsigned int> trsid;

    std::vector<T> data;
    UI_Trigger_Action_Table actions[UI_NO_TRIGGERS];  // array of all different actions, since indexed by action it should be easy to find the correct ones
    };

struct UI_Manager
    {
    void checkUI();
    UI_Manager(RenderPacket& in_ui_rp) : ui_rp(in_ui_rp) {}
    
    UI_Element_Table<GUI_Button> ui_btn_tab;
    UI_Element_Table<GUI_Menu> ui_men_tab;
    UI_Element_Table<GUI_Text_Box> ui_text_box_tab;
    
    RenderPacket& ui_rp;
    };

#pragma endregion

#pragma region Creation

GUI_Button createButton(vec2 pos, float scale, const char* spfp, int nosp, int spr);

GUI_Menu createMenu(vec2 pos, unsigned int head_id);

GUI_Text_Box createTextBox(vec2 pos, std::string cont);

#pragma endregion

#pragma region Element Adding

/**
 * Adds a button to the UI system
 * 
 * @param table A reference to the element table for the element type
 * @param rp A reference to the render packet
 * @param pos The position to render at
 * @param scale The scale of the element
 * @param button The button to add
 * 
 * @returns The UI ID for that button
 */
unsigned int addToElementTable(UI_Element_Table<GUI_Button>& table, RenderPacket& rp, vec2 pos, float scale, GUI_Button button);

/**
 * Adds a menu to the UI system
 * 
 * @param table A reference to the element table for the element type
 * @param btab A reference to the button table
 * @param rp A reference to the render packet
 * @param pos The position of the menu
 * @param menu The menu to add
 * 
 * @returns The UI ID for that menu
 */
unsigned int addToElementTable(UI_Element_Table<GUI_Menu>& table, const UI_Element_Table<GUI_Button>& btab, RenderPacket& rp, vec2 pos, GUI_Menu menu);

/**
 * Adds a text box to the UI system
 * 
 * @param table A reference to the element table for the element type
 * @param rp A reference to the render packet
 * @param pos The position of the text box
 * @param txbx The text box to add
 * 
 * @returns The UI ID for that text box
 */
unsigned int addToElementTable(UI_Element_Table<GUI_Text_Box>& table, RenderPacket& rp, vec2 pos, GUI_Text_Box txbx);

/**
 * Adds a button to the UI system
 * 
 * @param ui_man A reference to the UI manager
 * @param pos The position of the menu
 * @param button The button to add
 * 
 * @returns The UI ID for that button
 */
unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, float scale, GUI_Button button);

/**
 * Adds a menu to the UI system
 * 
 * @param ui_man A reference to the UI manager
 * @param pos The position of the menu
 * @param menu The menu to add
 * 
 * @returns The UI ID for that menu
 */
unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Menu menu);

/**
 * Adds a text box to the UI system
 * 
 * @param ui_man A reference to the UI manager
 * @param pos The position of the text box
 * @param txbx The text box to add
 * 
 * @returns The UI ID for that text box
 */
unsigned int addToElementTable(UI_Manager& ui_man, vec2 pos, GUI_Text_Box txbx);

#pragma endregion 

#pragma region Element Getting

/**
 * Gets the button with the given ID
 * 
 * @param table A reference the the button table
 * @param ui_id The UI ID of the button to find
 * 
 * @returns A reference to the button
 */
GUI_Button& getButton(UI_Element_Table<GUI_Button>& table, unsigned int ui_id);

/**
 * Gets the menu with the given ID
 * 
 * @param table A reference the the menu table
 * @param ui_id The UI ID of the menu to find
 * 
 * @returns A reference to the menu
 */
GUI_Menu& getMenu(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id);

/**
 * Gets the text box with the given ID
 * 
 * @param table A reference the the text box table
 * @param ui_id The UI ID of the text box to find
 * 
 * @returns A reference to the text box
 */
GUI_Text_Box& getTextBox(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id);

#pragma endregion

#pragma region Element Action Assigning

/**
 * Assigns the element action to the button element
 * 
 * @param table A reference to the button table
 * @param ui_id The UI ID of the element to add the trigger to
 * @param trigger The trigger to activate the action
 * @param action The action to perform
 */
void assignElementAction(UI_Element_Table<GUI_Button>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action);

/**
 * Assigns the element action to the menu element
 * 
 * @param table A reference to the menu table
 * @param ui_id The UI ID of the element to add the trigger to
 * @param trigger The trigger to activate the action
 * @param action The action to perform
 */
void assignElementAction(UI_Element_Table<GUI_Menu>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action);

/**
 * Assigns the element action to the text box element
 * 
 * @param table A reference to the text box table
 * @param ui_id The UI ID of the element to add the trigger to
 * @param trigger The trigger to activate the action
 * @param action The action to perform
 */
void assignElementAction(UI_Element_Table<GUI_Text_Box>& table, unsigned int ui_id, GUI_ACTION_TRIGGER trigger, ui_act_fun action);

#pragma endregion

/**
 * Adds the UI element with the given UI ID to the menu
 * 
 * @param men_tab A reference to the menu table
 * @param men_id The UI ID of the menu
 * @param ui_id The UI ID of the element to add to the table
 */
void addToMenu(UI_Element_Table<GUI_Menu>& men_tab, unsigned int men_id, unsigned int ui_id);

/**
 * Folds the menu
 * 
 * @param men_tab A reference to the menu table
 * @param btn_tab The menu table
 * @param rp A reference to the render packet for the UI
 * @param men_id The UI ID of the menu
 */
void foldMenu(UI_Element_Table<GUI_Menu>& men_tab, UI_Element_Table<GUI_Button> btn_tab, RenderPacket& rp, unsigned int men_id);

/**
 * Folds the menu
 * 
 * @param ui_man A reference to the UI manager
 * @param men_id The UI ID of the menu
 */
void foldMenu(UI_Manager& ui_man, unsigned int men_id);

/**
 * Unfolds the menu
 * 
 * @param men_tab A reference to the menu table
 * @param btn_tab The menu table
 * @param rp A reference to the render packet for the UI
 * @param men_id The UI ID of the menu
 */
void unfoldMenu(UI_Element_Table<GUI_Menu>& men_tab, UI_Element_Table<GUI_Button> btn_tab, RenderPacket& rp, unsigned int men_id);

/**
 * Unfolds the menu
 * 
 * @param ui_man A reference to the UI manager
 * @param men_id The UI ID of the menu
 */
void unfoldMenu(UI_Manager& ui_man, unsigned int men_id);

#endif

#endif