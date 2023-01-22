#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Location.h"

/**********************************************************************************/
int LocationReadMap(Location map[], int maxLocations, char *filename)
{
    // Open the file and make a pointer.
    FILE* fMap;
    fMap = fopen(filename, "r");

    // Error message if file is not found
    if (fMap == NULL)
    {
        printf("\nCannot open map.txt.\n");
        return -1;
    }

    /*********************************************/
    /* COUNT THE NUMBER OF LOCATIONS IN THE FILE */
    /*********************************************/
    /*********************************************
    NOTE: This code portion is no longer necessary after help from TA.
          It was an attempt to navigate through the memory corruption issue
                caused by strcpy the location description.

    // In passing an argument of maxLocations when parsing the file data
    //      we got a segmentation fault. This was traced back to main.c #define MAX_LOCATIONS
    **********************************************/
    /*

    int locCount = 0;   // Make variable to count the number of lines (aka total locations)
    int j = 0;          // Use variable for the 'for' loop.
    for (j = 0; j < maxLocations; j++)
    {
        char ch;
        while ((ch = fgetc(fMap)) != EOF)
        {        
            if (ch =='\n')
            {
                locCount++;
            }
            
        }
    }
    printf("Location Count: %d\n", locCount);
    
    // Cursor is at end of file. Set it back to the beginning.
    fseek(fMap, 0, SEEK_SET);		
    */
    /***** location count compelte ****/
    

    int i = 0;
    
    while (i < maxLocations)
    {
        char* buffer = calloc(500, 1);      // Allocate memory.

        if (fgets(buffer, 500, fMap) == NULL)
        {
            break;
        }
        // Now we are reading line-by-line with fgets.
        // We can use strtok to parse the data into variables.
        // Use atoi to convert the string into an integer.
        // Use strcpy to store the strings (name and description)
        // SYNTAX: char *strtok(char *str, const char *delim)
        
        // Simplified version of the code below.
        map[i].north = atoi(strtok(buffer, ","));
        map[i].east = atoi(strtok(NULL, ","));   
        map[i].south = atoi(strtok(NULL, ","));   
        map[i].west = atoi(strtok(NULL, ","));         
        strcpy(map[i].name, strtok(NULL, ",")); 
        strcpy(map[i].description, strtok(NULL, ""));    // HAD BIG ISSUE WITH THIS ONE. See below.
        
        // Store the items in the struct at position i.
        ItemListInit(&map[i].items);

        // Free the buffer.
        free(buffer);
        
        // Increment the counter.
        i++;

        /*********************************************/
        /************* DEBUGGING PROCESS *************/
        /*********************************************/
        /*
        char* north = strtok(buffer, ",");    
        char* south = strtok(NULL, ",");
        char* east = strtok(NULL, ",");
        char* west = strtok(NULL, ",");
        char *locName = strtok(NULL, ",");
        char* locDesc = strtok(NULL, ""); 

        // Since the coordinates are integers and strtok returns a char*,
        //  we need to convert the values to integers.
        int iNorth = atoi(north);   
        int iSouth = atoi(south);   
        int iEast = atoi(east);   
        int iWest = atoi(west); 

        // Now place the values gained 
        // These work
        map[i].north = iNorth;
        map[i].south = iSouth;    
        map[i].east = iEast;    
        map[i].west = iWest;       
        strcpy(map[i].name, locName); 
        strcpy(map[i].description, locDesc);    // HAD BIG ISSUE WITH THIS ONE. See below.
        */
        /**** locDesc PROBLEM SOLVING ****/
        /*
            Prior to Sunday 9 Oct. office hours, I used a similar function to Item.c.
            However, locDesc messed up. Carl stated it was most likely a memory corruption.
            strcpy(map[i].description, locDesc);     // This does not work
            strcpy(map[i].description, "locDesc");   // This works, though, regardless of string length.
            When I use strcpy with char* locDesc during strtok, the data gets really messy.
            Using just locName is fine.
            Using locDesc is when it gets bad. It seems to be a problem with the string length.
        */

        // TESTING WITH PRINTF.
        // printf("Location: %d\nNorth: %s\nSouth: %s\nEast: %s\nWest: %s\nName: %s\nDesc: %s\n\n", i, north, south, east, west, locName, locDesc);     // TEST LINE
        
        
    }  
        
    //  Used this to differentiate my printf statement test above versus what main.c printed.
    //  printf("***BEGIN MAIN FUNCTION CALL***\n");   
    
    //  Close the file, return the value.
    fclose(fMap);

    return i;
    
}
/**********************************************************************************/

/**********************************************************************************/
/*Adds the item specified by itemNumber to the item list for the location specified
by pLocation.*/
void LocationAddItem(Location *pLocation, int itemNumber)
{
    ItemListAddItem(&pLocation->items, itemNumber);
}
/**********************************************************************************/

/**********************************************************************************/
/*Removes the item specified by itemNumber from the item list for the location
specified by pLocation.*/
void LocationRemoveItem(Location *pLocation, int itemNumber)
{
    ItemListRemoveItem(&pLocation->items, itemNumber);
}
/**********************************************************************************/

/**********************************************************************************/
/*Returns true if the itemNumber is contained within the item list for the location
specified by pLocation and returns false if it is not.*/
bool LocationHasItem(Location *pLocation, int itemNumber)
{
    return ItemListHasItem(&pLocation->items, itemNumber);
}
/**********************************************************************************/