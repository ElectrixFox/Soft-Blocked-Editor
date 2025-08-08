#include "Text.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "include/stb/stb_truetype.h"

unsigned char ttf_buffer[1 << 20];
unsigned char tmp_bitmap[512 * 512];

stbtt_bakedchar cdata[96];
unsigned int ftex;

unsigned int initfont()
{
// const char* dat = readFile("C:/Windows/Fonts/COLONNA.TTF");
unsigned char dat[1 << 20];
fread(dat, 1, 1 << 20, fopen("C:/Windows/Fonts/COLONNA.TTF", "rb"));
stbtt_fontinfo font;
int offset = stbtt_GetFontOffsetForIndex((const unsigned char*)dat, 0);
stbtt_InitFont(&font, (const unsigned char*)dat, offset);
float hscale = stbtt_ScaleForPixelHeight(&font, 128.0f); // 128 pixels tall

int w, h, xoff, yoff;
unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, hscale,
    'H', &w, &h, &xoff, &yoff);

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

void Test(float x, float y, char* text)
{
glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, ftex);
   glBegin(GL_QUADS);
   while (*text) {
      if (*text >= 32 && *text < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
         glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
         glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
         glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
         glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
      }
      ++text;
   }
   glEnd();
}
