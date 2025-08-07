#ifndef ENTITY_HPP
#define ENTITY_HPP
#pragma once
#include "Renderer.hpp"
#include "Camera.hpp"
#include <functional>

#pragma region Entity

struct Entity
    {
    Entity();

    void draw();
    std::function<void (Entity&)> update();
    };

#pragma endregion

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

BLOCK_TYPE getBlockTypeFromSSI(SpriteSheetInfo ssi);

/**
 * Gets the block information for the immovable block
 * 
 * @returns The block information
 */
SpriteSheetInfo getImmovableBlock(BLOCK_IM_STATE state);

struct Block
    {
    Block(BLOCK_TYPE btype, vec2 position);

    BLOCK_TYPE type;
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
    std::function<void (Block& blk)> update = nullptr;

    // action stuff
    int clicked = 0;
    };

class Block_Manager
    {
    public:
        Block_Manager()
            : blocks()
            {}
        void drawBlocks(Camera cam);
        void addNewBlock(Block blk);
        void removeBlock(unsigned int bl_id);

        Block& getBlock(unsigned int bl_id);

        Block& getBlockAt(vec2 position);

        int isBlockAt(vec2 position) const;
        const Block& getBlockAt(vec2 position) const;

        const int getBlockCount() const { return blocks.size(); };

        int hasPressedBlock(vec2 pos) const;
        
        std::vector<Block> blocks;
    private:
        int findBlockIndex(const Block& blk);
    };

#pragma endregion

#pragma region UI Object

enum UI_ELEMENT_TYPE
    {
    UI_BUTTON,
    UI_MENU
    };

struct UI_Element
    {
    UI_Element(UI_ELEMENT_TYPE in_type, vec2 position);
    UI_Element(UI_ELEMENT_TYPE in_type, vec2 position, const char* spfp, int nosp, int spr);

    const UI_ELEMENT_TYPE type;
    unsigned int ui_id;

    // rendering stuff
    int render = 1; // should the object be drawn
    RenderObject rend_obj;  // the rendering stuff for it
    SpriteSheetInfo ssi;    // the actual sprite crap

    // transformation stuff
    vec2 pos;
    vec2 scale;
    float angle;

    // menu details
    std::vector<unsigned int> entries;
    int folded = 0;
    int lrud = 0;   // does it unfold left, right, up or down (0, 1, 2, 3)

    // runtime stuff
    void draw();
    std::function<void (UI_Element& ele)> update = nullptr;

    // action stuff
    int clickable = 0;
    std::function<void (UI_Element& ele)> onclick = nullptr;

    int hoveract = 0;
    int hovering = 0;
    std::function<void (UI_Element& ele)> onhover = nullptr;
    };

class UI_Manager
    {
    public:
        UI_Manager()
            : elements()
            {}

        void drawElements(Camera cam);
    
        void addNewElement(UI_Element ele);
        
        UI_Element& getElement(unsigned int ui_id);
        const UI_Element& getElement(unsigned int ui_id) const;

        const int getElementCount() const { return elements.size(); };

        /**
         * Checks if the passed element has been pressed
         * 
         * @param ele The element to query
         * @param cpos The screen space position of the cursor
         * 
         * @return 1 if has pressed and 0 if not (2 if pressed the head of the menu)
         */
        int hasPressedElement(const UI_Element& ele, vec2 cpos) const;

        int hasPressedUI(vec2 cpos) const;

        void checkUIInput();
        
        std::vector<UI_Element> elements;
    private:
    };

#pragma endregion

#endif