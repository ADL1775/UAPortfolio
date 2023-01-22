#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <stdbool.h>

#define MAX_ITEM_LIST   16


typedef struct
{
    int items[MAX_ITEM_LIST];   // Array of item numbers (the numbers that correspond with the items) in the list
    int itemCount;              // Count of the number of entries in the list
                                // If itemCount = 2, then there are 2 items.

} ItemList;

/*Initializes the ItemList structure pointed to by pItemList as an empty list.*/
void ItemListInit(ItemList *pItemList);

/*Adds item identified by itemNumber to the end of the list. Have the function
return the current count of items in the list.*/
int ItemListAddItem(ItemList *pItemList, int itemNumber);

/*Removes item identified by itemNumber from the list. Have the function return
the current number of items in the list.*/
int ItemListRemoveItem(ItemList *pItemList, int itemNumber);

/*Determines if the list pointed to by pItemList contains the item identified by
itemNumber.*/
bool ItemListHasItem(ItemList *pItemList, int itemNumber);


#endif