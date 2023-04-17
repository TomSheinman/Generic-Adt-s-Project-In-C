#ifndef CADTPROJ_MULTIVALUEHASHTABLE_H
#define CADTPROJ_MULTIVALUEHASHTABLE_H
#include "Defs.h"

/// This is a generic multiHashTable
typedef struct multiValueHashTable_s * mulValHashTable;

/** A function that creates a multiValueHashTable.
   it receives all the information about the generic key and value pair it stores
   creates the hash table and returns a pointer to it */
mulValHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction equalVal, TransformIntoNumberFunction transformIntoNumber, int hashNumber);

/// this function receives a multiValueHashTable, deletes it and cleans its memory
status destroyMultiValueHashTable(mulValHashTable mVHashTable);

/** a function that receives a multiValueHashTable, key and value
    if the key exits in the hash table it will add the value to its list of values that are mapped to it
    if not it will create a new mapping to the key and add the key and value as pair to the hash table */
status addToMultiValueHashTable(mulValHashTable mVHashTable, Element key ,Element value);

/** A function that receives a multiValueHashTable, key and returns a list of all the values that are mapped to that key
    if the key doesn't exist a null will return */
Element lookupInMultiValueHashTable(mulValHashTable mValHashTable, Element key);

/** A function that receives a multiValueHashTable, key and value
    if the key exists in the hash it searches for the value in the key list of values and deletes it if exists
    if not it will return a failure status */
status removeFromMultiValueHashTable(mulValHashTable mValHashTable, Element key, Element value);

/** A function that receives a multiValueHashTable, key
    and prints all the values that are mapped to the key in the hash table
    if the key doesn't exists in the hash a failure status will return */
status displayMultiValueHashElementsByKey(mulValHashTable mValHashTable, Element key);

#endif //CADTPROJ_MULTIVALUEHASHTABLE_H
