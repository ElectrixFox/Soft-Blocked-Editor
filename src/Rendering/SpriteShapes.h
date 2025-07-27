#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <vector>
#include <iostream>

typedef enum SHAPE
    {
    SHAPE_SQUARE = 0,
    SHAPE_TEXT,
    } SHAPE;

struct SpriteSheetInfo
    {
    const char* spfp;
    unsigned int nosp;
    unsigned int spr;

    SpriteSheetInfo() {}
    SpriteSheetInfo(const char* nspfp, unsigned int nnosp, unsigned int nspr)
        : spfp(nspfp), nosp(nnosp), spr(nspr)
        { }
    };

template <typename T>
struct viBundle
    {
    T* vi;
    const unsigned int n;
    };


/*
The shape variable is of this form
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0| Num S | Sprite| Shape 
*/
void SetNumberOfSprites(unsigned int* shape, unsigned int numofspr);
void SetActiveSprite(unsigned int* shape, unsigned int sprite);
void SetActiveShape(unsigned int* shape, unsigned int sh);

/**
 * A compactation of setting the number of sprites, the active sprite and the active shape
 */
void GeneralInitialise(unsigned int* shape, unsigned int numofspr, unsigned int sprite, SHAPE sh);

unsigned int GetNumberOfSprites(unsigned int shape);
unsigned int GetActiveSprite(unsigned int shape);
unsigned int GetActiveShape(unsigned int shape);

viBundle<float> GetShapeVertices(unsigned int shape);
viBundle<unsigned int> GetShapeIndices(unsigned int shape);
