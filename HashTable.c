#include "LinkedList.h"
#include "HashTable.h"
#include "KeyValuePair.h"

static FreeFunction freeKey; // saves the eqKey function sent to the hash to delete copy of key

/** This function receives an hashTable and key
   and maps a place in the hashTable for the key according to the mapping function */
static int mapKey(hashTable, Element key);

/** A function that shallow copies a keyAndValuePair element into a new element and returns it
   we need this function to create a LinkedList of pairs */
static Element copyKeyValuePair(Element elem);

/** A function that prints a keyValuePair by using displayValue and displayKey
    we need this function to create a LinkedList of pairs */
static status printKeyValuePair(Element elem);

/** A function that compares two keyValuePairs by their keys
    we need this function to create a LinkedList of pairs */
static bool isEqualPairs(Element elem, Element other);

struct hashTable_s
{
    LinkedList *table;
    CopyFunction cpyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction cpyVal;
    FreeFunction freeVal;
    PrintFunction printVal;
    EqualFunction eqKey;
    TransformIntoNumberFunction transformIntoNumber;
    int tableSize;
    int num_init_lList; // used to keep track of how many lList we need to free in case of failure in initializing them
};


static bool isEqualPairs(Element elem, Element other)
{
    if(elem == NULL || other == NULL)
        return false;
    Element otherKey = getKey((keyValuePair)other);
    bool result = isEqualKey(elem, otherKey);
    freeKey(otherKey);
    return result;
}


static Element copyKeyValuePair(Element elem)
{
    return elem;
}

static status printKeyValuePair(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    keyValuePair kVal = (keyValuePair)elem;
    if(displayKey(kVal) == failure || displayValue(kVal) == failure)
        return failure;
    return success;
}

/** A function that creates a HashTable.
   it receives all the information about the generic key and value pair it stores
   creates the hash table and returns a pointer to it */
hashTable createHashTable(CopyFunction cK, FreeFunction fK, PrintFunction pK, CopyFunction cV, FreeFunction fV, PrintFunction pV, EqualFunction eK, TransformIntoNumberFunction toInt, int size)
{
    if(cK == NULL || fK == NULL || pK == NULL || cV == NULL || fV == NULL || pV == NULL || eK == NULL || toInt == NULL)
        return NULL;
    hashTable hashTbl = malloc(sizeof (struct hashTable_s));
    if(hashTbl == NULL)
    {
        printf("Memory Problem");
        return NULL;
    }
    freeKey = fK;
    hashTbl->cpyKey = cK;
    hashTbl->freeKey = fK;
    hashTbl->printKey = pK;
    hashTbl->cpyVal = cV;
    hashTbl->freeVal = fV;
    hashTbl->printVal = pV;
    hashTbl->eqKey = eK;
    hashTbl-> transformIntoNumber = toInt;
    hashTbl->tableSize = size;
    hashTbl->table = (LinkedList *) malloc(sizeof(LinkedList) * size);
    if(hashTbl->table == NULL)
    {
        printf("Memory Problem");
        destroyHashTable(hashTbl);
        return NULL;
    }
    hashTbl->num_init_lList = 0;
    for(int i=0; i<hashTbl->tableSize; i++)
    {
        hashTbl->table[i] = createLinkedList(copyKeyValuePair, destroyKeyValuePair, printKeyValuePair,
                                             isEqualPairs, isEqualKey);
        if(hashTbl->table[i] == NULL)
        {
            destroyHashTable(hashTbl);
            return NULL;
        }
        hashTbl->num_init_lList ++;
    }
    return hashTbl;
}

/// this function receives a hashTable, deletes it and cleans its memory
status destroyHashTable(hashTable hashTbl)
{
    if (hashTbl == NULL)
        return argument_failure;
    status desStatus = success;
    for(int i=0; i<hashTbl->num_init_lList; i++) // freeing all the linked lists in the table
    {
        if(destroyList(hashTbl->table[i]) != success)
            desStatus = failure;
    }
    free(hashTbl->table);
    free(hashTbl);
    return desStatus;
}

static int mapKey(hashTable hashTbl, Element key)
{
    return hashTbl->transformIntoNumber(key) % hashTbl->tableSize;
}

/** a function that receives a hashTable, key and value if the key exits in the hash table it will return a failure
    if not it will create a new mapping to the key and add the key and value as pair to the hash table */
status addToHashTable(hashTable hashTbl, Element key, Element value)
{
    if(hashTbl == NULL || key == NULL || value == NULL)
        return argument_failure;
    if(lookupInHashTable(hashTbl, key) != NULL) // key already in the hash
        return failure;
    int mapIndex = mapKey(hashTbl, key);
    keyValuePair pairToInsert = createKeyValuePair(key, value, hashTbl->cpyKey, hashTbl->cpyVal, hashTbl->freeKey, hashTbl->freeVal, hashTbl->printKey, hashTbl->printVal, hashTbl->eqKey);
    if (appendNode(hashTbl->table[mapIndex], pairToInsert) != success) // adding the pair to the hash
    {
        destroyKeyValuePair(pairToInsert);
        return failure;
    }
    return success;
}

/** A function that receives a hash table, key and returns the value that is mapped to that key
   if the key doesn't exist a null well return */
Element lookupInHashTable(hashTable hashTbl, Element key)
{
    if(key == NULL || hashTbl == NULL)
        return NULL;
    int mapIndex = mapKey(hashTbl, key);
    return getValue(searchByKeyInList(hashTbl->table[mapIndex],key));
}

/** A function that receives a hashTable, key
   if the key exists in the hash it will delete the pair that holds the key from the hash, if not will return failure */
status removeFromHashTable(hashTable hashTbl, Element key)
{
    if (hashTbl == NULL || key == NULL)
        return argument_failure;
    int mapIndex = mapKey(hashTbl, key);
    Element pair = searchByKeyInList(hashTbl->table[mapIndex], key); // find the keyValuePair for deletion
    return deleteNode(hashTbl->table[mapIndex], pair);
}

/** A function that receives a HashTable and prints all the pairs in the hash table */
status displayHashElements(hashTable hashTbl)
{
    if (hashTbl == NULL)
        return argument_failure;
    for (int i = 0; i < hashTbl->tableSize; i++)
    {
        if (displayList(hashTbl->table[i]) != success)
            return failure;
    }
    return success;
}

