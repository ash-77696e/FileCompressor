#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "minheap.h"
#include "structs.h"

void buildHuffmanTree(node** heap, int* size)
{
    if(*size == 1)
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
        node* left = extractMin(heap, size);
        node* right = extractMin(heap, size);

        node* parent = (node*) malloc(sizeof(node));
        parent->freq = left->freq + right->freq;
        parent->token = NULL;
        parent->encoding = NULL;
        parent->left = left;
        parent->right = right;
        insertIntoHeap(heap, parent, size);
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
    if(root->left != NULL)
    {
        huffmanCodeArr[lengthOfEncoding] = 0;
        encode(root->left, huffmanCodeArr, lengthOfEncoding + 1);
    }

    if(root->right != NULL)
    {
        huffmanCodeArr[lengthOfEncoding] = 1;
        encode(root->right, huffmanCodeArr, lengthOfEncoding + 1);
    }

    if(root->left == NULL && root->right == NULL)
    {
        int i;
        int arrayIndex = 0;
        char* encoding = (char*) malloc(sizeof(char) * (arrayIndex + 1));
        encoding[0] = '\0';
        for(i = 0; i < lengthOfEncoding; i++)
        {
            encoding[arrayIndex] = huffmanCodeArr[i] + '0';
            arrayIndex++;
            char* tmp = (char*) realloc(encoding, sizeof(char) * (arrayIndex + 1));
            encoding = tmp;
            encoding[arrayIndex] = '\0';
        }

        root->encoding = encoding;
    }
}