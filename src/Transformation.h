#pragma once
#include "src/MathsOperations.h"

struct TransformationDetails
    {
    unsigned int* trsid;
    vec2* pos;
    vec2* scale;
    float* angle;
    unsigned int size;
    };
typedef struct TransformationDetails TransformationDetails;

/**
 * Initialises the transformation details to enable adding and removing easily
 * 
 * @param width A pointer to the width of the window
 * @param height A pointer to the height of the window
 * 
 * @returns Newly initialised TransformationDetails object
 */
TransformationDetails InitialiseTransformationDetails();

/**
 * Returns the index of the transformation
 * 
 * @param tds The details about the transformation
 * @param trsid The ID of the transformation to find
 * 
 * @returns Index of ID in the details object or -1 if not found
 */
int getTransformationIDIndex(TransformationDetails tds, unsigned int trsid);

/**
 * Adds a transformation to the details
 * 
 * @param tds A pointer to the details
 * @param pos The new position to add
 * @param scale The new scale to add
 * @param theta The angle (in degrees) to rotate
 * 
 * @returns The ID of the newly added transformation
 */
unsigned int AddTransformation(TransformationDetails* tds, vec2 pos, vec2 scale, float theta);

/**
 * Removes a transformation from the details
 * 
 * @param tds A pointer to the details
 * @param tid The ID of the object to remove
 */
void RemoveTransformation(TransformationDetails* tds, unsigned int tid);

#pragma region Functions

/**
 * Sets the position of the transform
 * 
 * @param tds The details about the transformation
 * @param trsid The transformation ID
 * @param newpos The new position to apply
 */
void setPosition(TransformationDetails tds, unsigned int trsid, vec2 newpos);

/**
 * Gets the position of the transform
 * 
 * @param tds The details about the transformation
 * @param trsid The transformation ID
 * 
 * @returns The position of the transform
 */
vec2 getPosition(TransformationDetails tds, unsigned int trsid);

/**
 * Translates the transform
 * 
 * @param tds The transformations table
 * @param trsid The ID of the transformation
 * @param trans The translation to apply 
 */
void applyTranslation(TransformationDetails tds, unsigned int trsid, vec2 trans);

/**
 * Sets the scale of the transform
 * 
 * @param tds The details about the transformation
 * @param trsid The transformation ID
 * @param newscale The new scale to apply
 */
void setScale(TransformationDetails tds, unsigned int trsid, vec2 newscale);

/**
 * Gets the scale of the transform
 * 
 * @param tds The details about the transformation
 * @param trsid The transformation ID
 * 
 * @returns The scale of the transform
 */
vec2 getScale(TransformationDetails tds, unsigned int trsid);

/**
 * Sets the rotation of the transform
 * 
 * @param tds The transformation array
 * @param trsid The transformaion ID
 * @param theta The angle (in radians)
 */
void setRotation(TransformationDetails tds, unsigned int trsid, float theta);

/**
 * Gets the model matrix for the transformation
 * 
 * @param tds The details of the transformation object
 * @param tid The ID of the transform record
 * 
 * @returns The model matrix for the relevant transform
 */
m4 getTransformModelMatrix(TransformationDetails tds, unsigned int tid);

/**
 * Gets all of the transformation matrices
 * 
 * @param tds The transformation details
 * @param models An array of m4 matrices (this array will be the size of tds.size)
 */
void getTransformModelMatrices(TransformationDetails tds, m4* models);

/**
 * Checks if a transformation object has been pressed
 * 
 * @param tds The details about the transform
 * @param tid The ID of the transformation
 * @param cursorpos The position of the cursor
 * 
 * @returns 1 if pressed and 0 if not pressed
 */
int CheckPressed(TransformationDetails tds, unsigned int tid, vec2 cursorpos);

/**
 * Checks if no objects have been pressed pressed
 * 
 * @param trds The transformations table
 * @param curpos The position of the cursor
 * 
 * @returns 1 if anything is pressed, 0 if not
 */
int PressedNothing(TransformationDetails trds, vec2 curpos);

/**
 * Checks if multiple objects have pressed at once
 * 
 * @param trds The transformations table
 * @param curpos The position of the cursor
 * 
 * @returns 1 if something else is pressed too, 0 if not
 */
int PressedAnother(TransformationDetails trds, vec2 curpos);

/**
 * Checks if there is a block which will overlap the given range
 * 
 * @param trds The transformations table
 * @param curpos The position of the cursor
 * @param range The range to check
 * 
 * @returns 1 if the area is not empty, 0 if it is empty
 */
int PressedArea(TransformationDetails trds, vec2 curpos, float range);

/**
 * Gets the pressed block
 * 
 * @param trds The transformation table
 * @param curpos The position of the cursor
 * 
 * @returns The transform ID of the pressed item
 * 
 * @warning Does not verify that a block has been pressed and hence should be only used when one has been pressed
 */
unsigned int getPressedBlock(TransformationDetails trds, vec2 curpos);

/**
 * Gets the transformation ID of the transformation at the position
 * 
 * @param tds The transformation details
 * @param pos The position to find the transformation at
 * 
 * @returns The ID of the transformation or -1 if not found
 */
int getTransformAt(TransformationDetails tds, vec2 pos);

/**
 * Outputs the transformations table
 * 
 * @param tds The table of transformations
 */
void OutputTransformations(TransformationDetails tds);

#pragma endregion
