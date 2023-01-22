
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Item.h"


/*
Loads the list of items from a text file with comma delimited fields. Returns the
number of objects loaded from the filename (or -1 if unable to open the file and
load the data). Fills the Item array items with the contents of a file entitled
"items.txt".
This reads in all the data from items.txt and puts them in Item array.
*/

int ItemReadItems(Item items[], int maxItems, char* filename)
{
    
    // Create file pointer and open the file.
    FILE* fp;
    fp = fopen(filename, "r");

    // Error message if file cannot be opened.
    if (fp == NULL)
    {
        printf("\nCannot open items.txt.\n");
        return -1;
    }

    // Create variables
    char* line;     // Holds the read items
    int i = 0;      // Counts the items in the loop.
    
    /*Failed memory allocation method:*/
    // char* line = calloc(100, sizeof(MAX_ITEM_NAME_STRING + MAX_ITEM_DESCRIPTION_STRING + 1));

    for (i = 0; i < maxItems; i++)
    {
        // Allocate memory for variable line.
        line = malloc(sizeof(MAX_ITEM_NAME_STRING + MAX_ITEM_DESCRIPTION_STRING + 1) * 100);

        char ch;
        int k = 0;      // Index of the line (the character position in the string of each line);

        while ((ch = fgetc(fp)) != '\n' && ch != EOF)
        {
            // Remove the quotations
            if (ch == '"')
            {

            }
            
            // Read characters into line.
            else
            {
                line[k] = ch;
                //printf("%c", line[k]);        // For testing with printf statement bleow
                k++;
            }
        }
        // use strtok to get names and descriptions into variables.
        // Note: you can use strtok as the 2nd argument in strcpy
        //       but this makes it easier for me to understand.
        char* itemName = strtok(line, ",");
        char* itemDesc = strtok(NULL, "");

        //printf("%s %s\n", itemName, itemDesc);    // For testing with printf statement in else condition above.

        /**** PARSE THE INFORMATION ****/
        // char *strcpy(char *dest, const char *src)    source character array to destination character array
        // char *strtok(char *str, const char *delim)
        // This copies the string that strtok gets into the struct position.
        strcpy(items[i].name, itemName);
        strcpy(items[i].description, itemDesc);

        /**** REALLOCATE MEMORY ****/
        //void *realloc(void *ptr, size_t size)
        // NOTE: Program was tested without realloc and worked fine. 
        line = realloc(line, sizeof(char) * 100);  

    }

    // Free memory
    free(line);
    // Close the file
    fclose(fp);
    
    // Return the number of read items
    return i;
}
/*
Finds an item by itemName and returns the item number of the item
itemName. The function returns -1 if an item with the specified itemName is not
present. The item number is the index of the item in the items array.

*/
int ItemGetItemNumber(Item items[], int maxItems, char* itemName)
{

    for (int i = 0; i < maxItems; i++)
    {
        // Condition if strings are equal
        // strcmp reuturns value of 0 if strings are equal 
        //(https://www.programiz.com/c-programming/library-function/string.h/strcmp)
        if (strcmp(itemName, items[i].name) == 0)
        {
            return i;
        }
        /* THESE DOESNT WORK
        else
        {
            return -1;
        }
        
        else if (strcmp(itemName, items[i].name) != 0)
        {
            return -1;
        }
        */
    }

    return -1;
}