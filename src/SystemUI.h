#ifndef SYSTEMUI_H
#define SYSTEMUI_H

#pragma once
#include "Drawable.h"
#include "InputManager.h"
#include "Text.h"
#include <iostream>
#include <algorithm>
#include <string>

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
    int aligndown = 0;
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
    UI_Manager(RenderPacket& in_ui_rp) : ui_rp(in_ui_rp), ch_tab(ui_rp.rds) { }

    void initialise()
        {
        ui_rp = InitialiseRenderPacket();
        InitialiseCharacterTable(ch_tab);
        }
    
    UI_Element_Table<GUI_Button> ui_btn_tab;
    UI_Element_Table<GUI_Menu> ui_men_tab;
    UI_Element_Table<GUI_Text_Box> ui_text_box_tab;
    
    RenderPacket& ui_rp;
    Character_Table ch_tab;
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
 * @param ch_tab A constant reference to the character table
 * @param rp A reference to the render packet
 * @param pos The position of the text box
 * @param txbx The text box to add
 * 
 * @returns The UI ID for that text box
 */
unsigned int addToElementTable(UI_Element_Table<GUI_Text_Box>& table, const Character_Table& ch_tab, RenderPacket& rp, vec2 pos, GUI_Text_Box txbx);

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

#pragma region Menu

/**
 * Adds the UI element with the given UI ID to the menu
 * 
 * @param men_tab A reference to the menu table
 * @param men_id The UI ID of the menu
 * @param ui_id The UI ID of the element to add to the table
 */
void addToMenu(UI_Element_Table<GUI_Menu>& men_tab, unsigned int men_id, unsigned int ui_id);

/**
 * Adds the UI element with the given UI ID to the menu
 * 
 * @param ui_man A reference to the UI manager
 * @param men_id The UI ID of the menu
 * @param ui_id The UI ID of the element to add to the table
 * @param fittomenu Should the element be made to fit to the menu
 * @param valign Should the elements be vertically
 */
void addToMenu(UI_Manager& ui_man, unsigned int men_id, unsigned int ui_id, int fittomenu = 1, int valign = 1);

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

#pragma endregion

#endif