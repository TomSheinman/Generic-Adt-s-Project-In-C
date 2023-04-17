#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "LinkedList.h"

/** this function receives a linked list shallow copies it and returns its copy
   we need this function to create a LinkedList
   the shallow copy is because we want to add to the actual link list and not to its copy
   also the elements inside the linked list will be copied using the copy functions */
static Element copyLinkedList(Element elem);

struct multiValueHashTable_s{
    hashTable table;
    PrintFunction printKey;
    CopyFunction cpyVal;
    FreeFunction freeVal;
    EqualFunction eqVal;
    PrintFunction printVal;
};

static Element copyLinkedList(Element elem)
{
    return elem;
}

mulValHashTable createMultiValueHashTable(CopyFunction cK, FreeFunction fK, PrintFunction pK, CopyFunction cV, FreeFunction fV, PrintFunction pV, EqualFunction eK, EqualFunction eV, TransformIntoNumberFunction toInt, int size)
{
    if(cV == NULL || fV == NULL || pV == NULL) // other arguments are checked in createHashTable
        return NULL;
    mulValHashTable mVHashTbl = (mulValHashTable)malloc(sizeof(struct multiValueHashTable_s));
    if(mVHashTbl == NULL)
        return NULL;
    mVHashTbl->printKey = pK;
    mVHashTbl->cpyVal = cV;
    mVHashTbl->printVal = pV;
    mVHashTbl->freeVal = fV;
    mVHashTbl->eqVal = eV;
    /// values in the hash are pairs of value and linkedList
    mVHashTbl->table = createHashTable(cK, fK, pK, copyLinkedList, destroyList, displayList, eK, toInt, size);
    if(mVHashTbl->table == NULL)
    {
        destroyMultiValueHashTable(mVHashTbl);
        return NULL;
    }
    return mVHashTbl;
}

status destroyMultiValueHashTable(mulValHashTable mVHashTable)
{
    if(mVHashTable == NULL)
        return argument_failure;
    status desStatus = success;
    if(mVHashTable->table != NULL)
        desStatus = destroyHashTable(mVHashTable->table);
    free(mVHashTable);
    return desStatus;
}

status addToMultiValueHashTable(mulValHashTable mVHashTable, Element key ,Element value)
{
    if(mVHashTable == NULL || key == NULL || value == NULL)
        return argument_failure;
    LinkedList mappedToKey = (LinkedList)lookupInHashTable(mVHashTable->table, key); // element list that is mapped to the key
    if (mappedToKey == NULL) // no value is mapped to the key
    {
        LinkedList valuesList = createLinkedList(mVHashTable->cpyVal, mVHashTable->freeVal, mVHashTable->printVal, mVHashTable->eqVal, mVHashTable->eqVal);
        if(valuesList == NULL)
            return failure;
        if(appendNode(valuesList, value) != success)
            return failure;
        return addToHashTable(mVHashTable->table, key, valuesList);
    }
    for(int i = 0; i < getLengthList(mappedToKey); i++) //search if value is already in the key linkedList
    {
        if(mVHashTable->eqVal(getDataByIndex(mappedToKey, i), value) == true)
            return failure;
    }
    return appendNode(mappedToKey, value);
}

Element lookupInMultiValueHashTable(mulValHashTable mValHashTable, Element key)
{
    if(mValHashTable == NULL || key == NULL)
        return NULL;
    return lookupInHashTable(mValHashTable->table, key);
}

status removeFromMultiValueHashTable(mulValHashTable mValHashTable, Element key, Element value)
{
    if(mValHashTable == NULL || key == NULL || value == NULL)
        return argument_failure;
    Element mappedToKey = lookupInMultiValueHashTable(mValHashTable, key);
    if(mappedToKey == NULL)
        return failure;
    if(deleteNode(mappedToKey, value) != success)
        return failure; // couldn't find value in keys mapped value or some other problem in deletion
    if(getLengthList(mappedToKey) == 0)
        return removeFromHashTable(mValHashTable->table, key); // no more value mapped to key so delete key
    return success;
}

status displayMultiValueHashElementsByKey(mulValHashTable mValHashTable, Element key)
{
    if(mValHashTable == NULL || key == NULL)
        return argument_failure;
    Element keyValues = lookupInMultiValueHashTable(mValHashTable, key);
    if(keyValues == NULL)
        return failure;
    mValHashTable->printKey(key);
    displayList(keyValues);
    return success;
}


