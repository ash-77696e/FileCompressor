#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int createHeap(node** heap, node* root, int i)
{
    if(root == NULL)
        return i;
    
    heap[i++] = root;
    if(root->left != NULL)
        i = createHeap(heap, root->left, i);
    if(root->right != NULL)
        i = createHeap(heap, root->right, i);
    
    return i;
}

void heapify(node** heap, int size, int parent)
{
    int smallest = parent;
    int leftChild = 2*parent + 1;
    int rightChild = 2*parent + 2;

    if(leftChild < size && heap[leftChild]->freq < heap[smallest]->freq)
        smallest = leftChild;
    if(rightChild < size && heap[rightChild]->freq < heap[smallest]->freq)
        smallest = rightChild;
    
    if(smallest != parent)
    {
        node* temp = heap[parent];
        heap[parent] = heap[smallest];
        heap[smallest] = temp;
        heapify(heap, size, smallest);
    }
}

void buildHeap(node** heap, int size)
{
    int i;
    for(i = (size / 2) - 1; i >= 0; i--)
    {
        heapify(heap, size, i);
    }

    for(i = 0; i < size; i++)
    {
        heap[i]->left = NULL;
        heap[i]->right = NULL;
    }
}

node* extractMin(node** heap, int* size)
{
    node* temp = heap[0];
    heap[0] = heap[(*size)-1];
    (*size)--;
    heapify(heap, *size, 0);

    return temp;
}

void insertIntoHeap(node** heap, node* newNode, int* size)
{
    *size = (*size)+1;
    int i = (*size)-1;
    
    while(i != 0 && (newNode->freq < heap[(i-1)/2]->freq))
    {
        heap[i] = heap[(i-1)/2];
        i = (i-1)/2;
    }

    heap[i] = newNode;
}