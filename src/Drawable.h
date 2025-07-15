#pragma once
#include "RenderObject.h"
#include "Transformation.h"

struct Drawables {
    unsigned int* trsids;
    unsigned int* rids;
    unsigned int size;
};
typedef struct Drawables Drawables;

Drawables InitialiseDrawables();

/**
 * Adds an entry to the drawables links
 * 
 * @param drabs A pointer to the drawables table
 * @param trid The transform ID to add
 * @param rid The render ID to add
 * 
 * @returns The index that the record was added at
 */
int AddDrawable(Drawables* drabs, unsigned int trid, unsigned int rid);

/**
 * Finds the index of the given transform ID in the drawables table
 * 
 * @param drabs The drawables table
 * @param trid The transform ID to find
 * 
 * @returns -1 if not found and the index otherwise
 */
int findDrawablesTransform(Drawables drabs, unsigned int trid);

/**
 * Finds the index of the given render ID in the drawables table
 * 
 * @param drabs The drawables table
 * @param rid The render ID to find
 * 
 * @returns -1 if not found and the index otherwise
 */
int findDrawablesRenderable(Drawables drabs, unsigned int rid);

/**
 * Gets the render IDs from an array of transform IDs
 * 
 * @param drabs The drawables table
 * @param trids The transformation IDs to find in the drawables
 * @param size The number of IDs
 * 
 * @returns An array of renderable IDs
 */
unsigned int* getRenderIDsFromTransformIDs(Drawables drabs, unsigned int* trids, unsigned int size);

void RemoveDrawable(Drawables* drabs, RenderDetails* rds, TransformationDetails* tds, unsigned int trid);

void UpdateImmovables(TransformationDetails* tds, Drawables* drabs);

void DrawDrawables(const RenderDetails rds, const TransformationDetails tds, const Drawables drabs);

/**
 * Outputs the drawables table
 * 
 * @param drabs The table of drawables
 */
void OutputDrawables(Drawables drabs);

struct RenderPacket
    {
    Drawables drabs;
    RenderDetails rds;
    TransformationDetails tds;
    };
typedef struct RenderPacket RenderPacket;

RenderPacket InitialiseRenderPacket();

/**
 * Creates a basic square at the given position with the given colour and scale
 * 
 * @param rp A pointer to the render packet
 * @param position The position to create the square at
 * @param scale The scale factor at which the square should be scaled
 * @param incol The colour to set the square (can be NULL as has a default value)
 * 
 * @returns The index of the renderable in the drawables
 */
int CreateBasicSquare(RenderPacket* rp, vec2 position, float scale, vec4 incol);

/**
 * Draws all of the items in the render packet
 * 
 * @param rp The render packet of the things to draw
 */
void DrawRenderPacket(const RenderPacket rp);

/**
 * Outputs all of the render packet details
 * 
 * @param rp The render packet
 */
void OutputRenderPacketDetails(RenderPacket rp);