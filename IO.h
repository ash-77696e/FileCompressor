#ifndef IO_H
#define IO_H

#include "structs.h"

node* buildAVLFromFile(char* path, node* root);

void writeHuffmanCodebook(int fd, node* root);

node* buildAVLFromHuffman(char* path, node* root);

void compressFile(char* oldFilePath, char* newFilePath, node* root);

node* buildHuffmanFromFile(char* path, node* root);

void decompressFile(char* oldPath, char* newPath, node* root);

#endif