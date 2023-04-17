#ifndef JERRY_H_
#define JERRY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"
#include "ctype.h"

/// this struct represents a planet, the planet has a name and coordinates
typedef struct planet_s
{
    char * name;
    double cord_x;
    double cord_y;
    double cord_z;
}Planet;

/// this struct represents an origin (for a jerry) meaning a dimension name and a planet
typedef struct origin_s
{
    Planet planet;
    char * dim_name;
}Origin;

/// this struct represents a physical characteristic, it holds the characteristic name and value
typedef struct Characteristic
{
    char * name;
    float value;
}PhysicalCharacteristic;

/// this struct represents a Jerry, each jerry has an id, name, origin, physical characteristic(/s)
typedef struct jerry_s
{
    char * id;
    int happiness;
    Origin * origin;
    PhysicalCharacteristic ** phy_char;
    int num_of_chars;
}Jerry;

/** This function initialises a Jerry
  it receives the following parameters, allocate memory for the jerry and create him according to the parameters */
Jerry * initJerry(char id[], char dim_name[], Planet *planet, int happiness);

/** This function initialises a Planet
  it receives its name and coordinates and allocates memory for the planet and creates it according to the parameters */
Planet * initPlanet(char [], double, double, double);

/** this func allocates memory to a new physical characteristic and initialize it according to the parameters
   received, name and value */
PhysicalCharacteristic * initChara(char [], float);

/** This function initialises an Origin, it receives the Planet it belongs to and its dimension name
    it allocates memory for the origin and creates it according to the parameters */
Origin * initOrigin(Planet *, char []);

/// this function checks if a jerry has a specific physical characteristic
bool checkCharaInJerry(Jerry, char []);

/// This function add a PhysicalCharacteristic to a jerry at the end of the dynamic array
status addChara(Jerry *, PhysicalCharacteristic *);

/** This function deletes a specific characteristic from jerry PhysicalCharacteristic array
    if jerry doesn't have that characteristic it will return failure */
status deleteChara(Jerry *, char []);

/** This function is used to free up a planet
    receives an element because its used as function in a generic adt (same for next generic functions) */

status destroyPlanet(Element);

/// This function is used to free up an origin
void destroyOrigin(Origin *);

/// This function is used to free up a PhysicalCharacteristic
status destroyChara(Element);

/// This function is used to free up a Jerry, uses the other destroy functions
status destroyJerry(Element);

/// This function is used to print a jerry
status printJerry(Element);
/// This function is used to print a planet
status printPlanet(Element);

/// This function copies a Jerry (a shallow copy we don't want duplicates) and returns it
Element copyJerry(Element);

/// This function copies a Planet (a shallow copy - we don't want duplicates) and returns it
Element copyPlanet(Element);

/// This function compares two jerries by their id, if they're equal returns true, otherwise false
bool eqJerry(Element, Element);

/// This function compares two Planets by their name, if they're the same returns true, false otherwise
bool eqPlanet(Element, Element);

/// This function receives a planet and a name and returns true if the planet holds that name
bool nameInPlanet(Element, Element);

/// This function receives a planet and an id and returns true if jerry has that id
bool idInJerry(Element elem, Element key);


#endif /* JERRY_H_ */

