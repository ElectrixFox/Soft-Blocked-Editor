#ifndef SYSTEMUI_HPP
#define SYSTEMUI_HPP

#pragma once
#include "InputManager.h"
#include "Entity.hpp"
#include <iostream>
#include <algorithm>
#include <string>

UI_Element createButton(vec2 pos, float scale, const char* spfp, int nosp, int spr);

UI_Element createMenu(vec2 pos);

UI_Element createMenu(UI_Manager& ui_man, vec2 pos);

void addToMenu(UI_Element& menu, const UI_Element& entry);

void foldMenu(UI_Manager& ui_man, UI_Element& menu);
void unfoldMenu(UI_Manager& ui_man, UI_Element& menu);

/**
 * @example Here is how to create UI
 * 
 * First use the constructors to create the base elements
 * If there is a menu add the buttons to it
 * Set up the actions using lambdas
 * Finally add the elements to the UI manager
 */
void Example(UI_Manager& ui_man);

// GUI_Menu createMenu(vec2 pos, unsigned int head_id);

// GUI_Text_Box createTextBox(vec2 pos, std::string cont);


#endif