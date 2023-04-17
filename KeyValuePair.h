#ifndef RICKANDMORTYADT_KEYVALUEPAIR_H
#define RICKANDMORTYADT_KEYVALUEPAIR_H
#include "Defs.h"

/// this is a generic keyValuePair
typedef struct keyValue_s * keyValuePair;

/** A function that creates a new keyValuePair
    it receives all the information about the generic key and value pair it stores and returns a pointer to the pair */
keyValuePair createKeyValuePair(Element key, Element Value, CopyFunction cpyKey, CopyFunction cpyVal, FreeFunction freeKey, FreeFunction freeValue, PrintFunction printKey, PrintFunction printValue, EqualFunction eqKey);

/// A function that deletes a key value pair and cleans its memory using the free functions received in the create
status destroyKeyValuePair(Element elem);

/// A function that receives a keyAndValuePair and prints its value using the printValue function received in the create
status displayValue(keyValuePair keyVal);

/// A function that receives a keyAndValuePair and prints its key using the printKey function received in the create
status displayKey(keyValuePair keyVal);

/// A function that receives a keyAndValuePair and returns a copy of its value using the copyValue function received in the create
Element getValue(keyValuePair keyVal);

/// A function that receives a keyAndValuePair and returns a copy of its key using the copyKey function received in the create
Element getKey(keyValuePair keyVal);

/** A function that receives a pair and a key
    returns true if the key equals to the key in the pair using the eqKey function received in the create */
bool isEqualKey(Element elem, Element other);



#endif //RICKANDMORTYADT_KEYVALUEPAIR_H
