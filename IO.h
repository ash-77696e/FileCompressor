#ifndef IO_H
#define IO_H

#include "structs.h"

node* buildAVLFromFile(const char* path, node* root);

void writeHuffmanCodebook(int fd, node* root);

node* buildAVLFromHuffman(int fd, node* root);

void compressFile(int oldFD, int compFD, node* root);

node* buildHuffmanFromFile(int fd, node* root);

#endif