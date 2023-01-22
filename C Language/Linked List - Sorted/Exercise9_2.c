/*****************************************************
 *
 *  CSCV352 - Exercise9_2.c - Read words from a file and put them
 *                 into a sorted linked list.
 *
 *  Student:  Adam Livingston
 *
 ******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

///////////////////////////////////////////////////////
//                CONSTANTS
///////////////////////////////////////////////////////
#define LINE_SIZE 256
#define MAX_WORD 30
#define MAX_POPULATE_WORDS 200

///////////////////////////////////////////////////////
//               typedefs and structures
//////////////////////////////////////////////////////
typedef struct _linkedListNode
{
    char word[MAX_WORD];
    struct _linkedListNode *pNext;

} LinkedListNode;

///////////////////////////////////////////////////////
//               globalVariables
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//                FunctionPrototypes
///////////////////////////////////////////////////////
LinkedListNode *PopulateSortedWordList(char *fileName);

void PrintList(LinkedListNode *pPrintList);

void FreeList(LinkedListNode *pWordToPrint);

/*****************************************************
 *
 *  Function:  main()
 *
 *  Parameters:
 *
 *      argc - main parameter for argument count
 *      argv - main parameter for argument values
 *
 ******************************************************/
/*
INSTRUCTIONS

Use the following structure as the node type for implementing a linked list:
#define MAX_WORD 30

typedef struct _linkedListNode
{
    char word[MAX_WORD];
    struct _ linkedListNode *pNext;
} LinkedListNode;

The linked list is used to store words that are read from a text file. Place each word in the list by
adding them to the end of the list. Add the words to the list in the order they were read from the
file.

You may use a tail pointer to make this process easier.

Each node in the list must be created using malloc.

Write a program that accepts the name of the file as the only command line parameter. Once
the list is populated with all the words from the file, print the word list in the order the words
appear in the list. This requires you to traverse the linked list and print the data in each node.

*/

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {
        printf("\nMissing an argument.\nNext time, enter a .txt file as the second argument.\nExiting program\n");
        printf("\n*******************\n");
        printf("***   GOODBYE   ***\n");
        printf("*******************\n\n");
    }
    
    char *fileName;
    fileName = argv[1];

    LinkedListNode *pSortedWordList; // When I put this in here...
                               // ...then PrintList(pPrintWord); doesn't print anything
    pSortedWordList = PopulateSortedWordList(fileName);
    //pSortedWordList = PopulateSortedWordList(fileName);

    PrintList(pSortedWordList); // This tells that pAddWord is pointing to the last word printed.
    FreeList(pSortedWordList);

    return 0;
}

//==================================================//
// Just a hybrid of "AddWord" and "AddWord"    //
//  from the 5_4 updated.                           //
//==================================================//

LinkedListNode *PopulateSortedWordList(char *fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Cannot open your .txt file. Closing program.\n");
        exit(0);
    }

    LinkedListNode *pNewNode = NULL;
    LinkedListNode *pCurNode = NULL;
    LinkedListNode *pPrevNode = NULL;
    LinkedListNode *pListHead = NULL;
    LinkedListNode *pListTail = NULL;
    char aline[LINE_SIZE];
    char word[MAX_WORD];


    while (!feof(fp))
    {
        // Allocate memory for the new node
        pNewNode = (LinkedListNode *)malloc(sizeof(LinkedListNode));

        if (pNewNode == NULL)
        {
            printf("Critical memory error\n");
            exit(0);
        }

        if (fgets(aline, LINE_SIZE, fp) != NULL)
        {
            sscanf(aline, "%s", word);
            strcpy(pNewNode->word, word);

            pNewNode->pNext = NULL;

            //============================================================//
            // This just inserts if the list is currently empty           //
            //============================================================//
            if (pListHead == NULL)
            {
                // If list was empty, add as first node
                pListTail = pListHead = pNewNode;
            }
            //============================================================//
            

            else
            {
                // Start at the beginning of the list
                pCurNode = pListHead;

                // If I'm starting at the beginning, how do I know I've reached the end?
                while (pCurNode != NULL)
                {
                    // We compare the value of the node we're trying to insert with the value of the current node.
                    
                    // int strcmp (const char* str1, const char* str2);
                    //  Returns >0 if str1 is greater (in ASCII) than str2
                    //  Returns < 0 if str1 is less than (in ASCII) than str2
                    //  H < K
                    if ((strcmp(pCurNode->word, pNewNode->word)) > 0)
                    {
                        //  So if we have:
                        //  NewNode = "Junk" 
                        //  CurNode = "Navy"
                        //  then we break;
                        
                        //  But if we have:
                        //  NewNode = "Junk"
                        //  CurNode = "Air"
                        //  Then we just move to the "else" statement.
                        
                        // We break when the word in the current node is greater than the
                        // word in the new node.
                        break;  
                    }

                    else
                    {
                        // Make PrevNode = CurNode
                        
                        pPrevNode = pCurNode;
                        // NewNode = Junk
                        // CurNode = Air
                        // PrevNode = Air

                        // Move to the next node.
                        pCurNode = pCurNode->pNext;
                        // NewNode = Junk
                        // PrevNode = Air
                        // CurNode = Coffee-Shop
                        
                    }
                }
                pNewNode->pNext = pCurNode;
                // So then pNext will have "Navy" in it.
                
                if (pPrevNode == NULL)
                {  
                    // What do we need to point to the new node if we're inserting at the beginning of the list? 
                    // ====== The head of the list! ======

                    // What's the head of the list?
                    // ===== pListHead! =====

                    // insert at the beginning of the list
                    pListHead = pNewNode;
                }

                else
                {
                    pPrevNode->pNext = pNewNode; // pPrevNode comes before the new one
                    pPrevNode = NULL;
                }
            }
        }
        
    }

    fclose(fp);
    return pListHead;
}
void PrintList(LinkedListNode *pListToPrint)
{
    while (pListToPrint != NULL)
    {
        printf("%s\n", pListToPrint->word);
        pListToPrint = pListToPrint->pNext;
    }
}

void FreeList(LinkedListNode *pListToPrint)
{
    // pListToPrint is a pointer to the beginning of the list.
    while (pListToPrint != NULL)
    {
        // Make a pointer to the current node.
        LinkedListNode *pNode;

        // Save the current node.
        pNode = pListToPrint;

        // Move to the next node.
        pListToPrint = pListToPrint->pNext;

        // Free the current node.
        free(pNode);
    }
}