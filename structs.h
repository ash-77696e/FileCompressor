#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char* token;
    struct node* left;
    struct node* right;
    int freq;
    int height;
} node;

#endif