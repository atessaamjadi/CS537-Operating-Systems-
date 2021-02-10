////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (statistics.c)
// Other Files:      (queue.c, queue.h, statistics.h, threads.c, threads.h)
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

//statistics initializer mallocs a stat struct and initializes lock and variables
Statistics *initializeStat(){

    Statistics *newStats = (Statistics*)malloc(sizeof(Statistics));

    if (pthread_mutex_init(&newStats->lock, NULL) != 0){
        printf("error in mutex_init\n");
        exit(1);
    }

    newStats->enqueueCount = 0;
    newStats->dequeueCount = 0;
    newStats->enqueueTime = 0;
    newStats->dequeueTime = 0;

    return newStats;
}

//updates enqueue time
void updateEnqueueTime(Statistics *newStats, clock_t start){
    
    //lock the monitor lock
    if(pthread_mutex_lock(&newStats->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }

    //get end time
    clock_t end = clock();
    if(end == -1) {
        printf("clock failed\n");
        exit(1);
    }

    //update enqueue time 
    newStats->enqueueTime = newStats->enqueueTime + ((float)(end - start)) / CLOCKS_PER_SEC;

    //unlock monitor lock
    if (pthread_mutex_unlock(&newStats->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }
}

//updates enqueue count
void updateEnqueueCount(Statistics *newStats){
    
    //lock the monitor lock
    if(pthread_mutex_lock(&newStats->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }

    //updates enqueue count by +1
    newStats->enqueueCount = newStats->enqueueCount + 1;
       

    //unlock monitor lock
    if (pthread_mutex_unlock(&newStats->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }
}

//updates dequeue time
void updateDequeueTime(Statistics *newStats, clock_t start){
    
    //lock the monitor lock
    if(pthread_mutex_lock(&newStats->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }

    //get end time
    clock_t end = clock();
    if(end == -1) {
        printf("clock failed\n");
        exit(1);
    }

    //update dequeue time
    newStats->dequeueTime = newStats->dequeueTime + ((float)((end - start)) / CLOCKS_PER_SEC);

    //unlock monitor lock
    if (pthread_mutex_unlock(&newStats->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }
}

//updates dequeue count
void updateDequeueCount(Statistics *newStats){
    
    //lock the monitor lock
    if(pthread_mutex_lock(&newStats->lock) != 0) {
        printf("error in mutex_lock\n");
        exit(1);
    }

    //updates dequeue count by +1
    newStats->dequeueCount = newStats->dequeueCount + 1;
       

    //unlock monitor lock
    if (pthread_mutex_unlock(&newStats->lock) != 0){
        printf("mutex unlock failed\n");
        exit(1);
    }
}

//print statistics 
void PrintStats(Statistics *newStats){
    //check for bad arguments
    if(newStats == NULL) {
        printf("bad arguments passed in\n");
        exit(1);
    }
    //else print stats
    fprintf(stderr, "Enqueue count: %i\n Dequeue count: %i\n Enqueue time: %f\n Dequeue time: %f\n", newStats->enqueueCount, newStats->dequeueCount, newStats->enqueueTime, newStats->dequeueTime);
}
