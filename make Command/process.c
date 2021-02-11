////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (process.c)
// Other Files:      (graph.h, graph.c process.h, parser.c, parser.h, )
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> 

#include "parser.h"
#include "process.h"
#include "graph.h"

//forks all commands of a node
void execute(Node *node){
    pid_t childPid;
    int status;

    //get number of commands in a node
    int numCommands = 0;

    //dont execute if there are no commands
    if(!node->commands){
        return;
    }

    while(node->commands[numCommands] != NULL){
        numCommands++;
    }

    //do for all commands of a node
    for(int i = 0; i < numCommands; i++){

        //if fork fails, terminate process
        if((childPid = fork()) < 0){
        fprintf(stderr, "Error: could not fork child process\n");
        exit(1);
        }

        //else if, run child process
        else if(childPid == 0){
            //if execvp fails, terminate process
            if((execvp(node->commands[i][0], node->commands[i])) == -1) {
                fprintf(stderr, "Error: execvp failed for child process\n");
                exit(1);
            }
        }
        //else wait for child process
        else {
            //if wait fails, terminate process
            if(wait(&status) < 0){
                fprintf(stderr, "Error: wait failed\n");
                exit(1);
            }

            if(!WIFEXITED(status)) {
                fprintf(stderr, "Error: the child did not exit successfully. Status: %i\n", WEXITSTATUS(status));
                exit(1);
            }
            //error checking for status
            /*
            //if child exited normally, print status
            else if(status == 0) {
                fprintf(stdout, "The child exited normally with a status of: %i\n", WEXITSTATUS(status));

            }
            else {
                fprintf(stderr, "Error: the child did not exit successfully. Status: %i\n", WEXITSTATUS(status));
                exit(1);
            } 
            */  
        } 
    }  
}