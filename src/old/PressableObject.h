#pragma once
#include "Transformation.h"
#include "Input.h"

enum BACT {
    BACT_DELETE = 1,
    BACT_SWITCH = 2
};
typedef enum BACT BACT;

typedef struct PressableDetails {
    unsigned int* prid;
    unsigned int* trsid;
    unsigned int* pract;
    unsigned int size;
} PressableDetails;

/**
 * Initialises the pressable details to enable adding and removing easily
 * 
 * @returns Newly initialised PressableDetails object
 */
PressableDetails InitialisePressableDetails();

/**
 * Returns the index of the pressable
 * 
 * @param prds The details about the pressable
 * @param prid The ID of the pressable to find
 * 
 * @returns Index of ID in the details object or -1 if not found
 */
int getPressableIDIndex(PressableDetails prds, unsigned int prid);

/**
 * Finds the index of the given transform ID in the pressables table
 * 
 * @param prds The pressables table
 * @param trid The transform ID to find
 * 
 * @returns -1 if not found and the index otherwise
 */
int findPressableTransfom(PressableDetails prds, unsigned int trid);

/**
 * Finds the action of the pressable
 * 
 * @param prds The details about the pressable
 * @param prid The ID of the pressable to find
 * 
 * @returns The action of the pressable
 */
unsigned int getPressableAction(PressableDetails prds, unsigned int prid);

/**
 * Adds a pressable to the array
 * 
 * @param prds A pointer to the details
 * @param tid The transformation detail of the object to press
 * @param pract The action performed when the object/block is pressed (clicked)
 * 
 * @returns The ID of the newly added pressable
 */
unsigned int AddPressable(PressableDetails* prds, unsigned int tid, unsigned int pract);

/**
 * Removes a pressable from the details
 * 
 * @param prds A pointer to the details
 * @param prid The ID of the object to remove
 */
void RemovePressable(PressableDetails* prds, unsigned int prid);

/**
 * Gets the number of pressables with the given action
 * 
 * @param prds The pressables table
 * @param pract The action to find
 * 
 * @returns The number of that action in the pressable table
 */
unsigned int getActionCount(PressableDetails prds, unsigned int pract);

/**
 * Gets the pressable IDs of the entries for the given action
 * 
 * @param prds The pressable details table
 * @param pract The action of the entities to get 
 * 
 * @returns The pressable IDs of the entities
 * @warning The first element contains the size of the array (so it will need to be accounted for)
 */
unsigned int* getPressablesWithAction(PressableDetails prds, unsigned int pract);

/**
 * Gets the transformation IDs of the entries for the given action
 * 
 * @param prds The pressable details table
 * @param pract The action of the entities to get 
 * 
 * @returns The transformaton IDs of the entities
 * @warning The first element contains the size of the array (so it will need to be accounted for)
 */
unsigned int* getPressablesTransformWithAction(PressableDetails prds, unsigned int pract);

/**
 * Gets the transformation IDs of the entries without the given action
 * 
 * @param prds The pressable details table
 * @param pract The action of the blocks to ignore
 * 
 * @returns The transformaton IDs of the entities
 * @warning The first element contains the size of the array (so it will need to be accounted for)
 */
unsigned int* getPressablesTransformWithoutAction(PressableDetails prds, unsigned int pract);

/**
 * Checks if the entity with eid has been pressed
 * 
 * @param poses The array of entity positions
 * @param scales Array of entity scales
 * @param eid The id of the entity to check
 */
int _CheckPressed(vec2* poses, vec2* scales, vec2 cursorpos, unsigned int eid);

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
 * Checks if there is a block (with a given action) which will overlap the given range
 * 
 * @param prds The pressables table
 * @param trds The transformations table
 * @param curpos The position of the cursor
 * @param range The range to check
 * @param pract The action of the blocks to check
 * 
 * @returns 1 if the area is not empty, 0 if it is empty
 */
int PressedAreaAction(PressableDetails prds, TransformationDetails trds, vec2 curpos, float range, unsigned int pract);

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
 * Gets all of the pressed blocks in a given area
 * 
 * @param prds The pressables table
 * @param trds The transformation table
 * @param curpos The position of the cursor
 * 
 * @returns An array of pressable IDs for the pressed objects
 * 
 * @warning The first element is the size of the array and should be accounted for
 */
unsigned int* getPressedBlocksArea(PressableDetails prds, TransformationDetails trds, vec2 curpos, float range);

/**
 * Gets the action of the pressed block
 * 
 * @param prds The pressable table
 * @param trds The transformations table
 * @param curpos The position of the cursor
 * 
 * @returns The action of the pressed block
 */
unsigned int getPressedBlockAction(PressableDetails prds, TransformationDetails trds, vec2 curpos);

/**
 * Sets the action for the pressable
 * 
 * @param prds A pointer to the details
 * @param prid The ID of the object to remove
 * @param pract The action to set
 */
void SetPressableAction(PressableDetails* prds, unsigned int prid, unsigned int pract);

/**
 * Output function for debugging
 */
void OutputPressables(PressableDetails prds);