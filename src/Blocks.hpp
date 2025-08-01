#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#pragma once
#include "Drawable.h"
#include "InputManager.h"
#include "Text.h"
#include <iostream>
#include <algorithm>
#include <string>

struct BLOCK_PLAYER
    {
       
    };

struct BLOCK_IMMOVABLE_BLOCK
    {
    
    };

template<typename T>
struct Block_Entity_Table
    {
    std::vector<unsigned int> blk_id;
    std::vector<unsigned int> trsid;

    std::vector<T> blocks;
    };

struct Block_Manager
    {
    
    Block_Entity_Table<BLOCK_PLAYER> blk_plr;
    Block_Entity_Table<BLOCK_IMMOVABLE_BLOCK> blk_imm;
    };


#endif