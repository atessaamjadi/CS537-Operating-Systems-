#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct Queue {
    char **charArray;
    int startIndex;
    int endIndex;
    int currSize;
    int capacity; 

    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;

    struct Statistics *newStats;

    int enqueueCount;
    int dequeueCount;
    float enqueueTime;
    float dequeueTime;
} Queue;

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char *DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif