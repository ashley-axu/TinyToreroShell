/*
 * The Tiny Torero Shell (TTSH)
 *
 * Create a data structure to represent a circular queue
 * to store and executed the history of commands.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "history_queue.h"

// history will be the array used by our circular queue of HistoryEntry items.
// Note that the "static" keyword means that this global variable is accessible
// only within this file.
static HistoryEntry history[MAXHIST]; 

// Add support for the history command, which will print out a list of
// the most recent commands the user has entered.
static int queue_start = 0; // the index where queue begins
static int queue_next = 0;  // The index where we will insert next item.
                            // This should change when we add something
                            // to the queue.
static int queue_size = 0;  // The number of items in the queue.


/** 
* Add a new value to the queue (and update queue state), the value added
* should replace the first item in the queue if the array is full.
*/
void addEntry(char cmdline[MAXLINE]){
    queue_size++;
    strcpy(history[queue_next].cmdline, cmdline); 
    history[queue_next].cmd_num = queue_size; 
    queue_next = (queue_next + 1) % MAXHIST;
    
    if (queue_size > MAXHIST){
        queue_start = (queue_start + 1) % MAXHIST;
    }
}

/*
* Prints out the most recent commands.
*/
void printHistoryQueue(){
    for (int i = 0, j = queue_start; i < MAXHIST; i++, j++){
        if(j == MAXHIST){ //if reach to the maximum
            j = 0; // set the index to 0
        }
        // if(strncmp(history[j].cmdline, "", strlen(history[j].cmdline)) == 0){
        if(strlen(history[j].cmdline) == 0){ // If equals
            continue;
        }
        else{ //otherwise prints out the history commands
            fprintf(stdout, "%u \t %s", history[j].cmd_num, history[j].cmdline);  
        }
    }
}
