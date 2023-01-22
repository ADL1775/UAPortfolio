#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Game.h"

// REFERENCE WK7 THURSDAY OFFICE HOURS

/*
    // INITIALIZE THE MAP
    DONE            int numLocations;

    done?           Location map[MAX_MAP_LOCATIONS];

    // INITIALIZE THE ITEMS
    DONE            int itemCount; 
    
                    Item items[MAX_ITEM_COUNT];
    
    DONE            Player player;
*/

int GameInitialize(Game *pGame)
{
    
    //int place;
    int playerStartPos;
    
    /*****************************************************************************************/
    /**** INITIALIZE THE ITEMS ****/
    /******************************/
    // Read in all the items. Initializes all of the game's items.
    // The return value is the total number of items.
    // So we want to store that number inside of the Game structure.
    // This is accomplished with this notation:
    //      |
    //      v
    pGame->itemCount = ItemReadItems(pGame->items, MAX_ITEMS, "items.txt");
    /*****************************************************************************************/
    
    
    /*****************************************************************************************/
    /**** INITIALIZE THE MAP ****/
    /****************************/
    // Read in all of the locations.
    pGame->numLocations = LocationReadMap(pGame->map, MAX_MAP_LOCATIONS, "map.txt");
    /*****************************************************************************************/

    /*****************************************************************************************/
    /**** INITIALIZE THE PLAYER ****/
    /*******************************/
    // We need to pass the location where the player is going to start 
    //          (a value between 0 and (numLocations-1))
    srand(clock()); // Seeds the random
    playerStartPos = rand() % pGame->numLocations;  // Use random start location
    PlayerInit(&pGame->player, playerStartPos);  
    /*****************************************************************************************/


    /*****************************************************************************************/
    /**** DISTRIBUTE ITEMS IN GAME ****/
    /**********************************/
    // Distrubte items throughout the game across the map
    int i = 0;
    for (i = 0; i < pGame->itemCount; i++)
    {
        
        /* You can use rand again to randomly distribute them */
        int randomItemLoc;
        randomItemLoc = rand() % pGame->numLocations;
        
        /* Now where shall we put them?*/
        Location* pLocationForItem;
        // Get a pointer to the random location
        pLocationForItem = &pGame->map[randomItemLoc];  
        
        /* Now, place the item in that location */
        // Takes a location pointer and an item number.
        // The item number will be i as determined by the for loop this code rests in.
        LocationAddItem(pLocationForItem, i);
        
    }
    /*****************************************************************************************/

    
    return 0;
}