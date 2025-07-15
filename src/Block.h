#pragma once
#include "MathsOperations.h"
#include "Rendering/SpriteShapes.h"
#include <stddef.h>

#pragma region Block Object

/**
 * Could subdivide the block enum into:
 * 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 * Sprite | Block
 */

enum BLOCK {
    BLOCK_PLAYER = 0,
    BLOCK_MOVABLE_BARRIER = 1,
    BLOCK_MOVABLE_BLOCK = 2,
    BLOCK_IMMOVABLE_BLOCK,
    BLOCK_COUNTABLE_BLOCK,
    BLOCK_MOVABLE_DESTINATION,
    BLOCK_PLAYER_DESTINATION,
    BLOCK_COUNT,
};
typedef enum BLOCK BLOCK;

struct BlockInfo
    {
    const char* spfp;
    unsigned int nosp;
    unsigned int spr;
    };
typedef struct BlockInfo BlockInfo;

struct BlockDetails
    {
    unsigned int* rids;
    BLOCK* blocks;
    unsigned int size;
    };
typedef struct BlockDetails BlockDetails;

/**
 * Sets the active sprite of the block
 * 
 * @param block A pointer to the block variable
 * @param spr The sprite to set the block
 */
void setBlockSprite(unsigned int* block, unsigned int spr);

unsigned int getBlockSprite(unsigned int block);

void setBlockSpriteCount(unsigned int* block, unsigned int nospr);

unsigned int getBlockSpriteCount(unsigned int block);

void setBlockType(unsigned int* block, unsigned int type);

unsigned int getBlockType(unsigned int block);


/**
 * Initialises the block details to enable assigning of blocks to drawables
 */
void InitialiseBlockDetails();

/**
 * Gets the number of blocks
 * 
 * @returns Number of blocks
 */
unsigned int getBlockCount();


/**
 * Returns the index of the renderable
 * 
 * @param rid The ID of the renderable to find
 * 
 * @returns Index of ID in the details object or -1 if not found
 */
int getBlockRenderIndex(unsigned int rid);

/**
 * Finds the block of the renderable
 * 
 * @param rid The ID of the renderable to find
 * 
 * @returns Returns the block of the renderable
 */
BLOCK getBlockFromRenderID(unsigned int rid);

/**
 * Adds a block to the details
 * 
 * @param rid The ID of the renderable
 * @param block The block to assign
 */
void AssignBlock(unsigned int rid, BLOCK block);

/**
 * Unassigns a block from the render object
 * 
 * @param blds A pointer to the details
 * @param rid The ID of the object to remove
 */
void UnassignBlock(unsigned int rid);

BlockInfo getBlockInfo(BLOCK block);

enum BLOCK_IM_STATE {
    BLOCK_NULL,
    BLOCK_IM_STATE_ALONE,
    BLOCK_IM_STATE_LINE_END,
    BLOCK_IM_STATE_LINE_STRAIGHT,
    BLOCK_IM_STATE_THREE_INTERSECT,
    BLOCK_IM_STATE_FOUR_INTERSECT,
    BLOCK_IM_STATE_CORNER
};
typedef enum BLOCK_IM_STATE BLOCK_IM_STATE;

/**
 * Gets the block information for the immovable block
 * 
 * @param dir The direction that the block is facing
 * 
 * @returns The block information
 */
BlockInfo getImmovableBlock(BLOCK_IM_STATE state);

/**
 * Gets the sprite count of the passed block
 * 
 * @param block The block to get
 * 
 * @returns The block info
 */
int getSpriteCount(BLOCK block);

/**
 * Finds the block from the path to its sprite sheet
 * 
 * @param fp The file path to the sprite sheet
 * 
 * @returns The block of the sprite
 */
BLOCK getBlockFromFilePath(const char* fp);

/**
 * Finds the block from the details
 * 
 * @param spfp The file path to the sprite sheet
 * @param nosp The size of the sprite sheet
 * @param spr The sprite
 * 
 * @returns The block of the sprite
 */
BLOCK getBlockFromDetails(const char* spfp, unsigned int nosp, unsigned int spr);

#pragma endregion
