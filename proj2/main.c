////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (main.c)
// Other Files:      (queue.c, queue.h, statistics.c statistics.h, threads.c, threads.h)
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
#include <ctype.h>
#include <string.h>

#include "queue.h"
#include "threads.h"
#include "statistics.h"

//main function creates three queues and four pthreads, and prints statistics
int main() {
    const int queueSize = 10;

    //create 3 queues
    Queue *reader = CreateStringQueue(queueSize);
    Queue *munch1 = CreateStringQueue(queueSize);
    Queue *munch2 = CreateStringQueue(queueSize);

    //create four pthreads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    //create queue pointer for queue pointers, so we can pass in multiple queues
    Queue **munch1input = (Queue**)malloc(sizeof(Queue*) * 2);
    munch1input[0] = reader;
    munch1input[1] = munch1;

    Queue **munch2input = (Queue**)malloc(sizeof(Queue*) * 2);
    munch2input[0] = munch1;
    munch2input[1] = munch2;
    
    //call pthread create
    if(pthread_create(&thread1, NULL, readerFunc, (void *)reader) != 0) {
        printf("error in pthread_create\n");
        exit(1);
    }

    
    if(pthread_create(&thread2, NULL, munch1Func, (void *)munch1input) != 0) {
        printf("error in pthread_create\n");
        exit(1);
    }
    
    if(pthread_create(&thread3, NULL, munch2Func, (void *)munch2input) != 0) {
        printf("error in pthread_create\n");
        exit(1);
    }

    if(pthread_create(&thread4, NULL, writerFunc, (void *)munch2) != 0) {
        printf("error in pthread_create\n");
        exit(1);
    }

    //call pthread_join
    if(pthread_join(thread1, NULL) != 0) {
        printf("error in pthread_join\n");
        exit(1);       
    }
    
    
    if(pthread_join(thread2, NULL) != 0) {
        printf("error in pthread_join\n");
        exit(1);  
    }
    
    if(pthread_join(thread3, NULL) != 0) {
        printf("error in pthread_join\n");
        exit(1);  
    }

    if(pthread_join(thread4, NULL) != 0) {
        printf("error in pthread_join\n");
        exit(1);  
    }

    

    //print out statistics to stderr for each queue using the PrintQueueStats function
    PrintQueueStats(reader);
    PrintQueueStats(munch1);
    PrintQueueStats(munch2);

    //free and null charArray pointers
    free(reader->charArray);
    reader->charArray = NULL;
    free(munch1->charArray);
    munch1->charArray = NULL;
    free(munch2->charArray);
    munch2->charArray = NULL;

    //free and null Statistics pointers
    free(reader->newStats);
    reader->newStats = NULL;
    free(munch1->newStats);
    munch1->newStats = NULL;
    free(munch2->newStats);
    munch2->newStats = NULL;

    //free and null queue pointers
    free(reader);
    reader = NULL;
    free(munch1);
    munch1 = NULL;
    free(munch2);
    munch2 = NULL;

    //free and null queue pointer pointers
    free(munch1input);
    munch1input = NULL;
    free(munch2input);
    munch2input = NULL;

   return 0;
}


