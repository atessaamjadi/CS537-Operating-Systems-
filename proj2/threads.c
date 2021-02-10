////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (threads.c)
// Other Files:      (queue.c, queue.h,statistics.h, statistics.c, threads.h)
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

//reader thread reads from standard input, one line at a time
//and takes each line of input and passes it to thread munch1 through a queue of character strings
void *readerFunc(void *reader) {

    const int BUFFSIZE = 4096;

    //cast function back to type struct Queue pointer
    reader = (struct Queue*)reader;

    int c;
    while(1){
        int i = 0;
        
        c = fgetc(stdin);

        while(c != EOF) {
            //malloc a new line every time
            char *buf = malloc(sizeof(char) * BUFFSIZE);

            if(buf == NULL){
                printf("error in malloc\n");
                exit(1);
            }

            char character = c;
            //adds to buff only if newline and EOF has not been reached 
            //and smaller than BUFFSIZE
            while(character != '\n' && c != EOF){
                //add to buf if < BUFFSIZE
                if(i < BUFFSIZE - 1){
                    buf[i] = character;
                }
                i++;
                c = fgetc(stdin);
                character = c;       
            }
            //do not include line if greater than BUFFSIZE
            if(i >= BUFFSIZE - 1){ 
                free(buf);
                fprintf(stderr, "Error: Length of line was larger than BUFFSIZE\n");
            }
            //else enqueue line with null terminator character
            else {
                buf[i] = '\0';
                EnqueueString(reader, buf);
            }
            //continue reading next lines 
            i = 0;
            c = fgetc(stdin);
        }

        //if EOF, enqueue null and pthread_exit()
        if(c == EOF){
            EnqueueString(reader, NULL);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

/*
munch1 thread gets lines from reader and replaces each space character 
(not tabs or newlines) with an asterisk ("*") character. 
*/
void *munch1Func(void *munch1input){
    const int BUFFSIZE = 4096;

    //cast input back to correct types
    struct Queue **munch1input2 = munch1input;
    struct Queue *reader = (struct Queue*)munch1input2[0];
    struct Queue *munch1 = (struct Queue*)munch1input2[1];
    
    while(1){
        int i = 0;

        //dequeue string from reader
        char *buf = DequeueString(reader);

        if(buf == NULL){
            EnqueueString(munch1, NULL);
            pthread_exit(NULL);
        }

        while(i < BUFFSIZE){
            //replace space with *
            if(isspace(buf[i])){
                buf[i] = '*';
                i++;
            }
            //add to munch1 queue if end of line is reached
            else if(buf[i] == '\0'){
                EnqueueString(munch1, buf);
                buf = DequeueString(reader);
                i = 0;
            }
            //enqueue null to indicate EOF
            if(buf == NULL){
                EnqueueString(munch1, NULL);
                pthread_exit(NULL);
            }
            else{
                i++;
            }
        }
        return NULL;
    }
    return NULL;
    
}

/*
munch 2 thread will scan the line and convert all lower case letters to upper case 
*/
void *munch2Func(void *munch2input){

    const int BUFFSIZE = 4096;

    //cast back to correct types
    struct Queue **munch2input2 = munch2input;
    struct Queue *munch1 = (struct Queue*)munch2input2[0];
    struct Queue *munch2 = (struct Queue*)munch2input2[1];

    while(1){
        int i = 0;

        char *buf = DequeueString(munch1);

        if(buf == NULL){
            EnqueueString(munch2, NULL);
            pthread_exit(NULL);
        }

        while(i < BUFFSIZE){
            //if end of line is reached, add to munch2 queue
            if(buf[i] == '\0'){
                EnqueueString(munch2, buf);

                buf = DequeueString(munch1);
                i = 0;
            }
            //if EOF is reached, enqueue NULL
            if(buf == NULL){
                EnqueueString(munch2, NULL);
                pthread_exit(NULL);
            }
            //change all lower case to upper
            else{
                buf[i] = toupper((unsigned char) buf[i]);
                i++;
            }
        }
        return NULL;
    }
    return NULL;
}

/*
Writer will write the line to standard output.
*/
void *writerFunc(void *munch2){

    int numStrings = 0;

    //cast to correct type
    munch2 = (struct Queue*)munch2;

    char* buf;

    //while EOF string is not reached, print out the lines from munch2 
    //and print number of strings processed
    while((buf = DequeueString(munch2)) != NULL){
        numStrings++;
        fprintf(stdout, "%s\n", buf);
        free(buf);
        buf = NULL;
    }
    fprintf(stdout, "Number of strings processed: %i\n", numStrings);
    pthread_exit(NULL);
    return NULL;
}
