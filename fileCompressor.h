#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "structs.h"
#include "AVL.h"
#include "minheap.h"
#include "IO.h"
#include "huffman.h"

int isDirectory(char* path);

node* buildAVLRecursive(char* basePath, node* root);

char* getFileExtension(char* fileName);

char* createHuffmanPath(char* path);

void build(node* root, char* huffmanPath);

void recursiveBuild(node* root, char* path);

void compress(node* root, char* huffmanPath, char* oldFile);

void recursiveCompress(char* basePath, char* huffmanPath, node* root);

void decompress(node* root, char* codebookPath, char* oldFilePath);

void recursiveDecompress(node* root, char* codebookPath, char* dirPath);

char* getCompressedFileName(char* path);

char* getDecompFileName(char* path);

#endif