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
    int left = 2*parent + 1;
    int right = 2*parent + 2;

    if(left < size && heap[left]->freq < heap[smallest]->freq)
        smallest = left;
    if(right < size && heap[right]->freq < heap[smallest]->freq)
        smallest = right;
    
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
}
