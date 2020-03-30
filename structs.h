#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct node
{
    char* token;
    struct node* left;
    struct node* right;
    int freq;
    int height;
} node;

#endif