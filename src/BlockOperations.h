#pragma once
#include "Drawable.h"
#include "Block.h"

#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

struct BlockManager
    {
    const size_t size;
    unsigned int* rids;
    BLOCK* block;
    };
typedef struct BlockManager BlockManager;

void InitialiseBlockManager();

#endif

#ifndef BLOCK_OPERATIONS_H
#define BLOCK_OPERATIONS_H

/**
 * Creates a block at the position with default scale (25x25)
 * 
 * @param rds The rendering table
 * @param tds The transform table
 * @param block The block information of the block to create
 * @param position Where to create the block
 * @param theta The angle of the block to be placed
 * 
 * @returns The render ID for the new block
 */
unsigned int _PlaceBlockCustom(RenderPacket* rp, BlockInfo block, vec2 position, float theta);

/**
 * Creates a block at the position with default scale (25x25)
 * 
 * @param rds The rendering table
 * @param tds The transform table
 * @param block The block to create
 * @param position Where to create the block
 * 
 * @returns The render ID for the new block
 */
unsigned int PlaceBlock(RenderPacket* rp, BLOCK block, vec2 position);

/**
 * Removes a block at the position
 * 
 * @param rds The rendering table
 * @param tds The transform table
 * @param rid The render ID of the block to remove
 */
void RemoveBlock(RenderPacket* rp, unsigned int rid);

/**
 * Rotates a given block by theta degrees
 * 
 * @param rp A pointer to the render packet
 * @param rid The render ID for the block to rotate
 * @param theta The angle (in radians) to rotate the block by
 */
void RotateBlock(RenderPacket* rp, unsigned int rid, float theta);

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

unsigned int PlaceImmovableBlock(RenderPacket* rp, BlockInfo block, vec2 position);

/**
 * Updates the immovable blocks in the grid
 * 
 * @param rp A pointer to the render packet
 * @param w The width of the grid
 * @param h The height of the grid
 * @param grid The actual grid
 */
unsigned int UpdateImmovableBlocks(RenderPacket* rp, const int w, const int h, const int** grid);

#endif