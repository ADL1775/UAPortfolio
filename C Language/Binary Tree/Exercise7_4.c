/*****************************************************
 *
 *  CSCV352 - Exercise7_4.c
 *
 *  Student:  Adam Livingston
 *
******************************************************/

/******************************************************
            INSTRUCTIONS

Write a program that accepts the name of a file as the one and only command line parameter.

The program must open and read the text file and record how many times each word occurs in
the file. 

Start with the binary search tree implementation from Chapter 17 in the book and
modify it to store both a word and the number of times it occurs. 

After the program has read the file and populated the search tree, it should offer a menu with three choices. 

    1) The first is to list all the words along with the number of occurrences. 

    2) The second is to let you enter a word, with the program reporting how many times the word occurred in the file. 

    3) The third choice is to quit.

******************************************************/

/* petclub.c -- use a binary search tree */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAXITEMS 1000

/**************************************************************************************/
/************** BEGIN TREE.H **************/
/******************************************/
/* redefine Item as appropriate */
typedef struct item
{
//  char petname[20];   // Old
//  char petkind[20];   // Old
//  int occurances;     // Old
    
    char word[128];     // Buffer to keep the word
    int count;          // Number of occurrances of the word.

} Item;

/****************************************************************************************************
            FIRST NOTE:
We replaced all occurrances of 'petname' to 'word'.

Then we removed any references to 'petkind'.

****************************************************************************************************/


typedef struct node
{
    Item item;
    struct node * left;    /* pointer to right branch  */
    struct node * right;   /* pointer to left branch   */

} Node;

typedef struct tree
{
    Node * root;           /* pointer to root of tree  */
    int size;              /* number of items in tree  */

} Tree;

/* function prototypes */

/* operation:      initialize a tree to empty          */
/* preconditions:  ptree points to a tree              */
/* postconditions: the tree is initialized to empty    */
void InitializeTree(Tree * ptree);

/* operation:      determine if tree is empty          */
/* preconditions:  ptree points to a tree              */
/* postconditions: function returns true if tree is    */
/*                 empty and returns false otherwise   */
bool TreeIsEmpty(const Tree * ptree);

/* operation:      determine if tree is full           */
/* preconditions:  ptree points to a tree              */
/* postconditions: function returns true if tree is    */
/*                 full and returns false otherwise    */
bool TreeIsFull(const Tree * ptree);

/* operation:      determine number of items in tree   */
/* preconditions:  ptree points to a tree              */
/* postconditions: function returns number of items in */
/*                 tree                                */
int TreeItemCount(const Tree * ptree);

/* operation:      add an item to a tree               */
/* preconditions:  pi is address of item to be added   */
/*                 ptree points to an initialized tree */
/* postconditions: if possible, function adds item to  */
/*                 tree and returns true; otherwise,    */
/*                 the function returns false          */
bool AddItem(const Item * pi, Tree * ptree);

/* operation:  find an item in a tree              */
/* preconditions:  pi points to an item                */
/*                 ptree points to an initialized tree */
/* postconditions: function returns true if item is in */
/*                 tree and returns false otherwise    */
bool InTree(const Item * pi, const Tree * ptree);

/* operation:      delete an item from a tree          */
/* preconditions:  pi is address of item to be deleted */
/*                 ptree points to an initialized tree */
/* postconditions: if possible, function deletes item  */
/*                 from tree and returns true;         */
/*                 otherwise, the function returns false*/
bool DeleteItem(const Item * pi, Tree * ptree);

/* operation:      apply a function to each item in    */
/*                 the tree                            */
/* preconditions:  ptree points to a tree              */
/*                 pfun points to a function that takes*/
/*                 an Item argument and has no return  */
/*                 value                               */
/* postcondition:  the function pointed to by pfun is  */
/*                 executed once for each item in tree */
void Traverse (const Tree * ptree, void (* pfun)(Item item));

/* operation:      delete everything from a tree       */
/* preconditions:  ptree points to an initialized tree */
/* postconditions: tree is empty                       */
void DeleteAll(Tree * ptree);

/******************************************/
/*************** END TREE.H ***************/
/**************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************************/
/*************** BEGIN TREE.C ***************/
/********************************************/
/* local data type */
typedef struct pair {
    Node * parent;
    Node * child;
} Pair;

/* protototypes for local functions */
static Node * MakeNode(const Item * pi);
static bool ToLeft(const Item * i1, const Item * i2);
static bool ToRight(const Item * i1, const Item * i2);
static void AddNode (Node * new_node, Node * root);
static void InOrder(const Node * root, void (* pfun)(Item item));
static Pair SeekItem(const Item * pi, const Tree * ptree);
static void DeleteNode(Node **ptr);
static void DeleteAllNodes(Node * ptr);

/* function definitions */
void InitializeTree(Tree * ptree)
{
    ptree->root = NULL;
    ptree->size = 0;
}

bool TreeIsEmpty(const Tree * ptree)
{
    if (ptree->root == NULL)
        return true;
    else
        return false;
}

bool TreeIsFull(const Tree * ptree)
{
    if (ptree->size == MAXITEMS)
        return true;
    else
        return false;
}

int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}

bool AddItem(const Item * pi, Tree * ptree)
{
    Node * new_node;
    
    // Pair seekResults;

    if  (TreeIsFull(ptree))
    {
        fprintf(stderr,"Tree is full\n");
        return false;             /* early return           */
    }
    if (SeekItem(pi, ptree).child != NULL)
    {
        fprintf(stderr, "Attempted to add duplicate item\n");
        return false;             /* early return           */
    }
    new_node = MakeNode(pi);      /* points to new node     */
    if (new_node == NULL)
    {
        fprintf(stderr, "Couldn't create node\n");
        return false;             /* early return           */
    }
    /* succeeded in creating a new node */
    ptree->size++;

    if (ptree->root == NULL)      /* case 1: tree is empty  */
        ptree->root = new_node;   /* new node is tree root  */
    else                          /* case 2: not empty      */
        AddNode(new_node,ptree->root); /* add node to tree  */
   
    return true;                  /* successful return      */
}

bool InTree(const Item * pi, const Tree * ptree)
{
    return (SeekItem(pi, ptree).child == NULL) ? false : true;
}

bool DeleteItem(const Item * pi, Tree * ptree)
{
    Pair look;
    
    look = SeekItem(pi, ptree);
    if (look.child == NULL)
        return false;
 
    if (look.parent == NULL)      /* delete root item       */
        DeleteNode(&ptree->root);
    else if (look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;

    return true;
}

void Traverse (const Tree * ptree, void (* pfun)(Item item))
{

    if (ptree != NULL)
        InOrder(ptree->root, pfun);
}

void DeleteAll(Tree * ptree)
{
    if (ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}


/* local functions */
static void InOrder(const Node * root, void (* pfun)(Item item))
{
    if (root != NULL)
    {
        InOrder(root->left, pfun);
        (*pfun)(root->item);
        InOrder(root->right, pfun);
    }
}

static void DeleteAllNodes(Node * root)
{
    Node * pright;

    if (root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}

static void AddNode (Node * new_node, Node * root)
{
    if (ToLeft(&new_node->item, &root->item))
    {
        if (root->left == NULL)      /* empty subtree       */
            root->left = new_node;   /* so add node here    */
        else
            AddNode(new_node, root->left);/* else process subtree*/
    }
    else if (ToRight(&new_node->item, &root->item))
    {
        if (root->right == NULL)
            root->right = new_node;
        else
            AddNode(new_node, root->right);
    }
    else                         /* should be no duplicates */
    {
        fprintf(stderr,"location error in AddNode()\n");
        exit(1);
    }
}

static bool ToLeft(const Item * i1, const Item * i2)
{
    int comp1;

    if ((comp1 = strcmp(i1->word, i2->word)) < 0)
        return true;
    else
        return false;
}

static bool ToRight(const Item * i1, const Item * i2)
{
    int comp1;

    if ((comp1 = strcmp(i1->word, i2->word)) > 0)
        return true;
    else
        return false;
}

static Node * MakeNode(const Item * pi)
{
    Node * new_node;

    new_node = (Node *) malloc(sizeof(Node));
    if (new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    
    return new_node;
}

static Pair SeekItem(const Item * pi, const Tree * ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;

    if (look.child == NULL)
        return look;            /* early return   */
 
    while (look.child != NULL)
    {
        if (ToLeft(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else        /* must be same if not to left or right    */
            break;  /* look.child is address of node with item */
    }
 
    return look;    /* successful return   */
}

static void DeleteNode(Node **ptr)
/* ptr is address of parent member pointing to target node  */
{
    Node * temp;

    puts((*ptr)->item.word);
    if ( (*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if ( (*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else    /* deleted node has two children */
    {
        /* find where to reattach right subtree */
        for (temp = (*ptr)->left; temp->right != NULL;
                temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr =(*ptr)->left;
        free(temp);
    }
}

/*******************************************/
/*************** END TREE.C ***************/
/**************************************************************************************/

char menu(void);
void printitem(Item item);

// void AddWord(Tree * pt, Item *pItem);    // This is what Prof. Duren added as an example
void AddWord(Tree * pt, char *word);        // This works    

void showWords(const Tree * pt);            // Derived from void showpets(const Tree * pt);
void findWord(const Tree * pt);             // Derived from void findpet(const Tree * pt);
void uppercase(char * str);                 // Was in original code.


/* 
Old function declarations
void addpet(Tree * pt);
void droppet(Tree * pt);
void showpets(const Tree * pt);
void findpet(const Tree * pt);
*/

    /**********************************************************************
     * Potential Steps:
     * --------------------------------
     *  1) Set command line argument conditions 
     *          (Write a program that accepts the name of a file as the one and only command line parameter.)
     * 
     *  2) Open file and read with character stream
     * 
     *  3) Make sure the stream knows when it encounters a word

     *  4) When character stream encounters a word, store it in the tree with AddWord()
     * 
     *          Now the binary tree is created...
     * 
     *  5) Display the menu options
     *      Three Options:
     *          1) List all the words along with the number of occurrences.
     *              Traverse(&words, printitem); ?
     * 
     *          2) Let user enter a word, with the program reporting how many times the word occurred in the file.
     *              void findpet(const Tree * pt);?
     * 
     *          3) Quit (easy)
     * 
     *  6) Have the menu execute the user's command.
     * 
     * ********************************************************************/


int main(int argc, char* argv[])        // Changed from int main(void)
{
    // Tree pets;
    Tree words;         // Changed to this from above. (New local structure)
    // Create file pointer.
    FILE* fp;
	char* fileName;	
        
    //InitializeTree(&pets);     
    InitializeTree(&words);         // Initialize the tree. Changed to this from above.


    // Are there enough parameters? (check argc)
	if (argc < 2)
	{
		printf("\nMissing an argument.\nNext time, enter a .txt file as the second argument.\nExiting program\n");
        printf("\n*******************\n");
        printf("***   GOODBYE   ***\n");
        printf("*******************\n\n");
	}

    fileName = argv[1];
    
    // Open the file and check for errors.
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("ERROR: Cannot Open file %s. Exiting program \n\n", fileName);
        exit(-1);
    }
    
    /*******************************************************************************/
    /*https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word*/
    // 
    
    char fileWord [46];
    
    // Got his one from stackoverflow, which assumes no word exceeds length of 1023 characters. 
    // while (fscanf(fp, " %1023s", fileWord) == 1)
    
    // Instead, we can just check for letters of the alphabet and ignore special characters.
    // The %45 limites word size to 45 characters.
    //          Why only 45?
    // Longest word in english: Pneumonoultramicroscopicsilicovolcanoconiosis
    while (fscanf(fp, "%45[a-zA-Z]%*[^a-zA-Z]", fileWord) == 1)  
    {        
        uppercase(fileWord);
        AddWord(&words, fileWord);
    }
    
    /*
    int fscanf(FILE *stream, const char *format, ...)
    Return Value: 
    This function returns the number of input items successfully matched and assigned,
    which can be fewer than provided for, or even zero in the event of an early matching
    failure.
    */
    /*******************************************************************************/
    
    /* This didn't work.
    char fileWord [1024];
    //int state = 0;
    char ch;
    int index = 0;
    
    while ((ch = fgetc(fp)) != EOF)
    {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
            fileWord[index] = ch;
            index++;
        }
        
        else if ((ch < 'a' && ch > 'z') || (ch <'A' && ch > 'Z'))
        {
            fileWord[0] = 0;
            index = 0;
        }
        uppercase(fileWord);
        AddWord(&words, fileWord);
    }
    */
    
    // Close the file.
    fclose(fp);
    
    char choice;
    while ((choice = menu()) != 'q')
    {
        switch (choice)
        {
            case 'l' :  showWords(&words);
                        break;
            case 'f' :  findWord(&words);
                        break;
            default  :  puts("Switching error");
        }
    }
    
    /*  
    Prof Duren had this in his 7_4 review lecture
    This adds the words into the tree one-at-a-time
    You want to pass a pointer to the tree (&words)

    Note: AddWord is a function that he created.
    AddWord(&words, "the");
    AddWord(&words, "crucible");
    AddWord(&words, "enable");
    AddWord(&words, "token");
    AddWord(&words, "the");

    **********************************************************************************
    // THIS WAS ADDED AT END OF LECTURE (12:10)
    Traverse(&words, printitem);

    // printitem is a function provided in the petclub.c file.
    // Traverse just shows all the things in the tree.
    
    Note the output of program when Traverse is called:
    
    PET: VELVET              Kind: CAT
    PET: VELVET              Kind: DOG
    
    Look at the function:
    void Traverse (const Tree * ptree, void (* pfun)(Item item))
    {
        if (ptree != NULL)
            InOrder(ptree->root, pfun);
    }

    It takes as a parameter a pointer to the tree and a pointer to a function.
    printitem is the function that actually prints the data. The function is defined below 'void AddWord()'.
    ***********************************************************************************/

    // DeleteAll(&pets);
    DeleteAll(&words);      // Changed to this from above.
    
    puts("\nGoodbye!");
   
    return 0;
}

/*
How to find the item:
    We can use something similar to seekResult = SeekItem(&newItem, pt);
    or a combination of InTree and SeekItem to actually get the item and display it.
*/
void findWord(const Tree * pt)
{
    Item temp;          // Continuation from findPet();
    Pair seekResult;    // Added this from Prof Duren's AddWord() function;

    if (TreeIsEmpty(pt))
    {
        puts("No entries! Your file must have been empty...");
        return;                 //quit function if tree is empty
    }
    
    else
    {      
        puts("Please enter name of the word you wish to find:");
              
        // Get this warning when using gets():
        // "warning: implicit declaration of function ‘gets’; did you mean ‘fgets’?"
        // But it works.        
        gets(temp.word);  

        //Tried this instead:
        //scanf("%c", temp.word);
        //returns gibberish when trying to print temp.word.  

        uppercase(temp.word);           // Convert to uppercase (because that's what our words are stored as).
        printf("\n%s ", temp.word);     // Print the word the user is searching for. 
                                        // Conditions statements below determine the outcome. 
        
        if (InTree(&temp, pt))
        {
            seekResult = SeekItem(&temp, pt);  
            
            //printf("occurs %d times.\n", temp.count);    // No. Always returned '0' in %d of printf().
            
            // CRUCIAL: ===========================================
            //                            |                       |
            //                            |                       |
            printf("occurs %d times.\n", ((Item *)seekResult.child)->count);
            
            /* My old attempts trying to get this to work:
            seekResult = SeekItem(&temp, pt);                                                    

            // This if() statement stops: "warning: unused variable ‘seekResult’".
            // 
            // Tried these: 
            // if (seekResult.child != NULL)
            // if (seekResult.parent != NULL)
            //
            // Returned gibberish. Fixed by replacing scanf() with gets() despite the warning.
            // Is the child the count?

            if (seekResult.child != NULL)
            {
                printf("occurs %d times.\n", temp.count);   // No. Always returned '0' in %d of printf().
            }
            */
        }
        
        else
        {
            printf("is not in the file.\n");
        }

    }


}

/***********************
*** INSTRUCTOR ALERT ***
***********************/
// PROF DUREN EXPLAINED THIS IN 7_4 LECTURE at 6:15 

// You could pass a new pointer to an item here (aka using the Item structure):
//                           _______________|
//                           |
//                           v
void AddWord(Tree * pt, char *word)     // ... but Prof Duren just passed the word.
{
    Item newItem;
    Pair seekResult;
    // So now we have a new local structure called 'newItem;'
    
    if (TreeIsFull(pt))
        puts("Not enough space for all the words.");

    else
    {
        // uppercase(newItem.word);     // like in addpet. Doesn't make a difference because
        // we did this when we read in the words in the file.
        
        // We are copying the actual word into the newItem 'word' field.
        strcpy(newItem.word, word);
        
        // Also, we are initializing the newItem.count to 1...
        // ...because that's the first time that we found this item.
        newItem.count = 1;


        // Check to see if the item is in the tree.
        // If the item is NOT in the tree, then we add the item.
        if (!InTree(&newItem, pt))      
        {
            AddItem(&newItem, pt);      // AddItem is only called when the item is NOT in the tree.
                                        // Because in this case, we don't want to add duplicates. 
        }
        
        // But in the case of duplicates...we still need to do some work.
        // In the petclub program, an error is called and says that it's already there.
        // In THIS case, we need to get that structure and add something to the count.


        // If the item IS in the tree, then we do this.
        else
        {
            // Get the node for this word from the tree
            // SeekItem is a provided function.
            // The value we get back from SeekItem will be stored in seekResult.
            seekResult = SeekItem(&newItem, pt);

            
            /******  How does SeekItem Work? *********************************************
            
            Recall the SeekItem code: static Pair SeekItem(const Item * pi, const Tree * ptree)

            It returns something called a pair.
                    
                    /////////////////////
                    // What is a pair? //
                    /////////////////////
            'Pair' is a structure part of the tree implementation.
            It contains a parent and a child pointer.
            
            The way that Pair works and the way that SeekItem works:
                If the item is already there, then the child member of pair is going to be non-NULL. 
                It's going to be pointing to the item that we want.
                
                We can see this in the InTree() code:

                bool InTree(const Item * pi, const Tree * ptree)
                {
                    return (SeekItem(pi, ptree).child == NULL) ? false : true;
                }
 
                We can see that it's going to return a true or false depending on whether or not...
                ... the SeekItem (which is the pair of pi and tree)-dot-child is NULL.
                If it is NULL, we say 'false' because it wasn't there.
                If it's not NULL, it's going to return 'true'.
            ******************************************************************************/ 

            // So we're going to take advantage of SeekItem and get the result back.
            // We should have a child pointer that is non-NULL.
            // THIS CHILD POINTER POINTS TO THE ITEM STRUCTURE.
            // It's going to point to the node for THIS particular word.
            if (seekResult.child != NULL)
            {
                // At this point, all we need to do is increment the count.
                ((Item *)seekResult.child)->count++;  
            }
        }
             
    }

}

char menu(void)
{
    int ch;

    puts("\n\nWhat do you want to do with your .txt file?\n");
    puts("Enter the letter corresponding to your choice:");
    puts("l) List all words w/ occurances       f) Search for a word");
    puts("q) quit                                                   \n");
    while ((ch = getchar()) != EOF)
    {
        while (getchar() != '\n')  // discard rest of line 
            continue;
        
        ch = tolower(ch);
        
        if (strchr("lfq", ch) == NULL)
            puts("Please enter an l, f, or q:");
        else
            break;
    }
    if (ch == EOF)       /// make EOF cause program to quit
        ch = 'q';

    return ch;
}


void printitem(Item item)
{
    printf("Word: %-19s  Occurrances: %-8d\n", item.word, item.count);}


void showWords(const Tree * pt)
{
    if (TreeIsEmpty(pt))
        puts("No entries! Your file must have been empty...");
    else
        Traverse(pt, printitem);
}


void uppercase(char * str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}
/********************//********************//********************/
/****** ALERT! ******//****** ALERT! ******//****** ALERT! ******/
/********************//********************//********************/
/*                  IMPORTANT LECTURE NOTE!                     */
/*
    We can use Prof Duren's AddWord function to get a good ways through the program.
                    
                        What he DOESNT show:
    1) How to find the item
        We can use something similar to seekResult = SeekItem(&newItem, pt);

        or a combination of InTree and SeekItem to actually get the item and display it.

*****************************************************************
****************************************************************/

/*************************************/
/*********** OLD FUNCTIONS ***********/
/************VVVVVVVVVVVVV************/

#if 0        
/********************//*****end alert******//********************/   

char menu(void)
{
    int ch;

    puts("Nerfville Pet Club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a) add a pet          l) show list of pets");
    puts("n) number of pets     f) find pets");
    puts("d) delete a pet       q) quit");
    while ((ch = getchar()) != EOF)
    {
        while (getchar() != '\n')  // discard rest of line 
            continue;
        ch = tolower(ch);
        if (strchr("alrfndq",ch) == NULL)
            puts("Please enter an a, l, f, n, d, or q:");
        else
            break;
    }
    if (ch == EOF)       /// make EOF cause program to quit
        ch = 'q';

    return ch;
}

/********************//***** Lecure Note ******//********************/

In the provided code, there is an addpet function.
In this function, the data is collected. See notes below.

void addpet(Tree * pt)
{
    Item temp;

    if (TreeIsFull(pt))
        puts("No room in the club!");
    else
    {
        puts("Please enter name of pet:");
        gets(temp.word);                    // Using gets to get the petkind and petname.
        puts("Please enter pet kind:");
        gets(temp.petkind);
        uppercase(temp.word);               // It converts the data to uppercase here
        uppercase(temp.petkind);            //              and here.
        
        
        // Then it calls the AddItem() function.
        // That function puts data into the tree.
        // Instead of this, we have AddWord().
        AddItem(&temp, pt);                 
    }
}

void showpets(const Tree * pt)
{
    if (TreeIsEmpty(pt))
        puts("No entries!");
    else
        Traverse(pt, printitem);
}


void findpet(const Tree * pt)
{
    Item temp;

    if (TreeIsEmpty(pt))
    {
        puts("No entries!");
        return;     /* quit function if tree is empty */
    }

    puts("Please enter name of pet you wish to find:");
    gets(temp.word);
    puts("Please enter pet kind:");
    gets(temp.petkind);
    uppercase(temp.word);
    uppercase(temp.petkind);
    
    printf("%s the %s ", temp.word, temp.petkind);
    if (InTree(&temp, pt))
        printf("is a member.\n");
    else
        printf("is not a member.\n");
}

void droppet(Tree * pt)
{
    Item temp;

    if (TreeIsEmpty(pt))
    {
        puts("No entries!");
        return;     /* quit function if tree is empty */
    }

    puts("Please enter name of pet you wish to delete:");
    gets(temp.word);
    puts("Please enter pet kind:");
    gets(temp.petkind);
    uppercase(temp.word);
    uppercase(temp.petkind);
    printf("%s the %s ", temp.word, temp.petkind);
    if (DeleteItem(&temp, pt))
        printf("is dropped from the club.\n");
    else
        printf("is not a member.\n");
}

void uppercase(char * str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}
#endif