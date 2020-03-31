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
#include "heap.h"
#include "IO.h"
#include "huffman.h"

int isDirectory(const char* path);

void printAllFiles(char* basePath);

#endif