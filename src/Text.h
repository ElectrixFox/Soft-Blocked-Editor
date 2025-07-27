#ifndef TEXT_H
#define TEXT_H
#pragma once
#include "Drawable.h"

struct Character_Table
    {
    std::vector<unsigned int> chrid;
    std::vector<unsigned int> rid;

    std::vector<char> chrs;
    };

struct Text_Table
    {
    std::vector<unsigned int> txid;
    std::vector<unsigned int> trsid;

    std::vector<std::string> txt;
    };

struct Text_Manager
    {
    Text_Manager(RenderPacket& in_text_rp) : text_rp(in_text_rp) {}

    RenderPacket& text_rp;   // the renderables are indexed a-z 0-9
    };

Character_Table InitialiseCharacterTable(RenderDetails& rds);

/**
 * Creates a letter renderable object
 * 
 * @param text_rp A reference to the text render packet
 * @param letter The character to render
 * @param position The position to put the letter
 * @param scale The scale of the letter
 * 
 * @deprecated This is superseded by AddCharacter
 */
void AddLetter(RenderPacket& text_rp, char letter, vec2 position, float scale);

/**
 * Creates a letter renderable object
 * 
 * @param text_man A reference to the text manager
 * @param letter The letter to render
 * @param position The position to put the letter
 * @param scale The scale of the letter
 * 
 * @deprecated This is superseded by AddCharacter
 */
void AddLetter(Text_Manager& text_man, char letter, vec2 position, float scale);

/**
 * Creates a character renderable object
 * 
 * @param text_rp A reference to the text render packet
 * @param ch The character to render
 * @param position The position to put the letter
 * @param scale The scale of the letter
 * 
 * @returns The render ID of the new character
 */
unsigned int AddCharacter(RenderPacket& text_rp, char ch, vec2 position, float scale);


#endif