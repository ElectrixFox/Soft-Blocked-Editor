#include "SpriteShapes.h"

void SetNumberOfSprites(unsigned int* shape, unsigned int numofspr)
{
unsigned int mask = 0b111110000000000U;  // the mask for the number of sprites
*(shape) = ((*(shape) & (~mask)) | (numofspr << 10));
}

void SetActiveSprite(unsigned int* shape, unsigned int sprite)
{
unsigned int mask = 0b000001111100000U;  // the mask for the active sprite
*(shape) = ((*(shape) & ~mask) | (sprite << 5));
}

void SetActiveShape(unsigned int* shape, unsigned int sh)
{
unsigned int mask = 0b000000000011111U;  // the mask for the active sprite
*(shape) = (((*shape) & (~mask)) | sh);
}

void GeneralInitialise(unsigned int* shape, unsigned int numofspr, unsigned int sprite, SHAPE sh)
{
SetNumberOfSprites(shape, numofspr);
SetActiveSprite(shape, sprite);
SetActiveShape(shape, sh);
}

unsigned int GetNumberOfSprites(unsigned int shape)
{
unsigned int mask = 0b111110000000000U;  // the mask for the number of sprites
return ((shape & mask) >> 10);
}

unsigned int GetActiveSprite(unsigned int shape)
{
unsigned int mask = 0b000001111100000U;  // the mask for the active sprite
return ((shape & mask) >> 5);
}

unsigned int GetActiveShape(unsigned int shape)
{
unsigned int mask = 0b000000000011111U;  // the mask for the active sprite
return (shape & mask);
}

viBundle GetShapeVertices(unsigned int shape)
{
unsigned int sprites = GetNumberOfSprites(shape);
unsigned int sprite = GetActiveSprite(shape);
if(sprites == 1)    // if there is only one sprite
    sprite = 1; // default to the first sprite

// printf("\n%.2f / %.2f = %.2f", (float)sprite, (float)sprites, (float)sprite / (float)sprites);

switch (GetActiveShape(shape))  // gets the shape by masking
    {
    case SQUARE:
        const float vertices[] = {
            1.0f,  1.0f, 1.0f,      1.0f, (float)sprite / (float)sprites,
            1.0f, -1.0f, 1.0f,      1.0f, (float)(sprite - 1) / (float)sprites,
            -1.0f, -1.0f, 1.0f,     0.0f, (float)(sprite - 1) / (float)sprites,
            -1.0f,  1.0f, 1.0f,     0.0f, (float)sprite / (float)sprites
        };
        unsigned int n = sizeof(vertices) / sizeof(vertices[0]);
        float* fl = calloc(n, sizeof(float));
        for (int i = 0; i < n; i++)
            {
            fl[i] = vertices[i];
            }

        return (viBundle){fl, n};
        break;
    default:
        break;
    }

return (viBundle){NULL, 0}; // return an empty bundle if no shape matches
}

viBundle GetShapeIndices(unsigned int shape)
{
switch (GetActiveShape(shape))  // gets the shape by masking
    {
    case SQUARE:
        const unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        unsigned int n = sizeof(indices) / sizeof(indices[0]);
        unsigned int* ui = calloc(n, sizeof(unsigned int));
        for (int i = 0; i < n; i++)
            {
            ui[i] = indices[i];
            }
        return (viBundle){ui, n};
        break;
    default:
        break;
    }

return (viBundle){NULL, 0}; // return an empty bundle if no shape matches
}
