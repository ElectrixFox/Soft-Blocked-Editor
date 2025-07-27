#include "Text.h"

TextManager::TextManager()
{
const int ntext = 36;
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";

text_rp = InitialiseRenderPacket();

for (int i = 1; i <= ntext; i++)    // going through and creating all of the render objects
    {
    unsigned int rid = CreateSpritesheetRenderable(text_rp.rds, textsheet, 40, 1440, ntext, i);
    }
}

void TextManager::AddChar(char letter, vec2 position, float scale)
{
unsigned int trsid = AddTransformation(text_rp.tds, position, {scale, scale}, 0.0f);
int index = letter - (int)'Z';
AddDrawable(text_rp.drabs, trsid, text_rp.rds.rid[index]);
}

void TextManager::AddText(const char* text, vec2 position, float scale)
{
/*
for (int i = 0; i < strlen(text); i++)
    {
    unsigned int trsid = AddTransformation(text_rp.tds, position, {scale, scale}, 0.0f);
    AddDrawable(text_rp.drabs, trsid, rid);
    }
*/
}


void TextManager::RenderText()
{
DrawRenderPacket(text_rp);
}

void AddLetter(Text_Manager& text_man, char letter, vec2 position, float scale)
{
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";
const int nchars = 36;
int ltr = (int)letter - (int)'Z';

unsigned int rid = CreateSpritesheetRenderable(text_man.text_rp.rds, textsheet, 40, 1440, nchars, ltr, 1);
unsigned int trsid = AddTransformation(text_man.text_rp.tds, position, {scale, scale}, 0.0f);

AddDrawable(text_man.text_rp.drabs, trsid, rid);
}

void AddLetter(RenderPacket& text_rp, char letter, vec2 position, float scale)
{
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";
const int nchars = 36;
int ltr = (int)letter - (int)'Z';
ltr = 1;

unsigned int rid = CreateTextRenderable(text_rp.rds, textsheet, 1);
unsigned int trsid = AddTransformation(text_rp.tds, position, {scale, scale}, 0.0f);

AddDrawable(text_rp.drabs, trsid, rid);
}

