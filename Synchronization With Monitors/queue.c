////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (queue.c)
// Other Files:      (queue.h, statistics.c statistics.h, threads.c, threads.h)
// Semester:         CS 537 Fall 2020
//
// Author:           (Atessa Amjadi, Katie Telford)
// NetID:            (aamjadi, ktelford)
// CS Login:         (atessa, telford)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
/////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "queue.h"
#include "threads.h"
#include "statistics.h"

/*
dynamically allocates a new queue struct and initializes it with an array of 
character points of length size
*/
Queue *CreateStringQueue(int size){

    //malloc queue struct 
    Queue *newQueue = (Queue*)malloc(sizeof(Queue));

    if(newQueue == NULL){
        printf("Malloc of newQueue failed\n");
        exit(1);
    }

    //malloc charArray of size 'size'
    newQueue->charArray = (char**)malloc(sizeof(char*) * size);

    if(newQueue->charArray == NULL){
        printf("Malloc of charArray failed\n");
        exit(1);
    }

    //create new statistic struct pointer
    newQueue->newStats = initializeStat();

    //initialize queues fields
    newQueue->startIndex = 0;
    newQueue->endIndex = 0;
    newQueue->currSize = 0;
    newQueue->capacity = size;

    //initialize and error check queues lock and condition variables
    if (pthread_mutex_init(&newQueue->lock, NULL) != 0){
        printf("error in mutex_init\n");
        exit(1);
    }
    if (pthread_cond_init(&newQueue->full, NULL) != 0){
        printf("error in cond_init\n");
        exit(1);
    }
    
    if (pthread_cond_init(&newQueue->empty, NULL) != 0){
        printf("error in cond_init\n");
        exit(1);
    }
    
    return newQueue;
}

/*
places string pointer to end of queue q. 
if queue is full, function blocks until space is available
*/
void EnqueueString(Queue *q, char *string){

    //start enqueue time count
    clock_t start = clock();
    if(start == -1) {
        printf("clock failed\n");
        exit(1);
    }
    

    //lock the monitor lock
    if(pthread_mutex_lock(&q->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }


    //make sure arguments passed in are okay
    if(q == NULL){
        printf("bad arguments passed in\n");
        exit(1);
    }


    //wait if queue is full
    while((q->currSize == q->capacity)){
        if(pthread_cond_wait(&q->full, &q->lock) != 0){
            printf("error in cond_wait\n");
        }
    }

    //else enqueue :)
    q->charArray[q->endIndex] = string;
    q->endIndex = ((q->endIndex + 1) % q->capacity);
    q->currSize++;

    //update enqueue count
    updateEnqueueCount(q->newStats);

    //unblock if there are threads waiting
    if(pthread_cond_signal(&q->empty) != 0){
        printf("error in cond_signal\n");
        exit(1);
    }

    //update enqueue time
    updateEnqueueTime(q->newStats, start);

    //unlock monitor lock
    if (pthread_mutex_unlock(&q->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }

    return;
}

//removes a pointer to a string from the beginning of queue q. 
//if queue is empty, the function blocks until there is a string in the queue
char *DequeueString(Queue *q){

    //start dequeue time count
    clock_t start = clock();
    if(start == -1) {
        printf("clock failed\n");
        exit(1);
    }
    
    //lock the monitor lock
    if(pthread_mutex_lock(&q->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }   

    //check for bad arguments
    if(q == NULL) {
        printf("bad arguments passed in\n");
        exit(1);
    }

    //block if queue is empty
    while(q->currSize == 0){
        if(pthread_cond_wait(&q->empty, &q->lock) != 0){
            printf("error in cond_wait\n");
        }
    }

    //else remove pointer to string at beginning of queue
    int temp = q->startIndex;
    char * retval =  q->charArray[temp];
    q->startIndex = ((q->startIndex + 1) % q->capacity);
    q->currSize--;

    //update dequeue count
    updateDequeueCount(q->newStats);

    //notify waiting thread
    if(pthread_cond_signal(&q->full) != 0){
        printf("error in cond_signal\n");
        exit(1);
    }

    //update dequeue time
    updateDequeueTime(q->newStats, start);

    //unlock monitor
    if (pthread_mutex_unlock(&q->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }
  
    return retval;
}

void PrintQueueStats(Queue *q){
    //check for bad arguments
    if(q == NULL) {
        printf("bad arguments passed in\n");
        exit(1);
    }

    //else call stats print function
    PrintStats(q->newStats);
}

