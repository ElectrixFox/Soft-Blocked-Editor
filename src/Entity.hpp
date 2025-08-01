#ifndef ENTITY_HPP
#define ENTITY_HPP
#pragma once
#include "Renderer.hpp"
#include "Camera.hpp"
#include <functional>

struct Entity
    {
    Entity();

    void draw();
    std::function<void (Entity&)> update();
    };

#pragma region Block

enum BLOCK_TYPE {
    BLOCK_PLAYER = 0,
    BLOCK_MOVABLE_BARRIER = 1,
    BLOCK_MOVABLE_BLOCK = 2,
    BLOCK_IMMOVABLE_BLOCK,
    BLOCK_COUNTABLE_BLOCK,
    BLOCK_MOVABLE_DESTINATION,
    BLOCK_PLAYER_DESTINATION,
    BLOCK_COUNTABLE_DESTINATION,
    BLOCK_PLAYER_BARRIER,
    BLOCK_TELEPORTER_SOURCE,
    BLOCK_TELEPORTER_DESTINATION,
    BLOCK_COUNT
};

enum BLOCK_IM_STATE {
    BLOCK_NULL,
    BLOCK_IM_STATE_ALONE,
    BLOCK_IM_STATE_LINE_END,
    BLOCK_IM_STATE_LINE_STRAIGHT,
    BLOCK_IM_STATE_THREE_INTERSECT,
    BLOCK_IM_STATE_FOUR_INTERSECT,
    BLOCK_IM_STATE_CORNER
};

SpriteSheetInfo getBlockSSI(BLOCK_TYPE btype);

/**
 * Gets the block information for the immovable block
 * 
 * @returns The block information
 */
SpriteSheetInfo getImmovableBlock(BLOCK_IM_STATE state);

struct Block
    {
    Block(BLOCK_TYPE btype, vec2 position);

    const BLOCK_TYPE type;
    unsigned int bl_id;
        
    // rendering stuff
    RenderObject rend_obj;
    SpriteSheetInfo ssi;

    // transformation stuff
    vec2 pos;
    vec2 scale;
    float angle;

    // runtime stuff
    void draw();
    std::function<void (Block& blk)> update;
    };


class Block_Manager
    {
    public:
        Block_Manager()
            : blocks()
            {}
        void drawBlocks(Camera cam);
        void addNewBlock(Block blk);
        Block& getBlock(unsigned int bl_id);

        Block& getBlockAt(vec2 position);

        int isBlockAt(vec2 position) const;
        const Block& getBlockAt(vec2 position) const;

        const int getBlockCount() const { return blocks.size(); };
        
        std::vector<Block> blocks;
    private:
    };


#pragma endregion

#endif