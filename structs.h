#ifndef STRUCTS_H
#define STRUCTS_H

/**
 * This is a standard node struct used for AVL and Huffman Tree implementation
 * token: word string
 * left/right: respective children
 * encoding: huffman encoding
 * freq: frequency of token
 * height: height of tree, used for rebalancing purposes
 */
typedef struct node
{
    char* token;
    struct node* left;
    struct node* right;
    char* encoding;
    int freq;
    int height;
} node;

#endif