#pragma once
#include "Entity.hpp"

extern const int snap_to_grid;
extern const int grid_size;

/**
 * Outputs the level in matrix form
 * 
 * @param grid The grid returned by the getLevel (or similar) function
 * @param w The width of the grid
 * @param h The height of the grid
 */
void OutputLevel(const int** grid, int w, int h);

/**
 * Reads the level from a file
 * 
 * @param levelfp The file path to the level
 * @param w A pointer to the width of the grid (to be set by the function)
 * @param h A pointer to the height of the grid (to be set by the function)
 * @param grid A pointer to the 2D array which the grid is (set by the function)
 */
void ReadLevel(const char* levelfp, int* w, int* h, int*** grid);

/**
 * Writes the level to the given file
 * 
 * @param levelfp The file path to write the level to
 * @param w The width of the level grid
 * @param h The height of the level grid
 * @param grid The actual grid
 */
void WriteLevel(const char* levelfp, const int w, const int h, const int** grid);

/**
 * Gets the information of where the blocks are
 * 
 * @param blk_man A constant block manager
 * @param w A pointer to the width (this will be set)
 * @param h A pointer to the height (this will be set)
 * @param grid A pointer to a 2D array of integers (again this will be set)
 */
void getLevel(const Block_Manager& blk_man, int* w, int* h, int*** grid);

/**
 * Draws the level to the screen
 * 
 * @param rp A reference to the block manager
 * @param w The width of the grid
 * @param h The height of the grid
 * @param grid The actual grid (a 2D array of integers)
 */
void DrawLevel(Block_Manager& blk_man, int w, int h, const int** grid);

void outputScope(const int scale, const int** scope);
void getBlockScopeAt(const Block_Manager& blk_man, vec2 pos, int*** scope);
void UpdateImmovableBlock(Block_Manager& blk_man, Block& blk);

/**
 * Gets the type of the immovable block
 * 
 * @param w The width of the grid
 * @param h The height of the grid
 * @param grid A constant grid
 * @param pos The position of the block in the grid to get the type of
 * @param angle A pointer to the angle to be changed if there is an angle
 * 
 * @returns The state of the immovable state
 */
BLOCK_IM_STATE getImmovableType(const int w, const int h, const int** grid, vec2 pos, float* angle);

/**
 * Updates the immovable blocks in the level
 * 
 * @param rp A reference to the block manager
 */
unsigned int UpdateImmovableBlocks(Block_Manager& block_man);