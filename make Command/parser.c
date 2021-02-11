////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (parser.c)
// Other Files:      (graph.h, process.c process.h, graph.c, parser.h)
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

Rule *initRule(char *buf, int lineNum);

//opens make or makefile, reads lines and tokenizes valid lines
Rule **parser(FILE *fp){
    const int BUFFSIZE = 4096;
    
    Rule **rules = NULL;

    
    int c;
    //keep track of line num for error messages, number of rules, and buf index
    int lineNum = 1;
    int ruleIndex = -1;
    int bufferIndex = 0;

    //fields for target and dependencies
    int beforeWasTarget = 0;

    //fields for commands
    int commandIndex1 = 0;
    int commandIndex2 = 0;

    //read makefile
    while((c = fgetc(fp)) != EOF) {
        bufferIndex = 0;

        //skip if newline
        if(c == '\n') {
            lineNum++;
            continue;
        }
        
        //malloc a new line 
        char *buf = malloc(sizeof(char) * BUFFSIZE); 

        if(buf == NULL){
            fprintf(stderr, "error in malloc\n");
            exit(1);
        }

        while(c != '\n' && c != EOF){
            //if a null byte is found it's an error
            if(c == '\0'){
                fprintf(stderr, "%i: Error: Null (zero) byte found %s\n", lineNum, buf);
                free(buf);
                exit(1);
            }
            //if line is greater than BUFFSIZE it's an error
            if(bufferIndex > BUFFSIZE - 1){
                fprintf(stderr, "%i: Error: Length of line was larger than BUFFSIZE %s\n", lineNum, buf);
                free(buf);
                exit(1);
            }
            //else add to buf
            buf[bufferIndex] = c;   
            bufferIndex++;
            c = fgetc(fp);
        }  
        //eliminate comment lines
        if(buf[0] == '#') {
            free(buf);
            lineNum++;
        }

        //eliminate if blank
        else if(isBlank(buf, bufferIndex)){       
            free(buf);
            lineNum++;
        }
        
        //else add null byte at the end of line AND tokenize line :) 
        else {
            buf[bufferIndex] = '\0';

            //if target line ...
            char* str = strstr(buf, ":");
            if(str != NULL){ 

                ruleIndex++;    

                rules = (Rule**)realloc(rules, (ruleIndex + 2) * sizeof(Rule*));
                if(rules == NULL){
                    fprintf(stderr, "Realloc of rules failed\n");
                    exit(1);
                }

                rules[ruleIndex] = initRule(buf, lineNum);  
                           
                rules[ruleIndex]->targetlineNum = lineNum;

                beforeWasTarget = 1;
                lineNum++;   
            }
            //check to see if line is a command line
            else if(buf[0] == '\t'){
                commandIndex2 = 0;

                //reset commandIndex1 if new rule, and malloc new commands field
                if(beforeWasTarget == 1){  
                    //beforeWasTarget = 0;
                    commandIndex1 = 0;

                    //make one extra space so it will always be null terminated
                    rules[ruleIndex]->commands = (char***)malloc(sizeof(char**) * 2);

                    if(rules[ruleIndex]->commands == NULL){
                        fprintf(stderr, "Malloc of commands failed\n");
                        exit(1);
                    }
                    //null terminate extra space malloced
                    rules[ruleIndex]->commands[1] = NULL;
                             
                }
                else {
                    //make one extra space so it will always be null terminated
                    rules[ruleIndex]->commands = (char***)realloc(rules[ruleIndex]->commands, ((commandIndex1 + 2) * sizeof(char**)));
                    rules[ruleIndex]->commands[commandIndex1 + 1] = NULL; 
                }

                rules[ruleIndex]->commands[commandIndex1] = (char**)malloc(sizeof(char*));

                if(rules[ruleIndex]->commands[commandIndex1] == NULL){
                        fprintf(stderr, "Malloc of commandIndex1 failed\n");
                        exit(1);
                }    
                
                //get the first command
                char *commandLine = strtok(buf, " \t");
      
                rules[ruleIndex]->commands[commandIndex1][commandIndex2] = commandLine;

                commandIndex2++;
                
                //continue to get more commands if there are any
                while((commandLine = strtok(NULL, " \t")) != NULL){
                    
                    rules[ruleIndex]->commands[commandIndex1] = (char**)realloc(rules[ruleIndex]->commands[commandIndex1], (commandIndex2 + 1) * sizeof(char*));
                    if(rules[ruleIndex]->commands[commandIndex1] == NULL){
                        fprintf(stderr, "Realloc of commandIndex1 failed\n");
                        exit(1);
                    }

                    rules[ruleIndex]->commands[commandIndex1][commandIndex2] = commandLine;

                    commandIndex2++;
                }


                rules[ruleIndex]->commands[commandIndex1] = (char**)realloc(rules[ruleIndex]->commands[commandIndex1], (commandIndex2 + 1) * sizeof(char*));

                if(rules[ruleIndex]->commands[commandIndex1] == NULL){
                    fprintf(stderr, "Realloc of commandIndex1 failed\n");
                    exit(1);
                }


                //add null pointer at end of command for execvp()
                rules[ruleIndex]->commands[commandIndex1][commandIndex2] = NULL;

                beforeWasTarget = 0;
                commandIndex1++;
                lineNum++;   
            }
            else {
                fprintf(stderr, "%i: Error: line did not start with a target or tab %s\n", lineNum, buf);
                exit(1); 
            }
        }
    }
    //if EOF, close file and return rules
    if(c == EOF){
        rules[ruleIndex + 1] = NULL;
        fclose(fp);
        return rules;
    }
    return rules;
}

int isBlank(char *buf, int bufferIndex){
    int isBlank = 1;
    for(int j = 1; j < bufferIndex - 1; j++){
        if((buf[j] != ' ') && (buf[j] != '\t')){
            isBlank = 0;
        }
    }
    return isBlank;
}

//takes a line and gives back target and dependencies
Rule *initRule(char *buf, int lineNum){
    int dependIndex = 0;

    Rule *rule = (Rule*)malloc(sizeof(Rule));
    //if a new target line is found, malloc a new rule
    if(rule == NULL){
        fprintf(stderr, "Malloc of rule failed\n");
        exit(1);
    }

    //split by ':' to get target 
    char *line = strtok(buf, ":");

    //make sure there was a target specified
    if (line == NULL) {
        fprintf(stderr, "%i: Error: no target was specified %s\n", lineNum, buf);
        exit(1); 
    }

    //make sure target doesnt have extra spaces or tabs
    char* savePointer;
    char* fixLine = strtok_r(line, " \t", &savePointer);

    rule->target = fixLine;
    
    //splitting by space to get dependencies
    rule->dependencies = (char**)malloc(sizeof(char*));
    if(rule->dependencies == NULL){
        fprintf(stderr, "Malloc of dependencies failed\n");
        exit(1);
    }

    while((line = strtok(NULL, " \t")) != NULL){
        rule->dependencies = (char**)realloc(rule->dependencies, (dependIndex + 2) * sizeof(char*));
        if(rule->dependencies == NULL){
            fprintf(stderr, "Realloc of dependencies failed\n");
            exit(1);
        }


        rule->dependencies[dependIndex] = line;

        dependIndex++;
    }

    //null terminate dependencies
    rule->dependencies[dependIndex] = NULL;

    return rule;
}
