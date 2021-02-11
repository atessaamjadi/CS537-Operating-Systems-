#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node{
    char *target;
    char **dependencies;
    char ***commands;

    int isFile;
    int visited;
    int noCycles;
    int targetLineNum;
}Node;

typedef struct Graph{
    int numTargets;
    int numNodes;
    Node **nodes;
    //int **matrix;

}Graph;

Node* createTargetNode(Rule *rule);

Node* createFileNode(char *dependency, Rule *rule);

Graph *build_graph(Rule **rules);

int detectCycle(Graph *graph);

//int detectCycleHelper(Graph *graph, Node *node);
int detectCycleHelper(Graph *graph, int nodeIndex);

int find(char *targetToCheck, Node **nodes);

int findTargetInRules(Rule **rules, char *target);

int prepareCommands(Graph *graph, Node *node);

#endif
