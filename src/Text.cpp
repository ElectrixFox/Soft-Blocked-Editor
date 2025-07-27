#include "Text.h"

void AddLetter(RenderPacket& text_rp, char letter, vec2 position, float scale)
{
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";
const int nchars = 36;
int ltr = nchars - ((int)letter - (int)'A');
unsigned int rid = CreateTextRenderable(text_rp.rds, textsheet, nchars, ltr);
unsigned int trsid = AddTransformation(text_rp.tds, position, {scale, scale}, 0.0f);

AddDrawable(text_rp.drabs, trsid, rid);
}

void AddLetter(Text_Manager& text_man, char letter, vec2 position, float scale) { AddLetter(text_man.text_rp, letter, position, scale); }

unsigned int AddCharacter(RenderPacket& text_rp, char ch, vec2 position, float scale)
{
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";
const int nchars = 36;
int pt = 0; // the point of the character in the character set

if(isdigit(ch))
    pt = (((int)'9' + 1) - (int)ch);
else if(isalpha(ch))
    pt = nchars - ((int)ch - (int)'A');

unsigned int rid = CreateTextRenderable(text_rp.rds, textsheet, nchars, pt);
unsigned int trsid = AddTransformation(text_rp.tds, position, {scale, scale}, 0.0f);

AddDrawable(text_rp.drabs, trsid, rid);

return rid;
}

