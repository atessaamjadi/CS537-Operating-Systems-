#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule{
    char *target;
    char **dependencies;
    char ***commands;

    int targetlineNum;

    //last entry of all rules, command lines, commands, and dependencies all have NULL pointers

}Rule;

Rule **parser(FILE *fp);

int isBlank(char *buf, int bufferIndex);

Rule *initRule(char *buf, int lineNum);

#endif