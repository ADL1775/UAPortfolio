#include "circular.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Using an array, implement a circular queue of up to 100 elements. 

Implement CircularDequeue() and CircularDequeue () functions to place numbers on the queue head and 
read them from the queue tail. 

A circular queue only saves the last n entries (where n is the number of elements in the queue). 

Overwrite the oldest entries with newest entries once the queue is full.

A main.c and makefile is supplied. The main.c source file contains a set of tests that verify your
implementation is correct.

typedef struct {
    int head;               // Head of the queue
    int tail;               // Tail of the queue

    int count;              // Number of entries in the queue   IMPORTANT
    
    int data[QUEUE_SIZE];   // Data in the queue
} CircularQueue;

#define QUEUE_SIZE 100
*/

// Start the queue with 0 entries
// Passes the address of the queue (a circular queue structure)
//                              |
//                              v
void CircularInitialize(CircularQueue * q)
{
    // Once we have the memory allocated, this just initializes the elements. 
    q->count = 0;
    /*
    if( q != NULL ) 
    {
        q->count = 0;
    }
    */
}

void CircularEnqueue(CircularQueue * q, int value)
{
    if(q->count < QUEUE_SIZE)	// Check if count is less than queue size
    {
        // Data at the head is equal to the value.
        q->data[q->head] = value;		
        
        // Increment the head by 1
        // We use the modulus operator here
        // Why?
        // Since we're using a circular queue, head will be the index of the queue.
        // But we don't want it to be greater than the queue size!
        //  (101 / 100) leaves a remainder of 1
        // So if we are on the 101st entry, it will go into index position 1.
        q->head = (q->head + 1) % QUEUE_SIZE;
        
        // Increase the count by 1.
        q->count++;
    }

    // WE PERFORM THIS WHEN THE QUEUE IS FULL!
    else
    {
        // Queue is full, so we need to add the new entry into the position...
        // ...that tail was previously in (q->head + 1) 
        
        // q->tail++;       // OLD
        // TODO: use modulus operator here 
        // (like in ‘head = (head + 1) % QUEUE_SIZE’ around 18:00 in lecture)  
        q->tail = (q->tail + 1) % QUEUE_SIZE;
        q->data[q->head++] = value;
    }
}

int CircularDequeue(CircularQueue * q, int *pValue)
{
    if(q->count > 0)
    {
        *pValue = q->data[q->tail];
        q->tail = (q->tail + 1) % QUEUE_SIZE;   // Move the tail up one one position.
        q->count--;
    }

    return *pValue;
}

/*
if( count < QUEUE_SIZE)	// Check if count is less than queue size
{
   data[head] = item;		
   head = (head + 1) % QUEUE_SIZE	
   count++;
}
else
{
   tail++;  // TODO: use modulus operator here (like in ‘head = (head + 1) % QUEUE_SIZE’ around 18:00 in lecture)
   data[head++] = item;
}
*/