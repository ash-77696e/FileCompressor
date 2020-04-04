#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Converts tree structure to an array of nodes
 * in order to create a MinHeap
 * Uses a pre-order traversal of the tree to set each index of the array
 * */
int createHeap(node** heap, node* root, int i)
{
    if(root == NULL) //base case
        return i;
    
    heap[i++] = root; //set index of array to the node
    if(root->left != NULL)
        i = createHeap(heap, root->left, i);
    if(root->right != NULL)
        i = createHeap(heap, root->right, i);
    
    return i;
}

/**
 * Maintains heap strucutre by checking if parent is indeed
 * smaller than both children. If the child is smaller than the parent,
 * parent and child is swapped, and heapify is called again because the structure
 * has updated
 * */
void heapify(node** heap, int size, int parent)
{
    int smallest = parent;
    int leftChild = 2*parent + 1; //get left child
    int rightChild = 2*parent + 2; //get right child

    if(leftChild < size && heap[leftChild]->freq < heap[smallest]->freq) //get smallest out of parent and children
        smallest = leftChild;
    if(rightChild < size && heap[rightChild]->freq < heap[smallest]->freq)
        smallest = rightChild;
    
    if(smallest != parent) //if child is smaller than parent, swap and heapify again
    {
        node* temp = heap[parent];
        heap[parent] = heap[smallest];
        heap[smallest] = temp;
        heapify(heap, size, smallest);
    }
}

/**
 * Given an array of nodes, creates a MinHeap
 * with comparaing factor being frequency
 * Uses heapify in order to set structure of heap
 * */
void buildHeap(node** heap, int size)
{
    int i;
    for(i = (size / 2) - 1; i >= 0; i--) //starting from the last element that has a child, call heapify
    {
        heapify(heap, size, i);
    }

    for(i = 0; i < size; i++) //initializes all children to NULL
    {
        heap[i]->left = NULL;
        heap[i]->right = NULL;
    }
}

/**
 * Extracts the smallest element of MinHeap
 * Uses heapify in order to maintain MinHeap structure
 * */
node* extractMin(node** heap, int* size)
{
    node* temp = heap[0]; //swap root with last item in heap
    heap[0] = heap[(*size)-1]; //delete "root", which is the min
    (*size)--;
    heapify(heap, *size, 0); //heapify to reset the new min as the root

    return temp;
}

/**
 * Insertion of node into MinHeap. Performs sift up when necessary in order
 * to maintain MinHeap structure
 * */
void insertIntoHeap(node** heap, node* newNode, int* size)
{
    *size = (*size)+1;

    int i = (*size)-1;
    heap[i] = newNode; //inserts new node to bottom of heap
    
    //sift up
    while(i != 0)
    {
        int parent = (i-1)/2; //get parent
        if(newNode->freq < heap[parent]->freq) //check if swapping with parent is needed
        {
            node* temp = heap[i];
            heap[i] = heap[parent];
            heap[parent] = temp;
            i = (i-1)/2; //go to next parent
        }
        else
            break;
    }
}