/*****************************************************
 *
 *  CSCV352 - FinalExercise_3.c
 *
 *  Student:  Adam Livingston
 *
 * Write a program that reads employee records from a binary file and places each record into a
    sorted linked list. The list must be sorted by the employee ID field, which is an integer value in
    the data.
 ******************************************************/
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

///////////////////////////////////////////////////////
//                CONSTANTS
///////////////////////////////////////////////////////
#define MAX_NAME 128
#define MAX_EMAIL_ADDRESS 128
#define MAX_DATE 64

///////////////////////////////////////////////////////
//               typedefs and structures
///////////////////////////////////////////////////////
typedef struct __attribute__((packed, aligned(1))) employee_data 
{
    int employeeId;                         //   4 bytes
    char firstName[MAX_NAME];               // 128 bytes
    char lastName[MAX_NAME];                // 128 bytes
    char middleName[MAX_NAME];              // 128 bytes
    char emailAddress[MAX_EMAIL_ADDRESS];   // 128 bytes
    char title[MAX_EMAIL_ADDRESS];          // 128 bytes
    float annualSalary;                     //   4 bytes
    char dataOfHire[MAX_DATE];              //  64 bytes
    struct employee_data *pNext;

} EmployeeInformation;

EmployeeInformation allEmps[20];

///////////////////////////////////////////////////////
//               globalVariables
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//               FunctionPrototypes
///////////////////////////////////////////////////////
EmployeeInformation *RemoveEmployee(EmployeeInformation *word, int *pCount);

EmployeeInformation *PopulateEmployeeList(int *pCount);
EmployeeInformation *GetNode();
EmployeeInformation *FindEmp(EmployeeInformation *empID);
void PrintList(EmployeeInformation *pPrintList, int *pCount);
void ReleaseNode(EmployeeInformation *pNode);
void *EditEmployee(EmployeeInformation *empID);
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
int main(int argc, char** argv)
{
    EmployeeInformation *pListHead;     // Head pointer variable.
    char choice;                        // For user input
    int count;                          // Global employee counter variable.

    pListHead = PopulateEmployeeList(&count);   // Populate the list.

    while ((choice = menu()) != 'q')
    {
        switch (choice)
        {

        case 'f':
            FindEmp(pListHead);
            break;

        case 'e':
            EditEmployee(pListHead);
            break;

        case 'r':
            pListHead = RemoveEmployee(pListHead, &count);
            break;

        case 'l':
            PrintList(pListHead, &count);
            break;

        default:
            puts("Switching error");
        }
    }
    
    return 0;
}

//===============================================================//
//========================== FUNCTIONS ==========================//
//===============================================================//
// Function to edit employee salary
void *EditEmployee(EmployeeInformation *empID)
{
    // Declare variables
    EmployeeInformation *pListHead;
    EmployeeInformation *pCurNode = NULL;
    pListHead = empID;

    int searchID;

    // Prompt user for employee ID
    printf("\n\n======================================\n");  
    printf("Please enter an employee ID to display employee information: ");
    scanf("%d", &searchID);

    if (searchID == 0)
    {
        printf("You didn't enter an ID. Returning to menu.\n");
        printf("======================================\n");
        return 0;
    }

    // Make the current node equal to the head of list
    pCurNode = pListHead;

    // Cycle through the nodes
    while (pCurNode != NULL)
    {
        // If the current node equals the search ID, then the employee is found. Print the info.
        if (pCurNode->employeeId == searchID)
        {
            float newSalary;      // Variable to enter the new salaray

            // Print the employee information
            printf("\n======================================\n");
            printf("Employee ID# %d was found in the list! Printing information...\n", searchID);
            printf("======================================\n");
            usleep(1500000); // Sleep to give user experience
            
            printf("Employee ID:    %d\n", pCurNode->employeeId);
            printf("FName:          %s\n", pCurNode->firstName);
            printf("MName:          %s\n", pCurNode->middleName);
            printf("LName:          %s\n", pCurNode->lastName);
            printf("Email:          %s\n", pCurNode->emailAddress);
            printf("Title:          %s\n", pCurNode->title);
            printf("Annual Salary:  %.02f\n", pCurNode->annualSalary);
            printf("Date of hire:   %s\n", pCurNode->dataOfHire);
            printf("===================================================\n");

            printf("What would you like %s %s's new annual salary to be? ", pCurNode->firstName, pCurNode->lastName);
            scanf("%f", &newSalary);

            printf("\n%s %s's annual salary changed from %.02f to %.02f\n", pCurNode->firstName, pCurNode->lastName, pCurNode->annualSalary, newSalary);
            printf("===================================================\n");
            pCurNode->annualSalary = newSalary;

            return 0;
        }
        
        // If if the current node's employee ID# is not equal to the user input, move to the next node.
        else
        {
            pCurNode = pCurNode->pNext;
        }
    }
    
    // If the employee is not found, let the user know.
    printf("\n======================================\n");
    printf("Sorry. Employee ID# %d was not found in the list.\n", searchID);
    printf("======================================\n");
    return 0;
}
//===============================================================//
//===============================================================//
// Reads data from the file to the linked list sorted by ascending employee ID #s.
EmployeeInformation *PopulateEmployeeList(int *pCount)
{
    EmployeeInformation employees;              // Make a local variable of the structure
    FILE* fp;                                   // Declare a file pointer
    fp = fopen("EmployeeRecords.bin", "rb");    // Open the file in "read binary" mode
    int empCount;                               // Counter for the total # of employees. Will be passed to pCount.

    // Exit program if the file is not found.
    if (fp == NULL)
    {
        printf("Cannot open your .bin file. Closing program.\n");
        exit(0);
    }
    
    // Make the node pointers
    EmployeeInformation *pNewNode = NULL;
    EmployeeInformation *pCurNode = NULL;
    EmployeeInformation *pPrevNode = NULL;
    EmployeeInformation *pListHead = NULL;

    // Read from the file and each entry will be the size of the structure without the next pointer.
    while ((fread(&employees, (sizeof(EmployeeInformation) - sizeof(struct employee_data *)), 1, fp)) == 1)
    {
        // Allocate memory for the new node using GetNode()
        // We're using GetNode() becuase the linked list has memory allocated via an array.
        pNewNode = GetNode();

        // Display an error message if memory allocation fails.
        if (pNewNode == NULL)
        {
            printf("Critical memory error\n");
            exit(0);
        }
        
        // Copy the data into the new node.
        pNewNode->employeeId = employees.employeeId;
        strcpy(pNewNode->firstName, employees.firstName);
        strcpy(pNewNode->lastName, employees.lastName);
        strcpy(pNewNode->middleName, employees.middleName);
        strcpy(pNewNode->emailAddress, employees.emailAddress);
        strcpy(pNewNode->title, employees.title);
        pNewNode->annualSalary = employees.annualSalary;
        strcpy(pNewNode->dataOfHire, employees.dataOfHire);
        pNewNode->pNext = NULL;
        
        // This just inserts if the list is currently empty
        if (pListHead == NULL)
        {
            // If list was empty, add as first node and increment the count
            pListHead = pNewNode;
            empCount++;
        }
        
        else
        {
            pCurNode = pListHead;   // Make the current node equal to the list head

            while (pCurNode != NULL)
            {
                
                // Break from the loop to insert the new employee at before the current node
                if (pCurNode->employeeId > pNewNode->employeeId)
                {
                    break;
                }

                else
                {
                    // Make previous node = current node
                    pPrevNode = pCurNode;

                    // Then move to the next node
                    pCurNode = pCurNode->pNext;
                }
            }
            // Now we know where to insert the employee from the binary file.
            pNewNode->pNext = pCurNode;     // Make the next pointer equal to pCurNode

            // If the previous node is empty, insert at the beginning of the list
            if(pPrevNode == NULL)
            {
                pListHead = pNewNode;
                empCount++;
            }

            else
            {
                pPrevNode->pNext = pNewNode;    // pPrevNode comes before the new node. Link them together
                empCount++;
                pPrevNode = NULL;               // Account for the next entry and make pPrevNode NULL
            }
            

        }

    }
    fclose(fp);             // Close the file
    
    *pCount = empCount;     // Make the employee count equal empIDo pCount by reference

    // Inform user of successful loading
    printf("\n===============================================================\n");
    printf("Successfully loaded %d employees from the file.\n", empCount);
    printf("===============================================================\n");

    return pListHead;       // Return the list head
}

//===============================================================//
//===============================================================//
// Finds an employee via employee ID# and prints information.
EmployeeInformation *FindEmp(EmployeeInformation *empID)
{
    // Declare variables
    EmployeeInformation *pListHead;
    EmployeeInformation *pCurNode = NULL;
    pListHead = empID;

    int searchID;

    // Prompt user for employee ID
    printf("\n\n======================================\n"); 
    printf("Please enter an employee ID to display employee information: ");
    scanf("%d", &searchID);

    if (searchID == 0)
    {
        printf("You didn't enter an ID. Returning to menu.\n");
        printf("======================================\n");
        return 0;
    }

    // Make the current node equal to the head of list
    pCurNode = pListHead;

    // Cycle through the nodes
    while (pCurNode != NULL)
    {
        // If the current node equals the search ID, then the employee is found. Print the info.
        if (pCurNode->employeeId == searchID)
        {
            printf("\n======================================\n");
            printf("Employee ID# %d was found in the list! Printing information...\n", searchID);
            printf("======================================\n");
            usleep(1500000); // Sleep to give user experience
            
            printf("Employee ID:    %d\n", pCurNode->employeeId);
            printf("FName:          %s\n", pCurNode->firstName);
            printf("MName:          %s\n", pCurNode->middleName);
            printf("LName:          %s\n", pCurNode->lastName);
            printf("Email:          %s\n", pCurNode->emailAddress);
            printf("Title:          %s\n", pCurNode->title);
            printf("Annual Salary:  %.02f\n", pCurNode->annualSalary);
            printf("Date of hire:   %s\n", pCurNode->dataOfHire);
            printf("===================================================");


            return 0;
        }
        
        // If if the current node's employee ID# is not equal to the user input, move to the next node.
        else
        {
            pCurNode = pCurNode->pNext;
        }
    }
    
    // If the employee is not found, let the user know.
    printf("\n======================================\n");
    printf("Sorry. Employee ID# %d was not found in the list.\n", searchID);
    printf("======================================\n");
    return 0;
}

//===============================================================//
//===============================================================//
// Removes an employee record after getting user-input
EmployeeInformation *RemoveEmployee(EmployeeInformation *empID, int *pCount)
{
// Declare variables
    EmployeeInformation *pListHead;
    EmployeeInformation *pCurNode = NULL;
    EmployeeInformation *pPrevNode = NULL;
    pListHead = empID;

    int empCount = *pCount;
    int searchID;

    // Prompt user for employee ID 
    printf("\n\n======================================\n"); 
    printf("Please enter an employee ID to remove: ");
    scanf("%d", &searchID);

    if (searchID == 0)
    {
        printf("You didn't enter an ID. Returning to menu.\n");
        printf("======================================\n");
        return pListHead;
    }

    // Make the current node equal to the head of list
    pCurNode = pListHead;

    // Cycle through the nodes
    while (pCurNode != NULL)
    {
        // If the current node equals the search ID, then the employee is found. Break from loop
        if (pCurNode->employeeId == searchID)
        {
            char choice;

            printf("\n======================================\n");
            printf("Employee ID# %d was found in the list! Printing information...\n", searchID);
            printf("======================================\n");
            usleep(1500000); // Sleep to give user experience
            
            printf("Employee ID:    %d\n", pCurNode->employeeId);
            printf("FName:          %s\n", pCurNode->firstName);
            printf("MName:          %s\n", pCurNode->middleName);
            printf("LName:          %s\n", pCurNode->lastName);
            printf("Email:          %s\n", pCurNode->emailAddress);
            printf("Title:          %s\n", pCurNode->title);
            printf("Annual Salary:  %.02f\n", pCurNode->annualSalary);
            printf("Date of hire:   %s\n", pCurNode->dataOfHire);
            printf("===================================================\n");

            // Get confirmation from user
            printf("Are you sure you want to remove this employee? Input 'y' and press 'enter/return' to confirm. ");
            scanf("%s", &choice);

            // Break from loop to remove the node if user input is correct.
            if(choice == 'y')
            {
                break;
            }
            
            else
            {
                printf("Employee not removed. Returning to menu.\n\n\n");
                return pListHead;
            }
                      
        }
        
        // If if the current node's employee ID# is not equal to the user input, move to the next node.
        // Ensure we keep track of the previous node.
        else
        {
            pPrevNode = pCurNode;
            pCurNode = pCurNode->pNext;
        }
    }

    if(pCurNode != NULL)
    {
        // We have found the node to remove
        if (pPrevNode == NULL)
        {
            // We know that the pCurNode is pointing to node we are on.
            // We know that pListHead is pointing to the head of the list.
            // All we have to do is make the list head equal to pCurNode->pNext.
            // This removes the first node.
            printf("\n======================================\n");
            printf("Removed Employee ID# %d from the list!\n", pCurNode->employeeId);
            printf("======================================\n");
            pListHead = pCurNode->pNext;

        }

        else //(If pPrevNode != NULL)
        {
            // Make the previous node's next pointer equal to the current node's next pointer to ensure linked list continuity
            printf("\n======================================\n");
            printf("Removed Employee ID# %d from the list!\n", pCurNode->employeeId);
            printf("======================================\n");
            pPrevNode->pNext = pCurNode->pNext;

        }

        // Decrement the employee count and pass to global pCount
        empCount--;
        *pCount = empCount;

        // Release the current node and return the list head.
        ReleaseNode(pCurNode);
        return pListHead;
    }
    
    else
    {
        // If the employee is not found, let the user know.
        printf("\n======================================\n");
        printf("Sorry. Employee ID# %d was not found in the list.\n", searchID);
        printf("======================================\n");
        return pListHead;
    }
    
}

//===============================================================//
//===============================================================//
// Prints all the employee records in the list
void PrintList(EmployeeInformation *pPrintList, int *pCount)
{
    int employeeCount = *pCount;
    int i = 1;

    if (pPrintList == NULL)
    {
        printf("\n\n===============================================================\n");
        printf("Sorry. You don't have any employee records to display.\n");
        printf("===============================================================\n");
    }
    else
    {
        // Display some information for the user
        printf("\n\n=======================\n");
        printf("Total Employees: %d\n", employeeCount);
        printf("Begin Employee List:\n");
        printf("===================================================\n");
        usleep(1500000);        // User experience to have them believe the system is "thinking"
        
        // Print all fields of the employee records
        while (pPrintList != NULL)
        {
            
            printf("Employee #%d:\n\n", i);     // Display an employee counter
            printf("Employee ID:    %d\n", pPrintList->employeeId);
            printf("FName:          %s\n", pPrintList->firstName);
            printf("MName:          %s\n", pPrintList->middleName);
            printf("LName:          %s\n", pPrintList->lastName);
            printf("Email:          %s\n", pPrintList->emailAddress);
            printf("Title:          %s\n", pPrintList->title);
            printf("Annual Salary:  %.02f\n", pPrintList->annualSalary);
            printf("Date of hire:   %s\n", pPrintList->dataOfHire);
            printf("===================================================\n");
            i++;        // Increment the employee counter.
            pPrintList = pPrintList->pNext;
        }
        
        // Print a message for the user indicating the end of the employee list.
        printf("End Employee List\n");
        printf("=====================\n\n");
    }
}

//===============================================================//
//===============================================================//
// Releases a node and NULLs the memory out, making it a free node for future use.
void ReleaseNode(EmployeeInformation *pNode)
{
    // For removing nodes
    // This just makes the removed node is nulled-out so it goes back into the pool of nodes.
    memset(pNode, 0, (sizeof(EmployeeInformation) - sizeof(struct employee_data *)));
}

//===============================================================//
//===============================================================//
// Menu options.
char menu(void)
{
    // Variable for user input
    int ch;

    // Print the menu options
    printf("\n\n\n\n\n======================== MENU ========================");
    puts("\nWhat do you want to do with the employee records?\n");
    puts("Enter the letter corresponding to your choice:");
    puts("f) Find and display an employee  |  e) Edit an employee's salary\n");
    puts("r) Remove an employee            |  l) List all employees    \n");
    puts("q) quit             \n\n");

    // Parse the user input
    while ((ch = getchar()) != EOF)
    {
        while (getchar() != '\n') // discard rest of line
            continue;

        ch = tolower(ch);

        if (strchr("ferlq", ch) == NULL)
            puts("Please enter: f, e, r, l, or q:");
        else
            break;
    }

    if (ch == EOF) /// make EOF cause program to quit
        ch = 'q';

    return ch;
}

//===============================================================//
//===============================================================//
// Retreives the next empty node from the array. Equivalent to memset from previous exercises.
EmployeeInformation *GetNode()
{
    int i = 0;
    EmployeeInformation *pEmptyNode = NULL;

    // Look for an empty entry in the list
    // As long as emptyNode is less than our max list entries...
    // And the length of the word is != 0...
    // We just go to the next entry.    
    while (i < 20 && allEmps[i].employeeId != 0)
    {
        ++i;
    }

    if (i < 20)
    {
        // So this is the position in the array because we incremented in the 'while' loop
        //                          |
        // (The array name)         |
        //              |           |
        //              v           v
        pEmptyNode = &allEmps[i];
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