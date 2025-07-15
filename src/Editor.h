#pragma once
#include "Drawable.h"
#include "Block.h"
#include "BlockOperations.h"
#include "Camera.h"
#include "Level.h"
#include "SystemUI.h"

#pragma region Main

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

/**
 * Selects the block type to be placed
 * 
 * @param drabs The drawable objects table
 * @param trsid The transformation ID of the object
 */
void SelectBlock(Drawables drabs, unsigned int trsid);

/**
 * Applies the view matrix to all render details in the given table
 * 
 * @param cam The camera to apply
 * @param rds The render details containing the shaders
 * 
 * @warning Make sure the right render detail table is passed otherwise things can go very wrong very quickly
 */
void ApplyCamera(Camera cam, RenderDetails rds);

/**
 * Clears the camera from all the following renderables
 * 
 * @param rds The render table
 */
void ClearCamera(RenderDetails rds);

/**
 * Applies the projection matrix to all render details in the given table
 * 
 * @param cam The camera to apply
 * @param rds The render details containing the shaders
 * 
 * @warning Make sure the right render detail table is passed otherwise things can go very wrong very quickly
 */
void ApplyProjection(Camera cam, RenderDetails rds);

#pragma endregion


#pragma region EditorUI

/**
 * Builds the block selection bar
 */
void BuildSelectBar();

#pragma endregion
