#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* add(node* root, node* strNode){
    if(root == NULL){ // tree is empty
        strNode->freq = 1;
        strNode->height = -1;
        strNode->left = NULL;
        strNode->right = NULL;
        return strNode;
    }

    if(strcmp(root->token, strNode->token) == 0){ // already exists in the tree so increment frequency
        root->freq++;
        return root; // to traverse back up the tree recursively to the original root and exit the function
    }

    if(strcmp(root->token, strNode->token) < 0){ // traverse through right subtree
        root->right = add(root->right, strNode);

        if(height(root->right) - height(root->left) == 2){ // inserting in right subtree means if the tree is unbalanced it's from the right
            //rotate
        }
    }

    if(strcmp(root->token, strNode->token) > 0){ // traverse through left subtree
        root->left = add(root->left, strNode);

        if(height(root->left) - height(root->right) == 2){ // inserting in left subtree means if the tree is unbalanced it's from the left
            //rotate
        }

    }

    root->height = height(root->left); // -1 if left child is empty
    if(height(root->right) > root->height){
        root->height = height(root->right);
    }
    root->height++; // height of a node is one greater than its biggest child
    return root; // to traverse back up the tree recursively to the original root and exit the function
}


int height(node* root){ // allows checking height of empty nodes as well
    if(root == NULL){
        return -1; // height of empty node is -1
    }
    else{
        return root->height;
    }
}

void printTree(node* root)
{
    if(root == NULL)
        return;
    printTree(root->left);
    printf("Token: %s, Freq: %d\n", root->token, root->freq);
    printTree(root->right);
}

void freeTree(node* root)
{
    if(root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->token);
    free(root);
}

int getSize(node* root)
{
    if(root == NULL)
        return 0;
    else
        return (1 + getSize(root->left) + getSize(root->right));
}
