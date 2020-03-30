#ifndef HEAP_H
#define HEAP_H

#include "structs.h"

int createHeap(node** heap, node* root, int i);

void buildHeap(node** heap, int size);

void heapify(node** heap, int size, int parent);

#endif