#pragma once
#include "RenderObject.h"
#include "PressableObject.h"

struct Entities
    {
    unsigned int* eid;
    unsigned int* trsid;
    int* pressable;
    unsigned int size;
    };
typedef struct Entities Entities;

/**
 * Initialises the Entities
 * 
 * @returns Newly initialised Entities object
 */
Entities InitialiseEntities();

/**
 * Returns the index of the entity record
 * 
 * @param ents The details about the entities
 * @param eid The ID of the entity to find
 * 
 * @returns Index of ID in the details object or -1 if not found
 */
int getEntitiesIDIndex(Entities ents, unsigned int eid);

/**
 * Adds an entity to the details
 * 
 * @param ents A pointer to the Entities object
 * @param trsid The ID of the transformation object for the entity
 * @param pressable If the entity is pressable
 * 
 * @returns The ID of the newly added entity
 */
unsigned int AddEntity(Entities* ents, unsigned int trsid, int pressable);

/**
 * Removes an entity from the details
 * 
 * @param ents A pointer to the Entities object
 * @param eid The ID of the entity to remove
 */
void RemoveEntity(Entities* ents, unsigned int eid);
