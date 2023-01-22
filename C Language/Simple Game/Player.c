#include <stdio.h>
#include "Player.h"

void PlayerInit(Player *pPlayer, int locationNumber)
{
    ItemListInit(&pPlayer->items);
    pPlayer->location = locationNumber;
}



void PlayerAddItem(Player *pPlayer, int itemNumber)
{
    ItemListAddItem(&pPlayer->items, itemNumber);
}



void PlayerRemoveItem(Player *pPlayer, int itemNumber)
{
    ItemListRemoveItem(&pPlayer->items, itemNumber);
}



bool PlayerHasItem(Player *pPlayer, int itemNumber)
{
    return ItemListHasItem(&pPlayer->items, itemNumber);
}