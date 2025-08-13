#ifndef TEXT_H
#define TEXT_H
#pragma once
#include "Drawable.h"

#pragma region Revamp

struct Character_Table
    {
    std::vector<unsigned int> chrid;
    std::vector<unsigned int> rid;

    std::vector<char> chrs;

    Character_Table(RenderDetails& text_rds);

    RenderDetails& rds;
    };

struct Text_Table
    {
    std::vector<unsigned int> txid;
    std::vector<unsigned int> trsid;

    std::vector<std::string> txt;
    };

/**
 * Initialises all of the characters and creates their renderables
 * 
 * @param rds A reference to the render details for the character table
 * 
 * @returns The new character table
 */
Character_Table InitialiseCharacterTable(RenderDetails& rds);

void InitialiseCharacterTable(Character_Table& ch_tab);

/**
 * Gets a new text table
 * 
 * @returns A new text table
 */
Text_Table InitialiseTextTable();

/**
 * Finds the render ID of a given character
 * 
 * @param ch_tab The character table
 * @param ch The character to find
 * 
 * @returns The render ID of the given character
 * 
 * @warning Will exit and give an error code on incorrect input
 */
unsigned int findCharacterRenderID(Character_Table ch_tab, char ch);

/**
 * Adds some given text to the text table
 * 
 * @param txt_tab A reference to the text table
 * @param tds A reference to the transform table
 * @param str The contents of the text
 * @param pos The position of the first character
 * 
 * @returns The text ID of the new text element
 */
unsigned int AddText(Text_Table& txt_tab, TransformationDetails& tds, const char* str, vec2 pos);

/**
 * Gets the transform ID for the given text ID
 * 
 * @param txt_tab The text table
 * @param text_id The ID of the text to find
 * 
 * @returns The transform ID of the text element
 * 
 * @warning Will exit and give an error code on incorrect input
 */
unsigned int getTextTransformID(Text_Table& txt_tab, unsigned int text_id);

/**
 * Combines the text into a drawable
 * 
 * @param ch_tab A reference to the characters table
 * @param txt_tab A reference to the text table
 * @param drabs A reference to the drawables
 * @param text_id The ID of the text to combine
 */
void CombineText(Character_Table& ch_tab, Text_Table& txt_tab, Drawables& drabs, unsigned int text_id);

/**
 * Draws the text
 * 
 * @param txt_tab A reference to the text table
 * @param rds A reference to the render details
 * @param tds A reference to the transform table
 * @param drabs A reference to the drawables
 */
void DrawText(Text_Table& txt_tab, RenderDetails& rds, TransformationDetails& tds, Drawables& drabs);

/**
 * To create text:
 * - first initialise both tables
 * - use the add text fuction to add the text to the table
 * - use the combine text to put it all together
 * - finally call draw text to draw it all
 * 
 */



#pragma endregion

#pragma region Old

struct Text_Manager
    {
    Text_Manager(RenderPacket& in_text_rp) : text_rp(in_text_rp) {}

    RenderPacket& text_rp;   // the renderables are indexed a-z 0-9
    };


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

#pragma endregion

#endif