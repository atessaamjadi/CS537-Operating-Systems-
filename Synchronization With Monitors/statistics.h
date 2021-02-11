#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct Statistics {
    int enqueueCount;
    int dequeueCount;
    float enqueueTime;
    float dequeueTime;

    pthread_mutex_t lock;

} Statistics;

Statistics *initializeStat();

void updateEnqueueTime(Statistics *newStat, clock_t start);

void updateEnqueueCount(Statistics *newStat);

void updateDequeueTime(Statistics *newStat, clock_t start);

void updateDequeueCount(Statistics *newStat);

void PrintStats(Statistics *newStats);

#endif