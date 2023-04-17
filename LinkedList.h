#ifndef RICKANDMORTYADT_LINKEDLIST_H
#define RICKANDMORTYADT_LINKEDLIST_H

#include "Defs.h"

/// This is a generic linked list
typedef struct LinkedList_s *LinkedList;

/// This function creates a new linked list, because it's a generic one it receives needed functions from the user
LinkedList createLinkedList(CopyFunction cpyFunc, FreeFunction freeFunc, PrintFunction printFunc, EqualFunction equalElem, EqualFunction eqKey);

/// this function receives a linked list, deletes it and cleans its memory using the free function received in the create
status destroyList(Element elem);

/// this function receives an element and a linked list and adds the element to the end of the list
status appendNode(LinkedList lnkList, Element elem);

/** A function that receives an element and deletes it from the linked list using the free function received
 if the element is not in the list a failure status is returned */
status deleteNode(LinkedList lnkList, Element elem);

/// the function prints all the elements in the list by their order
status displayList(Element elem);

/** this function returns an element in the linked list according to the index received
    indexes start at 1 */
Element getDataByIndex(LinkedList lnkList, int index);

/// A function that returns the number of elements in the list
int getLengthList(LinkedList lnkList);

/** this function searches for a node in the linked list that contains the element received
   by using the second compare function, if exists such node it returns its data, else returns null */
Element searchByKeyInList(LinkedList lnkList, Element elem);


#endif //RICKANDMORTYADT_LINKEDLIST_H
