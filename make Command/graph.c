////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (graph.c)
// Other Files:      (graph.h, process.c process.h, parser.c, parser.h, )
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
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parser.h"
#include "graph.h"
#include "process.h"

//creates node representation of a rule
Node* createTargetNode(Rule *rule){

    Node *newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL){
        fprintf(stderr, "ERROR: Malloc of Node failed\n");
        exit(1);
    }

    //assign all of a rule's fields to node
    newNode->target = rule->target;
    newNode->dependencies = rule->dependencies;
    newNode->commands = rule->commands;

    newNode->visited = 0;
    newNode->noCycles = 0;
    newNode->isFile = 0;
    newNode->targetLineNum = rule->targetlineNum;

    return newNode;
}

// files can only be dependency
// function creates a node for a file
Node* createFileNode(char *dependency, Rule *rule){
  Node *newNode = (Node*)malloc(sizeof(Node));

  if(newNode == NULL){
      fprintf(stderr, "ERROR: Malloc of Node failed\n");
      exit(1);
  }

  //assign all of a rule's fields to node
  newNode->target = dependency;
  newNode->dependencies = NULL;
  newNode->commands = NULL;
  newNode->visited = 0;
  newNode->noCycles = 0;
  newNode->isFile = 1;
  newNode->targetLineNum = rule->targetlineNum;

  return newNode;
}

//takes in all rules and builds graph :)
Graph *build_graph(Rule **rules){

    //create Graph struct, and assign all rules to it's nodes field
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    if(graph == NULL){
        fprintf(stderr, "ERROR: Malloc of graph failed\n");
        exit(1);
    }
    int i;

    // count targets
    graph->numTargets = 0;
    while(rules[graph->numTargets] != NULL){
        graph->numTargets++;
        graph->numNodes++;
    }
    
    // count dependencies
    int numDependencies = 0;
    for(i = 0; i < graph->numTargets; i++){
        int j = 0;
        while(rules[i]->dependencies[j] != NULL){
            numDependencies++;
            graph->numNodes++;
            j++;
        }
    }

    // will hold max number of nodes possible
    graph->nodes = (Node**)calloc(graph->numNodes + 1, sizeof(Node*));

    if(graph->nodes == NULL){
        fprintf(stderr, "ERROR: Malloc of nodes failed\n");
        exit(1);
    }
    i = 0;
    int nodeIndex = 0;
    
    // NODES FOR TARGETS
    // looping through all targets and making each one a node
    while(rules[i] != NULL){
        // if we haven't made a node for this target yet

        if(find(rules[i]->target, graph->nodes) == -1){

            graph->nodes[nodeIndex] = createTargetNode(rules[i]);
            nodeIndex++;
        }
        //else update a dependency to be a target node
        else {
            // throw error
            fprintf(stderr, "Error: We found a file in the graph when we shouldn't have\n");
        }
        int depenIndex = 0;

        //set target nodes fields of dependencies etc

        // NODES FOR FILES
        // looping through all dependencies of current target
        // every dependency that is a file becomes a file node
        while(rules[i]->dependencies[depenIndex] != NULL){
            //fflush (stdout);
            
            // if we haven't made a node for this dependency yet
            if(find(rules[i]->dependencies[depenIndex], graph->nodes) == -1){
                // check if the dependency is a target
                // enter if statement if dependency is not a target
                if(findTargetInRules(rules, rules[i]->dependencies[depenIndex]) == 0){
                    graph->nodes[nodeIndex] = createFileNode(rules[i]->dependencies[depenIndex], rules[i]);

                    nodeIndex++;   
                // there is no else statement here because if the dependency is not a file
                // then it is a target so it will be created in a later iteration of the outer most while loop
                }
            }              
            depenIndex++;       
        }
        i++;
    }
    int updateNumNodes = 0;
    while(graph->nodes[updateNumNodes] != NULL){
        updateNumNodes++;
    }
    graph->numNodes = updateNumNodes;
    
    return graph;
}

int detectCycle(Graph *graph){
    for(int i = 0; i < graph->numNodes; i++){

        // sending each node to cycle detector
        //if(detectCycleHelper(graph, graph->nodes[i])) return i;
        if(detectCycleHelper(graph, i)) return i;
    }
    return -1;
}

// takes in graph and node
// returns 1 if cycle found
int detectCycleHelper(Graph *graph, int nodeIndex){
    Node *node = graph->nodes[nodeIndex];
    // we have already checked this node and its children for cycles
    //fprintf(stderr, "the node %s has noCycles value of %i\n", node->target, node->noCycles);
    if(node->noCycles == 1){
        return 0;
    }
    // if we haven't visited the node yet
    if(node->visited == 0){
        node->visited = 1;
        int i = 0;
        // while we have more dependencies belonging to the target

        if(node->dependencies != NULL){
            while(node->dependencies[i] != NULL){
                int depenIndex = find(node->dependencies[i], graph->nodes);
                //detectCycleHelper(graph, graph->nodes[depenIndex]);
                detectCycleHelper(graph, depenIndex);
                i++;
            }
        }
        node->noCycles = 1;
    }else{
        // FOUND A CYCLE
        fprintf(stderr, "ERROR: Cycle detected on line %i for target: %s\n", node->targetLineNum, node->target);
        exit(1);
    }
    return 0;
}


// implies we send in array of all nodes
// returns -1 if we can't find the target
int find(char *targetToCheck, Node **nodes){
    int i = 0;
    
    // if we have a node
    while(nodes[i] != NULL){
      // get the target
      char *target = nodes[i]->target;
      if(strcmp(targetToCheck, target) == 0){
          // return index of found node
          return i;
      }
      i++;
    }
    // if we fail
    return -1;
}

// pass in all rules and target to try and find duplicates of
// returns number of times we found target in the list of rules
// pass in a target to check for duplicates - will return 2 or more if duplicate
// pass in a dependency to see if it is a target - will return 1
// returns 0 if parameter is not found in list of targets
int findTargetInRules(Rule **rules, char *target){
    int i = 0;
    int counter = 0;
    // loop through all rules looking for target
    // if there is a duplicate then target should appear twice
    while(rules[i] != NULL){
        if(strcmp(rules[i]->target, target) == 0)
          counter++;
        i++;
    }
    return counter;
}

// return -1 if fail
// 1 if success
int prepareCommands(Graph *graph, Node *node){
  int needToMake = 0; // set to 1 if out of date and we need to build
  int targetIndex = find(node->target, graph->nodes);
  struct stat targetFile;
  int statReturn;
  time_t targetModTime;

  // if target isn't valid
  if(targetIndex < 0){
      fprintf(stderr, "ERROR: Target index was invalid\n");
      exit(1);
  }

  int i = 0;


  // looping through all nodes that our current target depends on
  if(graph->nodes[targetIndex]->dependencies != NULL){
    while(graph->nodes[targetIndex]->dependencies[i] != NULL){
        // getting the node index of the current dependency
        int depenIndex = find(graph->nodes[targetIndex]->dependencies[i], graph->nodes);
        needToMake |= prepareCommands(graph, graph->nodes[depenIndex]);
        // return -1 if we failed before this point
        if(needToMake < 0){
            return needToMake;
        }
        i++;
    }
  }

  // enter if no children were out of date
  // run commands if we don't find the file
  // check if we are up to date if we find the file
  if(!needToMake){
      statReturn = stat(node->target, &targetFile);
      // fail
      if(statReturn == -1){
          // file not found
          if(errno == ENOENT){
              needToMake = 1;
          }else{ // other error
              return -1;
          }
      // file opened successfully
      }else{
          targetModTime = targetFile.st_mtime;
          int currDepenIndex = 0;

          // check mod time of each dependency
          if(graph->nodes[targetIndex]->dependencies != NULL){
            while((graph->nodes[targetIndex]->dependencies[currDepenIndex] != NULL) && !needToMake){
                struct stat dependencyFile;
                char *currDepen = graph->nodes[targetIndex]->dependencies[currDepenIndex];
                if(stat(currDepen, &dependencyFile) == 0){
                    time_t dependencyModTime = dependencyFile.st_mtime;
                    if(dependencyModTime > targetModTime){
                        needToMake = 1;
                    }
                }
                currDepenIndex++;
            }
          }
      }
  }
  if(needToMake){
      execute(graph->nodes[targetIndex]);
  }
  return needToMake;
}
