#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ItemList.h"

// Use 5_3 as an example. Reference Wk7 Monday office hours as well.
/**** PURPOSE ****/
/*
    Keeps track of what items might be on the player or in the location.
    Creates a set of functions for adding/removing items to both player or location

*/

void ItemListInit(ItemList *pItemList)
{
    pItemList->itemCount = 0;
}

int ItemListAddItem(ItemList *pItemList, int itemNumber)
{
    int count = 0;

    // Set error code if list is already full
    if (pItemList->itemCount == MAX_ITEM_LIST)
    {
        count = -1;
    }
    
    /*********************************************************
    if (pItemList->itemCount < MAX_ITEM_LIST)
    {
        pItemList->items[pItemList->itemCount++] = itemNumber;
    }
    *********************************************************/
    
    // Example from Wk7 monday office hours)
    else
    {
        int index = pItemList->itemCount;
        
        //Place into the itemNumber array at index 0 (or other)
        //the itemNumber passed into it.
        pItemList->items[index] = itemNumber;  
        pItemList->itemCount++;
        count = pItemList->itemCount;
    }

    //return pItemList->itemCount;
    return count;
}

int ItemListRemoveItem(ItemList *pItemList, int itemNumber)
{
    // We still passed itemNumber and need to remove it from the list.
    // What do we need to do to remove itemNumber x?
    int count = 0;          // Keep some consistency between add/remove functions.
    
    int indexOfItem = -1;   // Find where itemNumber is in the array.
                            // Set to -1 initially since 0 is a valid index position.
                            // We can use the -1 value if the item is not found.


    // Loop through the array to find itemNumber and its index position.
    for (int i = 0; i < MAX_ITEM_LIST; ++i)
    {
        if (pItemList->items[i] == itemNumber)
        {
            indexOfItem = i;
            break;
        }
    }
    
    // If the item is in the list, we need to remove it and...
    // ...shift all the entries down from that index forward.
    // REMEMBER: This is ignored if the item was not found in the 'for' loop above.
    if (indexOfItem != -1)
    {
        for (int i = indexOfItem; i < (MAX_ITEM_LIST - 1); ++i)
        {
            // Take position i starting at the index where the entry was found
            // then overwrite it with whatever the next position in the array is [i + 1].
            pItemList->items[i] = pItemList->items[i + 1];
        }
        // Decrement the itemCount.
        --pItemList->itemCount;
    }
    
    // Make the count variable equal to the itemCount.
    // This isn't really necessary since we could just 'return pItemList->itemCount;'
    // But this maintains consistency between add/remove item functions.
    count = pItemList->itemCount;
    return count;
}

// REMINDER: boolean data is anything that is either true or false.
bool ItemListHasItem(ItemList *pItemList, int itemNumber)
{
    // Make a bool variable type that defaults to false.
    bool bFound = false;
    
    // Let's search for the itemNumber!
    // This loop is essentially identical to the ItemRemoveItem for loop when...
    // ...we searched for the item. Why?
    // BECAUSE THAT'S WHAT THE FUNCTION IS SUPPOSED TO DO! We already did the work.
    for (int i = 0; i < pItemList->itemCount; ++i)
    {
        if (pItemList->items[i] == itemNumber)
        {
            bFound = true;      // Instead of returning the index of the item, we need...
                                // ...to set the variable to 'true'.
            break;
        }
    }
    
    // Return either true or false. True = the item was found! False = item was NOT found.
    return bFound;

}