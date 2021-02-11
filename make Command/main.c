////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (main.c)
// Other Files:      (graph.h, graph.c, process.c, process.h, parser.c, parser.h)
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

#include "parser.h"
#include "process.h"
#include "graph.h"

int main(int argc, char* argv[]){
    FILE *fp;
    
    //check to make sure no more than two arguements were passed in!
    if(argc > 2) {
        fprintf(stderr, "Error: Too many arguements were passed in\n");
        exit(1);
    }

    //look for makefile or Makefile and build specification for first rule
    if(argc == 1) {
        //check to make sure the fist arguement is 537ps      
        if((fp = fopen("makefile", "r")) == NULL) {
            if((fp = fopen("Makefile", "r")) == NULL){
                fprintf(stderr, "Error: Could not find makefile or Makefile to open'n");
                exit(1);
            }
            //else send to parser with Makefile
            else {
                Rule **rules = parser(fp);

                int i = 0;
                
                Graph *graph = build_graph(rules);
                
                for(i = 0; i < graph->numTargets; i++){
                    if(findTargetInRules(rules, graph->nodes[i]->target) >= 2){
                        fprintf(stderr, "Error: Duplicate target");
                        exit(1);
                    }
                }
                if(detectCycle(graph) == -1){
                    
                    if(prepareCommands(graph, graph->nodes[0]) == -1){
                        fprintf(stderr, "Error: Make processing failed");
                        exit(1);
                    }
                    
                }
            }
        }
        //else send to parser with makefile
        else {
            Rule **rules = parser(fp);

            int i = 0;
            
            Graph *graph = build_graph(rules);
            
            for(i = 0; i < graph->numTargets; i++){
                if(findTargetInRules(rules, graph->nodes[i]->target) >= 2){
                    fprintf(stderr, "Error: Duplicate target");
                    exit(1);
                }
            }
            if(detectCycle(graph) == -1){
                
                if(prepareCommands(graph, graph->nodes[0]) == -1){
                    fprintf(stderr, "Error: Make processing failed");
                    exit(1);
                }
                
            }

        }
    }

    //else if look for makefile or Makefile and build specification for specified rule
    else if(argc == 2) {
         //check to make sure the fist arguement is 537ps      
        if((fp = fopen("makefile", "r")) == NULL) {
            if((fp = fopen("Makefile", "r")) == NULL){
                fprintf(stderr, "Error: Could not find makefile or Makefile to open");
                exit(1);
            }
            //else send to parser with Makefile
            else {
                Rule **rules = parser(fp);

                int i = 0;
                
                Graph *graph = build_graph(rules);
                
                for(i = 0; i < graph->numTargets; i++){
                    if(findTargetInRules(rules, graph->nodes[i]->target) >= 2){
                        fprintf(stderr, "Error: Duplicate target");
                        exit(1);
                    }
                }
                if(detectCycle(graph) == -1){
                    int argIndex = find(argv[1], graph->nodes);   
                    if(argIndex < 0){
                        fprintf(stderr, "Error: Target specified not found\n");
                        exit(1);
                    }
                    if(prepareCommands(graph, graph->nodes[argIndex]) == -1){
                        fprintf(stderr, "Error: Make processing failed");
                        exit(1);
                    }
                }
            }
        }
        //else send to parser with makefile
        else {
            Rule **rules = parser(fp);

            int i = 0;
            
            Graph *graph = build_graph(rules);
            
            for(i = 0; i < graph->numTargets; i++){
                if(findTargetInRules(rules, graph->nodes[i]->target) >= 2){
                    fprintf(stderr, "Error: Duplicate target");
                    exit(1);
                }
            }
            if(detectCycle(graph) == -1){
                int argIndex = find(argv[1], graph->nodes); 
                if(argIndex < 0){
                    fprintf(stderr, "Error: Target specified not found\n");
                    exit(1);
                }   
                if(prepareCommands(graph, graph->nodes[argIndex]) == -1){
                    fprintf(stderr, "Error: Make processing failed");
                    exit(1);
                }  
            }
        }
    }
    
    return 0;
}
