#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "minheap.h"
#include "structs.h"

node* buildHuffmanFromEncoding(node* root, node* strNode)
{
    int index = 0;
    node* ptr = root;

    while(strNode->encoding[index] != '\0') //walk through encoding
    {
        int val = (int) (strNode->encoding[index] - '0');
        if(val == 0) //if current char at index is 0, go left
        {
            if(ptr->left == NULL)
            {
                ptr->left = (node*) malloc(sizeof(node));
                ptr->left->encoding = NULL;
                ptr->left->token = NULL;
            } 

            ptr = ptr->left;
        }

        if(val == 1) //if current char at index is 1, go right
        {
            if(ptr->right == NULL)
            {
                ptr->right = (node*) malloc(sizeof(node));
                ptr->right->encoding = NULL;
                ptr->right->token = NULL;
            }

            ptr = ptr->right;
        }

        index++;
    }

    ptr->encoding = strNode->encoding; //after reaching a leaf node, set node's encoding and token
    ptr->token = strNode->token;
    return root;
}

/**
 * Huffman Tree Algorithm
 * */
void buildHuffmanTree(node** heap, int* size)
{
    if(*size == 1) //if only 1 node in heap, set as left child of root
    {
        node* left = extractMin(heap, size);
        node* parent = (node*) malloc(sizeof(node));

        parent->freq = left->freq;
        parent->token = NULL;
        parent->encoding = NULL;
        parent->left = left;
        parent->right = NULL;
        insertIntoHeap(heap, parent, size);
        return;
    }

    while(*size != 1)
    {
        node* left = extractMin(heap, size); //extract two minimum
        node* right = extractMin(heap, size);

        node* parent = (node*) malloc(sizeof(node)); //combine into 1 parent
        parent->freq = left->freq + right->freq;
        parent->token = NULL;
        parent->encoding = NULL;
        parent->left = left;
        parent->right = right;
        insertIntoHeap(heap, parent, size); //insert into heap
    }
}

void printHuffman(node* root)
{
    if(root == NULL)
        return;
    
    if(root->encoding != NULL)
    {
        printf("Token: %s, Encoding: %s\n", root->token, root->encoding);
    }

    printHuffman(root->left);    
    printHuffman(root->right);
}

void freeHuffman(node* root)
{
    if(root == NULL)
        return;
    
    freeHuffman(root->left);
    freeHuffman(root->right);
    if(root->token != NULL)
        free(root->token);
    if(root->encoding != NULL)
        free(root->encoding);
    free(root);
}

void encode(node* root, int* huffmanCodeArr, int lengthOfEncoding)
{
    if(root->left != NULL) //if possible to go left
    {
        huffmanCodeArr[lengthOfEncoding] = 0; //set index to 0
        encode(root->left, huffmanCodeArr, lengthOfEncoding + 1); //go left
    }

    if(root->right != NULL)
    {
        huffmanCodeArr[lengthOfEncoding] = 1; //set index of array to 1
        encode(root->right, huffmanCodeArr, lengthOfEncoding + 1); //go right
    }

    if(root->left == NULL && root->right == NULL) //if leaf node is met
    {
        int i;
        int arrayIndex = 0;
        char* encoding = (char*) malloc(sizeof(char) * (arrayIndex + 1));
        encoding[0] = '\0';
        for(i = 0; i < lengthOfEncoding; i++)
        {
            encoding[arrayIndex] = huffmanCodeArr[i] + '0'; //traverse through array and concatinate to string encoding
            arrayIndex++;
            char* tmp = (char*) realloc(encoding, sizeof(char) * (arrayIndex + 1));
            encoding = tmp;
            encoding[arrayIndex] = '\0';
        }

        root->encoding = encoding;
    }
}