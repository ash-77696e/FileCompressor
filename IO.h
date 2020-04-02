#ifndef IO_H
#define IO_H

#include "structs.h"

node* buildAVLFromFile(const char* path, node* root);

void writeHuffmanCodebook(int fd, node* root);

node* buildAVLFromHuffman(int fd, node* root);

#endif