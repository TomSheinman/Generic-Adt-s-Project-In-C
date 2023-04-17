#include "KeyValuePair.h"

struct keyValue_s
{
    Element key;
    Element value;
    CopyFunction cpyKey;
    CopyFunction cpyVal;
    FreeFunction freeKey;
    FreeFunction freeVal;
    PrintFunction printKey;
    PrintFunction printVal;
    EqualFunction eqKey;
};

keyValuePair createKeyValuePair(Element k, Element v, CopyFunction cK, CopyFunction cV, FreeFunction fK, FreeFunction fV, PrintFunction pK, PrintFunction pV, EqualFunction eK)
{
    if(k == NULL || v == NULL || cK == NULL || cV == NULL || fK == NULL || fV == NULL || pK == NULL || pV == NULL || eK == NULL)
        return NULL;
    keyValuePair keyVal = (keyValuePair) malloc(sizeof (struct keyValue_s));
    if (keyVal == NULL)
    {
        printf("Memory Problem");
        return NULL;
    }
    keyVal->cpyKey = cK;
    keyVal->cpyVal = cV;
    keyVal->freeKey = fK;
    keyVal->freeVal = fV;
    keyVal->printKey = pK;
    keyVal->printVal = pV;
    keyVal->eqKey = eK;
    keyVal->key = cK(k);
    if(keyVal->key == NULL)
    {
        keyVal->value = NULL; // so no free for unallocated element
        destroyKeyValuePair(keyVal);
        return NULL;
    }

    keyVal->value = cV(v);
    if(keyVal->value == NULL)
    {
        destroyKeyValuePair(keyVal);
        return NULL;
    }
    return keyVal;
}

status destroyKeyValuePair(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    keyValuePair keyVal = (keyValuePair)elem;
    if(keyVal->freeKey(keyVal->key) != success)
        return failure;
    if(keyVal->freeVal(keyVal->value) != success)
        return failure;
    free(keyVal);
    return success;
}

status displayValue(keyValuePair keyVal)
{
    if(keyVal == NULL)
        return argument_failure;
    if (keyVal->printVal(keyVal->value) != success)
        return failure;
    return success;
}

status displayKey(keyValuePair keyVal)
{
    if(keyVal == NULL)
        return argument_failure;
    if (keyVal->printKey(keyVal->key) != success)
        return failure;
    return success;
}

Element getValue(keyValuePair keyVal)
{
    if(keyVal == NULL)
        return NULL;
    return keyVal->cpyVal(keyVal->value);
}

Element getKey(keyValuePair keyVal)
{
    if(keyVal == NULL)
        return NULL;
    return keyVal->cpyKey(keyVal->key);
}

bool isEqualKey(Element elem, Element other)
{
    if(elem == NULL || other == NULL)
        return false;
    keyValuePair kVal = (keyValuePair)elem;
    return kVal->eqKey(kVal->key, other);
}
