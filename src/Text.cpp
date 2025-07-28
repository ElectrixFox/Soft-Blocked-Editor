#include "Text.h"

#pragma region Revamp

Character_Table InitialiseCharacterTable(RenderDetails& rds)
{
const char* textsheet = "res/sprites/general_text_tilesheet_black_plan.png";
const int nchars = 36;

Character_Table ch_tab;

int nstrt = 10; // digits are below 10

for (int i = 0; i < nchars; i++)    // loop through the character set
    {
    unsigned int rid = CreateTextRenderable(rds, textsheet, nchars, i + 1); // creating the character
    char ch;

    if(i < nstrt)  // if i is in the digit zone
        ch = (char)((int)'9' - i);
    else    // if i is not in the digit zone start from A
        ch = (char)((int)'Z' - (i - nstrt));

    ch_tab.chrs.push_back(ch);   // setting the character part
    ch_tab.rid.push_back(rid);  // setting the render ID part
    }

return ch_tab;
}

unsigned int findCharacterRenderID(Character_Table ch_tab, char ch)
{
const int nchars = 36;

for (int i = 0; i < nchars; i++)
    if(ch_tab.chrs[i] == ch)
        return ch_tab.rid[i];

printf("\nERROR: Could not find the character %c in the character table", ch);
exit(1);
}

Text_Table InitialiseTextTable()
{
Text_Table tx_tab;

return tx_tab;
}

unsigned int AddText(Text_Table& txt_tab, TransformationDetails& tds, const char* str, vec2 pos)
{
static unsigned int id = 0;
const float scale = 25.0f;

unsigned int trsid = AddTransformation(tds, pos, {scale, scale}, 0.0f); // creating the transform

// adding all of the details
txt_tab.trsid.push_back(trsid);
txt_tab.txid.push_back(id);
txt_tab.txt.push_back(str);

return id++;
}

void CombineText(Character_Table& ch_tab, Text_Table& txt_tab, Drawables& drabs, unsigned int text_id)
{
int index = -1;
for (int i = 0; i < txt_tab.txid.size(); i++)   // finding the text ID
    if(txt_tab.txid[i] == text_id)
        {
        index = i;
        break;
        }

if(index == -1) // error checking
    {
    printf("\nERROR: Could not find the text ID %d to combine", text_id);
    exit(1);
    }

unsigned int trsid = txt_tab.trsid[index];  // getting the transform ID of the first charcter in the text
std::string txt = txt_tab.txt[index];   // getting the text to show

for (char c : txt)  // for each character in the string
    {
    unsigned int rid = findCharacterRenderID(ch_tab, c);    // getting the render ID
    printf("\n%c -> %d", c, rid);
    AddDrawable(drabs, trsid, rid); // creating the new drawable
    }
}

void DrawText(Text_Table& txt_tab, RenderDetails& rds, TransformationDetails& tds, Drawables& drabs)
{
DrawDrawablesInstancedPosition(rds, tds, drabs, 50.0f);
}

#pragma endregion

#pragma region Old

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

#pragma endregion

