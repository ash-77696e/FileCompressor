#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "structs.h"

void buildHuffmanTree(node** heap, int* size);

void printHuffman(node* root);

void freeHuffman(node* root);

void encode(node* root, int* huffmanCodeArr, int lengthOfEncoding);

node* buildHuffmanFromEncoding(node* root, node* strNode);

#endif