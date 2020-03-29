#include "structs.h"

node* add(node* root, node* strNode){
    if( root == NULL){ // tree is empty
        return strNode;
    }

    if(strcmp(root->token, strNode->token) == 0){ // already exists in the tree so increment frequency
        root->freq++;
        return root;
    }

    if(strcmp(root->token, strNode->token) > 0){ // traverse through right subtree
        root->right = add(root->right, strNode);
    }

    if(strcmp(root->token, strNode->token) < 0){ // traverse through left subtree
        root->left = add(root->left, strNode);
    }

    return root; // to traverse back up the tree recursively to the original root and exit the function
}

void heights(node* root){
    if (root == NULL){ 
        return;
    }

    heights(root->left); // recursively assigns heights for left subtree
    heights(root->right); // recursively assigns height for right subtree

    root->height = -1; // height of empty node is -1

    if(root->left != NULL){ // node has a left child so set its height temporarily equal to this
        root->height = root->left->height;
    }

    if(root->right != NULL){
        if(root->right->height > root->height){ // if the right child has a height greater than the current height 
            root->height = root->right->height; //reassign height;
        }
    }

    root->height++; // height of a node is its largest child's height plus one (zero if a node has no children)
}

