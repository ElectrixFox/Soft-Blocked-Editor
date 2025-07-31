#pragma once
#include "Drawable.h"
#include "Block.h"
#include "BlockOperations.h"
#include "Camera.h"
#include "Level.h"
#include "SystemUI.h"

#pragma region Functions

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
 * @returns The block type to be created
 */
BlockInfo getActiveBlock();

/**
 * Sets the block to be created
 * 
 * @param block The block which should be used when blocks are created
 */
void setActiveBlock(BlockInfo block);

void CheckEditorInput(RenderPacket& block_rp, Camera& cam, int& updims, int w, int h, int** grid);

#pragma endregion


#pragma region EditorUI

#define EDITOR_UI
#ifdef EDITOR_UI

/**
 * Builds the block selection bar
 */
void BuildSelectBar();

#endif

#pragma endregion
