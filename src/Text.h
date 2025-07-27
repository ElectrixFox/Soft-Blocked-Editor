#ifndef TEXT_H
#define TEXT_H
#pragma once
#include "Drawable.h"

class TextManager
    {
    public:
        TextManager();

        void AddChar(char letter, vec2 position, float scale);

        void AddText(const char* text, vec2 position, float scale);

        void RenderText();
    
    private:
        RenderPacket text_rp;   // the renderables are indexed a-z 0-9
    };

struct Text_Manager
    {
    Text_Manager(RenderPacket& in_text_rp) : text_rp(in_text_rp) {}

    RenderPacket& text_rp;   // the renderables are indexed a-z 0-9
    };


void AddLetter(Text_Manager& text_man, char letter, vec2 position, float scale);
void AddLetter(RenderPacket& text_rp, char letter, vec2 position, float scale);



#endif