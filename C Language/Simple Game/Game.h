#ifndef GAME_H
#define GAME_H

#define MAX_MAP_LOCATIONS   25
#define MAX_ITEM_COUNT      25

#include "Item.h"
#include "ItemList.h"
#include "Player.h"
#include "Location.h"

typedef struct
{
    int numLocations;
    Location map[MAX_MAP_LOCATIONS];
    int itemCount;
    Item items[MAX_ITEM_COUNT];
    Player player;  // Player object

} Game;

/*Initializes the game structure and calls the initialize routines for the game’s map
and the game’s items. The player object is also initialized.*/

int GameInitialize(Game *pGame);

#endif