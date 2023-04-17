#include "LinkedList.h"

/// this struct defines a linked list node, holds up data and a pointer to the next node
typedef struct Node_s
{
    Element data;
    struct Node_s *next;
}Node;

struct LinkedList_s
{
    Node *head;
    Node *tail;
    int size;
    FreeFunction freeFunc;
    CopyFunction cpyFunc;
    PrintFunction printFunc;
    EqualFunction cmpElement;
    EqualFunction cmpKey;
};

/// this function initializes a new node
static Node * initNode (Element elem);

/// this function deletes a node and free's its memory
static status DestroyNode(LinkedList lnkList, Node *node);


static Node * initNode(Element elem)
{
    Node *node = (Node *) malloc(sizeof (Node));
    if(node == NULL)
    {
        printf("Memory Problem");
        return NULL;
    }
    node->data = elem;
    node->next = NULL;
    return node;
}


static status DestroyNode(LinkedList lnkList, Node *node)
{
    if(lnkList->freeFunc(node->data) == failure)
        return failure;
    free(node);
    return success;
}

LinkedList createLinkedList(CopyFunction cpyFunc, FreeFunction freeFunc, PrintFunction printFunc, EqualFunction cmpElement, EqualFunction cmpKey)
{
    if(cpyFunc == NULL || freeFunc == NULL || printFunc == NULL || cmpElement == NULL)
        return NULL;
    LinkedList lnkList = (LinkedList)malloc(sizeof (struct LinkedList_s));
    if(lnkList == NULL)
    {
        printf("Memory Problem");
        return NULL;
    }
    lnkList->head = NULL;
    lnkList->tail = NULL;
    lnkList->size = 0;
    lnkList->freeFunc = freeFunc;
    lnkList->cpyFunc = cpyFunc;
    lnkList->printFunc = printFunc;
    lnkList->cmpElement = cmpElement;
    lnkList->cmpKey = cmpKey;
    return lnkList;
}

status destroyList(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    LinkedList lnkList = (LinkedList)elem;
    Node *curr = lnkList->head;
    Node *tmp;
    while (curr != NULL)
    {
        tmp = curr;
        curr = curr->next;
        if (DestroyNode(lnkList, tmp) != success)
            return failure;
    }
    free(lnkList);
    return success;
}

status appendNode(LinkedList lnkList, Element elem)
{
    if(lnkList == NULL || elem == NULL)
        return argument_failure;
    Element newElem = lnkList->cpyFunc(elem);
    if (newElem == NULL)
        return memory_failure;
    Node * new_node = initNode(newElem);
    if (new_node == NULL)
        return memory_failure;
    if (lnkList->head == NULL)
        lnkList->head = new_node;
    else
    {
        Node *tmp = lnkList->tail;
        tmp->next = new_node;
    }
    lnkList->tail = new_node;
    lnkList->size ++;
    return success;
}

status deleteNode(LinkedList lnkList, Element elem)
{
    if (lnkList == NULL || elem == NULL)
        return argument_failure;
    Node * tmp;
    if(lnkList->size == 0)
        return failure;
    if(lnkList->cmpElement(lnkList->head->data, elem) == true) // element to delete is the head of the list
    {
        tmp = lnkList->head;
        lnkList->head = lnkList->head->next;
        if (DestroyNode(lnkList, tmp) != success)
            return failure;
        lnkList->size--;
        return success;
    }
    Node *curr = lnkList->head;
    while (curr->next != NULL) // search the node in the list and delete it if exists
    {
        if (lnkList->cmpElement(curr->next->data, elem) == true)
        {
            tmp = curr->next;
            if(lnkList->tail == tmp)
                lnkList->tail = curr;
            curr->next = curr->next->next;
            if (DestroyNode(lnkList, tmp) != success)
                return failure;
            lnkList->size --;
            return success;
        }
        curr = curr->next;
    }
    return failure;
}

status displayList(Element elem)
{
    if (elem == NULL)
        return argument_failure;
    LinkedList lnkList = (LinkedList)elem;
    Node *curr = lnkList->head;
    while (curr != NULL)
    {
        if (lnkList->printFunc(curr->data) != success)
            return failure;
        curr = curr->next;
    }
    return success;
}

Element getDataByIndex(LinkedList lnkList, int index)
{
    if (lnkList == NULL || index > lnkList->size || index < 1)
        return NULL;
    Node * curr = lnkList->head;
    for (int i = 1; i < index; ++i)
        curr = curr->next;
    return lnkList->cpyFunc(curr->data);
}

int getLengthList(LinkedList lnkList)
{
    if (lnkList == NULL)
        return -1;
    return lnkList->size;
}

Element searchByKeyInList(LinkedList lnkList, Element elem)
{
    if(lnkList == NULL || elem == NULL)
        return NULL;
    Node * curr = lnkList->head;
    while(curr != NULL)
    {
        if(lnkList->cmpKey(curr->data, elem) == true)
            return lnkList->cpyFunc(curr->data);
        curr = curr->next;
    }
    return NULL;
}