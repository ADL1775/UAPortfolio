/*****************************************************
 *
 *  CSCV352 - Exercise10_3.c
 *
 *  Student:  Adam Livingston
 *
 ******************************************************/
/*
            INSTRUCTIONS
Use the following structure as the node type for implementing a sorted linked list:

#define MAX_WORD 30

typedef struct _linkedListNode
{
    char word[MAX_WORD];
    struct _linkedListNode *pNext;
} LinkedListNode;

The linked list is used to store words that are read from a text file. After reading a word from the
file, place the word in the list by adding it in SORTED order. The list should contain an
alphabetically sorted list of as many as 100 words from the file.

Use a single array to manage the data for all the words. Do not use malloc() to create nodes
that are inserted into the list. The array should be large enough to hold up to 100 words.
Write a program that accepts the name of the file as the only command line parameter. Once
the list is populated with words, use a simple menu that provides the user with the options to:
- Print the list of words
- Search for a word in the list and indicate if it’s present or not
- Add a word to the list
- Remove a word from the list

*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

///////////////////////////////////////////////////////
//                CONSTANTS
///////////////////////////////////////////////////////
#define LINE_SIZE 256
#define MAX_WORD_LEN 30
#define MAX_LIST_ENTRIES 100

///////////////////////////////////////////////////////
//               typedefs and structures
//////////////////////////////////////////////////////
typedef struct _linkedListNode
{
    struct _linkedListNode *pNext;
    char word[MAX_WORD_LEN];

} LinkedListNode;

// THIS IS STORING NODES
// This already allocates the memory.
// MAX_LIST_ENTRIES * each element in LinkedListNode.
LinkedListNode allWords[MAX_LIST_ENTRIES];

///////////////////////////////////////////////////////
//                FunctionPrototypes
///////////////////////////////////////////////////////
LinkedListNode *ReadWordList(char *fileName, int *pCount);
LinkedListNode *FindWord(LinkedListNode *word);
LinkedListNode *AddWord(LinkedListNode *word, int *pCount);
LinkedListNode *RemoveWord(LinkedListNode *word, int *pCount);
LinkedListNode *GetNode();

void FreeList();
void PrintList(LinkedListNode *pPrintList, int *pCount);
void ReleaseNode(LinkedListNode *pNode);
void CountWords(int *pCount);
void uppercase(char *str);

char menu(void);


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
int main(int argc, char *argv[])
{
    //char *fileName = "words.txt";         // (for hard-coding)
    LinkedListNode *pLinkedListNode;
    
    // COMMENT OUT BELOW FOR HARD-CODING
    if (argc < 2)
    {
        printf("\nMissing an argument.\nNext time, enter a .txt file as the second argument.\nExiting program\n");
        printf("\n*******************\n");
        printf("***   GOODBYE   ***\n");
        printf("*******************\n\n");
    }

    char* fileName;
    fileName = argv[1];
    // END COMMENT BLOCK FOR HARD-CODING

    // Initialize the list
    memset(allWords, 0, sizeof(allWords));

    int count;

    // Read the text document into the structure.
    pLinkedListNode = ReadWordList(fileName, &count);

    if (pLinkedListNode == NULL)
    {
        printf("Cannot load the word list.\n");
        return 1;
    }

    char choice;

    while ((choice = menu()) != 'q')
    {
        switch (choice)
        {
        case 'p':
            PrintList(pLinkedListNode, &count);
            break;

        case 'f':
            FindWord(pLinkedListNode);
            break;

        case 'a':
            pLinkedListNode = AddWord(pLinkedListNode, &count);
            break;

        case 'r':
            pLinkedListNode = RemoveWord(pLinkedListNode, &count);
            break;
        case 'c':
            CountWords(&count);
            break;

        default:
            puts("Switching error");
        }
    }

    FreeList();

    printf("\n===========================================\n");
    printf("                GOODBYE! \n");
    printf("===========================================\n");

    return 0;
}


//===============================================================//
//========================== FUNCTIONS ==========================//
//===============================================================//
// Reads in the .txt document and stores the words in alphabetical order.
LinkedListNode *ReadWordList(char *fileName, int *pCount)
{
    //=====================================//
    // Open the file and check if it's valid.
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Cannot open your .txt file. Closing program.\n");
        exit(0);
    }
    //=====================================//

    char word[MAX_WORD_LEN];
    int wordCount = 0;

    LinkedListNode *pNewNode = NULL;
    LinkedListNode *pCurNode = NULL;
    LinkedListNode *pPrevNode = NULL;
    LinkedListNode *pListHead = NULL;

    char aline[LINE_SIZE];

    while (!feof(fp))
    {
        // Find a memory location for the new node.
        // Instead of dynamically allocation memory using:
        //      pNewNode = (LinkedListNode *)malloc(sizeof(LinkedListNode));
        // 
        // We just use the GetNode() function since we already allocated the memory.
        pNewNode = GetNode();

        if (pNewNode == NULL)
        {
            printf("Critical memory error\n");
            exit(0);
        }

        if (fgets(aline, LINE_SIZE, fp) != NULL)
        {
            sscanf(aline, "%s", word);
            uppercase(word);
            strcpy(pNewNode->word, word);

            pNewNode->pNext = NULL;

            //============================================================//
            // This just inserts if the list is currently empty           //
            //============================================================//
            if (pListHead == NULL)
            {
                // If list was empty, add as first node
                pListHead = pNewNode;
                wordCount++;
            }
            //============================================================//

            else
            {
                pCurNode = pListHead;

                while (pCurNode != NULL)
                {

                    if ((strcmp(pCurNode->word, pNewNode->word)) > 0)
                    {
                        break;
                    }

                    else
                    {
                        // Make PrevNode = CurNode
                        pPrevNode = pCurNode;

                        // Move to the next node.
                        pCurNode = pCurNode->pNext;
                    }
                }

                pNewNode->pNext = pCurNode;

                if (pPrevNode == NULL)
                {
                    pListHead = pNewNode;
                    wordCount++;
                }

                else
                {
                    pPrevNode->pNext = pNewNode;    // pPrevNode comes before the new one
                    wordCount++;
                    pPrevNode = NULL;               // Account for the next entry and make pPrevNode NULL
                }
            }
        }
    }

    fclose(fp);

    *pCount = wordCount;

    printf("\n===============================================================\n");
    printf("%s was successfully read in and the list is populated!\n", fileName);
    printf("Loaded %d words from the word list.\n", wordCount);
    printf("===============================================================\n");

    return pListHead;
}

//===============================================================//
//===============================================================//
// Finds a word in the linked list.
LinkedListNode *FindWord(LinkedListNode *word)
{
    LinkedListNode *pListHead;
    LinkedListNode *pCurNode = NULL;
    pListHead = word;

    char searchWord[MAX_WORD_LEN];


    printf("Please enter a word you wish to find: ");
    scanf("%29[^\n]", searchWord);

    uppercase(searchWord);

    if (strlen(searchWord) == 0)
    {
        printf("You didn't enter a word...\n");
        return 0;
    }

    pCurNode = pListHead;

    while (pCurNode != NULL)
    {
        if ((strcmp(pCurNode->word, searchWord)) == 0)
        {
            printf("\n======================================\n");
            printf("%s was found in the list!\n", searchWord);
            printf("======================================\n");
            return 0;
        }
        
        else
        {
            pCurNode = pCurNode->pNext;
        }
    }
    
    printf("\n======================================\n");
    printf("Sorry. %s was not found in the list.\n", searchWord);
    printf("======================================\n");
    return 0;
}

//===============================================================//
//===============================================================//
// Allows user to add a word to the linked list
LinkedListNode *AddWord(LinkedListNode *word ,int *pCount)
{
    int wordCount = *pCount;

    // First, check to see if we can even add any new words.
    if (wordCount >= MAX_LIST_ENTRIES)
    {
        printf("\n==============================================");
        printf("\nSorry. The list is full and cannot add a word.\n");
        printf("==============================================\n");
        return 0;
    }    

    LinkedListNode *pListHead;
    pListHead = word;

    LinkedListNode *pNewNode = NULL;
    LinkedListNode *pCurNode = NULL;
    LinkedListNode *pPrevNode = NULL;


    char newWord[MAX_WORD_LEN];

    // Get the word from the user and clean it up.
    printf("\n======================================\n");
    printf("Please enter the word you wish to add: ");
    scanf("%29[^\n]", newWord);
    printf("======================================\n");

    uppercase(newWord);

    // Create the pNewNode
    // GetNode() Finds the next empty node in which to place the new word.
    pNewNode = GetNode();
    pNewNode->pNext = NULL;     // Make pNext of the new node = NULL.

    // Copy the user input into the new node.
    strcpy(pNewNode->word, newWord);

    pCurNode = pListHead;

    if (pListHead == NULL)
    {
        // If list was empty, add as first node
        pListHead = pNewNode;
    }

    else
    {
        // Start at the beginning of the list
        pCurNode = pListHead;

        // If I'm starting at the beginning, how do I know I've reached the end?
        while (pCurNode != NULL)
        {

            int compareResult;

            if ((compareResult = strcmp(pCurNode->word, pNewNode->word)) >= 0) // If we make the this < 0, it would just change the order from ascending to descending.
            {
                // Check for a duplicate.
                if (compareResult == 0)
                {
                    printf("\n======================================\n");
                    printf("%s is already in the list!\n", pNewNode->word);
                    printf("======================================\n");
                    return 0;
                }

                else
                {
                    break;
                }
            }

            else
            {
                pPrevNode = pCurNode;
                // Move to the next node.
                pCurNode = pCurNode->pNext;
            }
        }

        pNewNode->pNext = pCurNode;

        if (pPrevNode == NULL)
        {
            pListHead = pNewNode;
            printf("\n======================================\n");
            printf("%s was added to the beginning of the list!\n", pNewNode->word);
            printf("======================================\n");
            wordCount++;
            *pCount = wordCount;
            return pListHead;
        }

        else
        {
            pPrevNode->pNext = pNewNode;
            printf("\n======================================\n");
            printf("%s was added to the list!\n", pNewNode->word);
            printf("======================================\n");
            wordCount++;
            *pCount = wordCount;
            return pListHead;
        }
    }
    
}

//===============================================================//
//===============================================================//
// Allows a user to remove a word from the list.
LinkedListNode *RemoveWord(LinkedListNode *word, int *pCount)
{    
    //    memset(pNode, 0, sizeof(LinkedListNode));
    char removeWord[MAX_WORD_LEN];
    LinkedListNode *pListHead;
    LinkedListNode *pCurNode = NULL;
    LinkedListNode *pPrevNode = NULL;
    
    
    pListHead = word;
    int wordCount = *pCount;

    printf("\n===========================================\n");
    puts("Please enter the word you wish to remove: ");
    scanf("%29[^\n]", removeWord);
    printf("===========================================\n");
    
    uppercase(removeWord);

    if (strlen(removeWord) == 0)
    {
        printf("You didn't enter a word...\n");
        return 0;
    }

    pCurNode = pListHead;

    while (pCurNode != NULL)
    {

        // We want the strcmp to equal '0' because that means we found the word.
        if ((strcmp(pCurNode->word, removeWord)) == 0)
        {
            break;
        }

        else
        {
            // Make PrevNode = CurNode
            pPrevNode = pCurNode;

            // Move to the next node.
            pCurNode = pCurNode->pNext;
        }
    }

    if (pCurNode != NULL)
    {
        // We have now found the node.

        // Do this if the word is at the head of the list.
        if (pPrevNode == NULL)
        {
            // We know that the pCurNode is pointing to node we are on.
            // We know that pListHead is pointing to the head of the list.
            // All we have to do is make the list head equal to pCurNode->pNext.
            // This removes the first node.
            pListHead = pCurNode->pNext;
            printf("\n======================================\n");
            printf("Removed %s from the list!\n", removeWord);
            printf("======================================\n");

        }

        else //(If pPrevNode != NULL)
        {
            pPrevNode->pNext = pCurNode->pNext;
            printf("\n======================================\n");
            printf("Removed %s from the list!\n", removeWord);
            printf("======================================\n");
        }

        wordCount--;
        *pCount = wordCount;
        ReleaseNode(pCurNode);
        return pListHead;
    }

    else
    {
        printf("\n==================================================\n");
        printf("Sorry. %s was not found and could not be removed.\n", removeWord);
        printf("==================================================\n");
        return 0;
    }
}

//===============================================================//
//===============================================================//
// Retreives the next empty node from the array. Equivalent to memset from previous exercises.
LinkedListNode *GetNode()
{
    int i = 0;
    LinkedListNode *pEmptyNode = NULL;

    // Look for an empty entry in the list
    // As long as emptyNode is less than our max list entries...
    // And the length of the word is != 0...
    // We just go to the next entry.    
    while (i < MAX_LIST_ENTRIES && strlen(allWords[i].word) != 0)
    {
        ++i;
    }

    if (i < MAX_LIST_ENTRIES)
    {
        // So this is the position in the array because we incremented in the 'while' loop
        //                          |
        // (The array name)         |
        //              |           |
        //              v           v
        pEmptyNode = &allWords[i];
    }


    /*    
    // COULD ALSO DO THIS FOR THE FUNCTION:
    // Accomplishes the same thing.
    LinkedListNode * pEmptyNode;
    int i = 0;
    while (i < MAX_LIST_ENTRIES)
    {
        if (strlen(allWords[i].word) == 0)
        {
            pEmptyNode = &allWords[i];
            break;
        }
        i++;
    }
    return pEmptyNode;
    */

    return pEmptyNode;
}



//==============================//
//      Simpler functions       //
//==============================//
//===============================================================//
//===============================================================//
// Frees the list at the conclusion of the program.
void FreeList()
{
    int i = 0;

    while (i < MAX_LIST_ENTRIES && strlen(allWords[i].word) != 0)
    {
        memset(allWords[i].word, 0, sizeof(LinkedListNode));
        ++i;
    }
    /*
    // Old code from original implementation
    // When closing the program, said that free() was an invalid pointer
    
    while (pFreeList != NULL)
    {
        LinkedListNode *pNode;
        pNode = pFreeList;
        pFreeList = pFreeList->pNext;
        free(pNode);
    }
    */
    
}

//===============================================================//
//===============================================================//
// Prints the list of words.
void PrintList(LinkedListNode *pListToPrint, int *pCount)
{
    int wordCount = *pCount;
    int remainingEntries = MAX_LIST_ENTRIES - wordCount;


    if (pListToPrint == NULL)
    {
        printf("\n\n===============================================================\n");
        printf("Sorry. Your list is empty! Try adding some words to it first.\n");
        printf("===============================================================\n");
    }
    else
    {
        printf("\n\n=======================\n");
        printf("Begin Wordlist:\n");
        printf("=======================\n");
        
        while (pListToPrint != NULL)
        {
            printf("%s\n", pListToPrint->word);
            pListToPrint = pListToPrint->pNext;
        }
        printf("\n=================================\n");
        printf("End Wordlist\n");
        printf("Total Words: %d out of %d\n", wordCount, MAX_LIST_ENTRIES);
        printf("There is space for %d more word(s)\n", remainingEntries);
        printf("=================================\n");
    }
}

//===============================================================//
//===============================================================//
// Releases a node and NULLs the memory out, making it a free node for future use.
void ReleaseNode(LinkedListNode *pNode)
{
    // For removing nodes
    // This just makes the removed node is nulled-out so it goes back into the pool of nodes.
    memset(pNode, 0, sizeof(LinkedListNode));
}

//===============================================================//
//===============================================================//
// A function just for fun.
void CountWords(int *pCount)
{
    int wordCount = *pCount;
    int remainingEntries = MAX_LIST_ENTRIES - wordCount;
    
    printf("\n========================================\n");
    printf("Total words in the list: %d out of %d\n", wordCount, MAX_LIST_ENTRIES);
    printf("There is space for %d more word(s)\n", remainingEntries);
    printf("========================================\n");
}

//===============================================================//
//===============================================================//
// Converts a string to uppercase.
void uppercase(char *str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}

//===============================================================//
//===============================================================//
// Menu options.
char menu(void)
{
    int ch;

    printf("\n\n======================== MENU ========================");
    puts("\nWhat do you want to do with your list of words?\n");
    puts("Enter the letter corresponding to your choice:");
    puts("p) Print a list of the words       f) Search for a word\n");
    puts("a) Add a word to the list          r) Remove a word    \n");
    puts("c) Total words in the list         q) quit             \n");

    while ((ch = getchar()) != EOF)
    {
        while (getchar() != '\n') // discard rest of line
            continue;

        ch = tolower(ch);

        if (strchr("pfarcq", ch) == NULL)
            puts("Please enter: p, f, a, r, c, or q:");
        else
            break;
    }

    if (ch == EOF) /// make EOF cause program to quit
        ch = 'q';

    return ch;
}