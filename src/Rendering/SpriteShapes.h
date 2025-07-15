#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum SHAPE
    {
    SQUARE = 0,
    } SHAPE;

struct SpriteSheetInfo
    {
    const char* spfp;
    unsigned int nosp;
    unsigned int spr;
    };
typedef struct SpriteSheetInfo SpriteSheetInfo;

struct viBundle
    {
    union
        {
        unsigned int* indices;
        float* vertices;
        } vi;
    const unsigned int n;
    };
typedef struct viBundle viBundle;


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

viBundle GetShapeVertices(unsigned int shape);
viBundle GetShapeIndices(unsigned int shape);
