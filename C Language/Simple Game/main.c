/*****************************************************
 *
 *  CSCV352 - Exercise6_5.c
 *
 *  Student:  Adam Livingston
 *
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"

Game game;
///////////////////////////////////////////////////////
//                CONSTANTS
///////////////////////////////////////////////////////
#define LINE_MAX    15  

///////////////////////////////////////////////////////
//               typedefs and structures
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//               globalVariables
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//                FunctionPrototypes
///////////////////////////////////////////////////////
void Describe(int location)
{
    Location *pLocation;

    pLocation = &game.map[location];

    printf("Location: %s.\n", pLocation->name);
    printf("%s", pLocation->description);

    int i = 0;
    for (i = 0; i < pLocation->items.itemCount; i++)
    {
        int itemNumber;
        itemNumber = pLocation->items.items[i];
        printf("There is a %s here.\n", game.items[itemNumber].name);
    }

}


int GetUserInput(char *aline, int maxLine)     
{
    // Use fgets to get user input one line at-a-time
    fgets(aline, maxLine, stdin);
    if (strlen(aline) > 0)
    {
        aline[strlen(aline) - 1] = '\0';
    }

    return strlen(aline);
}


void ParseUserInput(char *aline)
{
    // This is where we take that user input and analyze it.
    char* verb;
    char* noun;
    int itemNumber;
    
    // Make pointers to necessary structs.
    Player* pPlayer;                // pointer to Player struct                          
    Location* pCurrentLocation;     // pointer to Location struct

    /* Get player location */
    int currLocIndex = game.player.location;    // Set the player's current location equal to game.player.location.
    pCurrentLocation = &game.map[currLocIndex]; // Make the current location equal to game.map's index.
    // Now pCurrentlocation will store the player's location.

    
    //int currItemCount = game.player.items.itemCount;
    // pPlayer = &game.player.items.items[currItemCount];
    pPlayer = &game.player;
    
    verb = strtok(aline, " ");
    noun = strtok(NULL, "");

    if (verb != NULL)
    {
        switch(verb[0])
        {
            // Go North, South, East, West
            // BE SURE TO HAVE A CHECK THAT MAKES SURE IT'S A REAL LOCATION.
            case 'n':
            case 'N':
                if( game.map[game.player.location].north == -1)
                {
                    printf("\nYou can't go that way\n\n");
                }
                
                else
                {
                    game.player.location = game.map[game.player.location].north;
                }  
                
                break;
            
            case 's':
            case 'S':
                if( game.map[game.player.location].south == -1)
                {
                    printf("\nYou can't go that way\n\n");
                }
                
                else
                {
                    game.player.location = game.map[game.player.location].south;
                }  
                  
                break;

            case 'e':
            case 'E':
                if( game.map[game.player.location].east == -1)
                {
                    printf("\nYou can't go that way\n\n");
                }
                
                else
                {
                    game.player.location = game.map[game.player.location].east;
                }  
                break;

            case 'w':
            case 'W':
                if( game.map[game.player.location].west == -1)
                {
                    printf("\nYou can't go that way\n\n");
                }
                
                else
                {
                    game.player.location = game.map[game.player.location].west;
                }  
                  
                break;
            
            // Get <item>: Get the item (if it exists) and put in user item list
            case 'g':
            case 'G':
                if (noun == NULL)
                {
                    printf("\nI can't do that without specifying an item...\n\n");
                    break;
                }
                
                /*
                // But we want to determine if 'noun' is a real item.
                
                // We have a function for that! ItemListHasItem from 6_2:
                //          bool ItemListHasItem(ItemList *pItemList, int itemNumber);
                
                // We have to transpose that string into an item number.
                // And we have a function for THAT as well! From 6_1:
                //          int ItemGetItemNumber(Item *pItems, int maxItems, char* itemName);
                
                // This finds the itemNumber by using the noun string to find the item number.
                */
                itemNumber = ItemGetItemNumber(game.items, game.itemCount, noun);
                
                if (itemNumber == -1)
                {
                    printf("\n%s is not a valid item.\n\n", noun);
                }
                
                else
                {
                    // Make sure the item is present.
                    // add the item to the player object
                    // remove the itemm from the location
                    
                    
                    if(!LocationHasItem(pCurrentLocation, itemNumber))
                    {
                        printf("****************************************************");
                        printf("\n%s is not here. Maybe try looking somewhere else?\n", noun);
                        printf("****************************************************\n\n");
                    }
          
                    if (LocationHasItem(pCurrentLocation, itemNumber))       // Returns a True/false value
                    {

                        PlayerAddItem(pPlayer, itemNumber);
                        
                        printf("\n\n******************************");
                        printf("\n%s added to your inventory\n", noun);
                        printf("******************************\n\n");

                        LocationRemoveItem(pCurrentLocation, itemNumber);
                            
                    }
                    
                    
                }
                break;
            
            // Drop <item>: Get the item (if carrying it) and put in current location
            case 'd':
            case 'D':
                
                // But we want to determine if 'noun' is a real item.
                //      we can use the same function as we did with picking up the item.
                itemNumber = ItemGetItemNumber(game.items, game.itemCount, noun);
                
                if (itemNumber == -1)
                {
                    printf("\n%s is not a valid item.\n\n", verb);
                }
                
                // Error message if the player does not have the item in their inventory.
                if (!PlayerHasItem(pPlayer, itemNumber))
                {
                        printf("\n\n******************************");
                        printf("\n%s is not in your inventory.\n", noun);
                        printf("******************************\n\n");
                }


                if (PlayerHasItem(pPlayer, itemNumber))
                {
                    LocationAddItem(pCurrentLocation, itemNumber);
                    PlayerRemoveItem(pPlayer, itemNumber);
                    printf("\n\n******************************");
                    printf("\nYou dropped the %s on the floor\n", noun);
                    printf("******************************\n\n");
                }
               
                break;
            
            // Inventory: list the items the user is carrying
            case 'i':
            case 'I':

                if (pPlayer->items.itemCount > 0)
                {
                    
                    int i;
                    int numItems = pPlayer->items.itemCount;
                    printf("\n# of items in inventory: %d\n", numItems);
                    printf("YOUR INVENTORY:\n");
                    for (i = 0; i < numItems; i++)
                    {
                        
                        //printf("%d\n",pPlayer->items.items[i]);
                        // pPlayer->items.items[i] is the number associated with the item.

                        itemNumber = pPlayer->items.items[i];
                        // Student note: I know there is an easier way to do this.
                        // There is probably a way to reference the struct in Item.h and print
                        //          game.items->name and game.items->description.
                        
                        
                        if (itemNumber == 0)
                        {
                            printf("\ncomb - a small hair comb\n");
                        }
                        if (itemNumber == 1)
                        {
                            printf("brush - a large hair brush\n");
                        }
                        if (itemNumber == 2)
                        {
                            printf("bowl - a pretty bowl suitable for eating breakfast cereal\n");
                        }
                        if (itemNumber == 3)
                        {
                            printf("soap - a fragrantly smelling bar of soap\n");
                        }
                        if (itemNumber == 4)
                        {
                            printf("vase - a beautifully decorated vase\n");
                        }
                        if (itemNumber == 5)
                        {
                            printf("candlestick - a brass candlestick covered with melted wax\n");
                        }
                        
                        // printf("%ls\n", game.player.items.items);
                        // No good
                        //printf("%s\n", pPlayer->items);
                        //printf("%s\n",pPlayer->items.items[i]);

                    }

                }
                else
                {
                    printf("\n\nYour inventory is empty...\n\n");
                }
                printf("\n");
                break;
            


            // Look: Describe the current location (and its associated items).
            case 'l':
            case 'L':
                
                //printf("\n%s\n", pCurrentLocation);
                /* Not Working Code */
                //Describe(game.player.location); 
                //Describe(pCurrentLocation);
                //printf("%s\n", game.player);  // THIS WORKS. But produces errors when making the file.

                // After a command is entered, Describe() is called.
                // So all we have to do is break.                   
                break;
                

                // If the user doesn't input a valid character
                default:
                    printf("\nI don't know how to do %s\n", verb);
                    break;
        }
    }   
    
    else
    {
        
        
    }
    
    
}


/*
Implement the following looping process in your main.c:
    Describe the location (including items located at the user's current location).
    Read user input
    Parse and perform the appropriate operation or report an error message

Implement each function below:
    void Describe(int location);
    int GetUserInput(char *aline, int maxLine);     // USE FGETS
    void ParseUserInput(char *aline);
*/



/***********************************/
/****           MAIN            ****/
/***********************************/
int main()
{
    char input[LINE_MAX];

    GameInitialize(&game);

    while (1)
    {
        Describe(game.player.location);
        GetUserInput(input, LINE_MAX);
        ParseUserInput(input);
    }
}