#pragma once
#include "Rendering/Buffer.h"
#include "Utils.hpp"
#include <vector>
#include <string>

struct RenderDetails
    {
    std::vector<unsigned int> rid;
    std::vector<unsigned int> vao;
    std::vector<unsigned int> vbo;
    std::vector<unsigned int> ibo;
    std::vector<unsigned int> shader;
    std::vector<unsigned int> texture;
    };
typedef struct RenderDetails RenderDetails;

/**
 * Initialises a new RenderDetails object
 * 
 * @returns A new initialised RenderDetails object
 */
RenderDetails InitialiseRenderDetails();

/**
 * Returns the index of the render detail
 * 
 * @param rd The render details object
 * @param rid The ID of the details to find
 * 
 * @returns Index of ID in the details object or -1 if not found
 */
int getRenderDetailsIDIndex(RenderDetails rd, unsigned int rid);

/**
 * Adds a render object to the details
 * 
 * @param rd A reference to the details
 * @param vao The new vao to add
 * @param vbo The new vbo to add
 * @param ibo The new ibo to add
 * @param shader The new shader to add
 * @param texture The new texture to add
 * 
 * @returns The ID of the newly added detail object
 */
unsigned int AddRenderDetail(RenderDetails& rd, unsigned int vao, unsigned int vbo, unsigned int ibo, unsigned int shader, unsigned int texture);

/**
 * Removes a render object from the details
 * 
 * @param rd A reference to the details
 * @param rid The ID of the object to remove
 */
void RemoveRenderDetail(RenderDetails& rd, unsigned int rid);

#pragma region Functions

unsigned int CreateSquareRenderable(RenderDetails& rd);

/**
 * Creates a renderable object from a sprite sheet
 * 
 * @param rd Reference the the render details
 * @param spfp File path to the sprite sheet
 * @param nosp Number of sprites on the sheet
 * @param spr The sprite to render
 * 
 * @warning This only works for sprite sheets 75x75n where n is the number of sprites - it has only been tested in this case
 * 
 * @returns The ID of the render object
 */
unsigned int CreateSpriteRenderable(RenderDetails& rd, std::string spfp, unsigned int nosp, unsigned int spr);

/**
 * Just creates a standard coloured square
 * 
 * @param rd Reference the the render details
 * 
 * @returns The ID of the render object
 */
unsigned int CreatePlainSquareRenderable(RenderDetails& rd);

void DrawRenderable(RenderDetails rds, unsigned int rid);

/**
 * Gets the programs of the given render IDs
 * 
 * @param rds The render details table
 * @param rids The IDs of the renderables to get the programs for
 * 
 * @returns The programs of the render IDs
 */
unsigned int* getRenderablePrograms(RenderDetails rds, std::vector<unsigned int> rids);

/**
 * Outputs the render details
 * 
 * @param rds The rendering table
 */
void OutputRenderDetails(RenderDetails rds);

#pragma endregion