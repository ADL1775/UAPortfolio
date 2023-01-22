/*****************************************************
 *
 *  CSCV352 - Exercise7_2.c
 *
 *  Student:  Adam Livingston
 *
******************************************************/




/* list.c -- functions supporting list operations */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/* local function prototype */
static void CopyToNode(ItemData item, Node * pnode);

/*  interface functions     */
/*  set the list to empty   */
void InitializeList(List * plist)
{
    plist->pHead = NULL;        // Sunday office hours: changed these.
    plist->pTail = NULL;
//    *plist = NULL;            // First method.
}

// *pList in a function is an indicator that data can (and should) be managed.
// We have to keep track of the head and the tail.



/* returns true if list is empty */
bool ListIsEmpty(const List * plist)
{
    if (plist->pHead == NULL)
        return true;
    else
        return false;
}

/* returns true if list is full */
bool ListIsFull(const List * plist)
{
    Node * pt;
    bool full;

    pt = (Node *)malloc(sizeof(Node));
    if (pt == NULL)
        full = true;
    else
        full = false;
    free(pt);

    return full;
}

/* returns number of nodes */
unsigned int ListItemCount(const List * plist)
{
    unsigned int count = 0;
    
    Node * pnode = plist->pHead;    // set to start of list.
    
    //Node * pnode = *plist;    /* old way to set to start of list */


    while (pnode != NULL)
    {
        ++count;
        pnode = pnode->pNext;  /* set to next node */
    }

    return count;
}

/* creates node to hold item and adds it to the end of */
/* the list pointed to by plist (slow implementation) */
bool AddItem(ItemData item, List * plist)
{    
    Node * pnew;
    //Node * scan = *plist;
    
    Node * scan = plist->pHead;         // Scan now points to head of list.


    //  REMINDER: pList is now a pointer to the List struct containing:
    //  Node *pHead
    //  Node *pTail
    
    pnew = (Node *)malloc(sizeof(Node));            // pnew is the size of node we're looking for.
                                                    // This is similar to the last few slides in the lecture
                                                    // (where we defined it as pNewNode instead of pnew

    if (pnew == NULL)
        return false;           /* quit function on failure */

    CopyToNode(item, pnew);     // Adds the item data into Node struct, linked to Itemdata struct, linked to film struct.
                                // Remember: film struct was defined as ItemData in list.h
                                //          and then Itemdata was defined as item in the passed arguments when function was called.

    pnew->pNext = NULL;         // Adds a null pointer to the end of the node.
    
    if (scan == NULL)
    {
        plist->pHead = pnew;
        
        //scan = pnew;        /* empty list, so place... */
                            /* ...pnew at head of list */
    }           
        
    // If list is not empty...
    else
    {        
        while (scan->pNext != NULL)
        {
            scan = scan->pNext;
        }

        // pLastNode is now pointing to the lats node.
        // We now need to insert a new node.

        scan->pNext = pnew;
        
        /* OLD CODE THAT MIGHT NOT WORK
        firstNode->pNext = pnew;
        plist->pTail = pnew;
        
        plist->pTail->pNext = pnew;
        plist->pTail = pnew;
        */
        
        // pTail now keeps track of the end.

        /* OLD CODE */
        //while (scan->pNext != NULL)     // Continue scanning until the end of the list is found.
        //scan = scan->pNext;             /* find end of list */
        //scan->pNext = pnew;             /* add pnew to end */
        
    }

    return true;
}

/* visit each node and execute function pointed to by pfun */
void Traverse(const List * plist, void(*pfun)(ItemData item))
{
    
    //Node * pnode = *plist;        // old code.
    Node * pnode = plist->pHead;    /* set to start of list */

    while (pnode != NULL)
    {
        (*pfun)(pnode->itemData);   /* apply function to item */
        pnode = pnode->pNext;       /* advance to next item */
    }
}

/* free memory allocated by malloc() */
/* set list pointer to NULL */
void EmptyTheList(List * plist)
{
    Node * psave;

    while (plist->pHead != NULL)
    {
        psave = (plist->pHead)->pNext;
        free(plist->pHead);
        plist->pHead = psave;
    }
    
    /*
    Node * psave;

    while (plist != NULL)
    {
        psave = psave->pNext;
        free(plist);
        plist = psave;
    }
    */
    
    /*
    while (*plist != NULL)
    {
        psave = (*plist)->pNext;    // save address of next node
        free(*plist);               // free current node
        *plist = psave;             // advance to next node
    }
    */
}

/* local function definition */
/* copies an item into a node */
static void CopyToNode(ItemData item, Node * pnode)
{
    pnode->itemData = item;  /* structure copy */
}