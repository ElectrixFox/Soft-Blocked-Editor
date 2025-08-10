#include "Text.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "include/stb/stb_truetype.h"

unsigned char ttf_buffer[1 << 20];
unsigned char tmp_bitmap[512 * 512];

stbtt_bakedchar cdata[96];
unsigned int ftex;

std::vector<char> chrs;
std::vector<unsigned int> ids;

std::map<char, unsigned int> chtxpair;

const char* fontfp = "res/font/fnt.ttf";

unsigned int getCharaTex(char ch)
{
printf("\n%c", ch);
unsigned char dat[1 << 20];
FILE* file = fopen(fontfp, "rb");
fread(dat, 1, 1 << 20, file);
fclose(file);
stbtt_fontinfo font;
int offset = stbtt_GetFontOffsetForIndex((const unsigned char*)dat, 0);
stbtt_InitFont(&font, (const unsigned char*)dat, offset);
float hscale = stbtt_ScaleForPixelHeight(&font, 128.0f); // 128 pixels tall
int w, h, xoff, yoff;
unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, hscale,
    (int)ch, &w, &h, &xoff, &yoff);

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// creating a texture
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    w,
    h,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    bitmap
);
// set texture options
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

stbtt_FreeBitmap(bitmap, 0);

return texture;
}

void initfont()
{
// const char* dat = readFile("C:/Windows/Fonts/COLONNA.TTF");
unsigned char dat[1 << 20];
FILE* file = fopen(fontfp, "rb");
fread(dat, 1, 1 << 20, file);
fclose(file);
stbtt_fontinfo font;
int offset = stbtt_GetFontOffsetForIndex((const unsigned char*)dat, 0);
stbtt_InitFont(&font, (const unsigned char*)dat, offset);
float hscale = stbtt_ScaleForPixelHeight(&font, 128.0f); // 128 pixels tall

int strt = (int)'A', end = (int)'Z' + 1;
for (int i = strt; i < end; i++)
    {
    int w, h, xoff, yoff;
    unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, hscale,
        (char)i, &w, &h, &xoff, &yoff);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // creating a texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        w,
        h,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbtt_FreeBitmap(bitmap, 0);

    texture = SetTextureVariable(texture, 1);

    // unsigned int tex = getCharaTex((char)i);
    chtxpair.insert(std::pair<char, unsigned int>((char)i, texture));
    }

strt = (int)'0', end = (int)'9' + 1;
for (int i = strt; i < end; i++)
    {
    int w, h, xoff, yoff;
    unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, hscale,
        (char)i, &w, &h, &xoff, &yoff);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // creating a texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        w,
        h,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbtt_FreeBitmap(bitmap, 0);

    // unsigned int tex = getCharaTex((char)i);
    chtxpair.insert(std::pair<char, unsigned int>((char)i, texture));
    }
}

unsigned int getChar(char ch)
{
return chtxpair.find(ch)->second;
}
