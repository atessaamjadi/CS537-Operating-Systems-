#ifndef _THREADS_H_
#define _THREADS_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

void *readerFunc(void *reader);

void *munch1Func(void *munch1input);

void *munch2Func(void *munch2input);

void *writerFunc(void *munch2);

#endif