#pragma once
#include "Entity.hpp"
#include "Camera.hpp"
#include "Level.hpp"
#include "SystemUI.hpp"
#include "Utils.h"


struct Editor
    {
    BLOCK_TYPE actblk = BLOCK_TYPE::BLOCK_IMMOVABLE_BLOCK;

    UI_Manager ui_man;  // the UI
    };


/**
 * A function to read the level
 * 
 * A function to render the level
 * 
 * A function to build the item select bar
 * 
 */

/**
 * Gets the currently selected block (the block to be placed)
 * 
 * @param ed The editor information structure
 * 
 * @returns The block type to be created
 */
BLOCK_TYPE getActiveBlock(const Editor& ed);

/**
 * Sets the block to be created
 * 
 * @param ed A reference to the editor structure
 * @param block The block which should be used when blocks are created
 */
void setActiveBlock(Editor& ed, BLOCK_TYPE block);

void CheckEditorInput(Editor& ed, Block_Manager& blk_man, Camera& cam);


// #define EDITOR_UI
#ifdef EDITOR_UI

/**
 * Builds the block selection bar
 */
void BuildSelectBar();

#endif