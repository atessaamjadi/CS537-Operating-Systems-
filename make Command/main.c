#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "process.h"
#include "graph.h"

// MAKE SURE TO PRINT ERROR IF PROCESSCOMMANDS FAILS

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

    

    //THIS was the old way of running in main to test parser.c and process.c

    /*
    //Rule **rules = parser("makefile");
    FILE *fp;
    fp = fopen("makefile", "r");
    Rule **rules = parser(fp);

    int numRules = 0;
    while(rules[numRules] != NULL){
        numRules++;
    }

    for(int i = 0; i < numRules; i++){
        execute(rules[i]);
    }
    */
    
    return 0;
}
