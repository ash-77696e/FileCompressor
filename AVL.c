#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"

node* add(node* root, node* strNode){
    if(root == NULL){ // tree is empty
        strNode->freq = 1;
        strNode->height = -1;
        strNode->left = NULL;
        strNode->right = NULL;
        return strNode;
    }

    else if(strcmp(root->token, strNode->token) == 0){ // already exists in the tree so increment frequency
        //free(strNode->token); for some reason, these 2 lines cause a segfault sometimes
        //free(strNode);
        root->freq++;
        return root; // to traverse back up the tree recursively to the original root and exit the function
    }

    else if(strcmp(root->token, strNode->token) < 0){ // traverse through right subtree
        root->right = add(root->right, strNode);

        if(height(root->right) - height(root->left) == 2){ // inserting in right subtree means if the tree is unbalanced it's from the right
            if(strcmp(strNode->token, root->right->token) > 0){ // right right case
                node* p = root;
                node* c = root->right;

                root = rightright(p, c);
            }
            else{ // right left case
                node* p = root; 
                node* c = root->right;
                node* g = root->right->left;

                root = rightleft(p, c, g);
            }
        }
    }

    else if(strcmp(root->token, strNode->token) > 0){ // traverse through left subtree
        root->left = add(root->left, strNode);

        if(height(root->left) - height(root->right) == 2){ // inserting in left subtree means if the tree is unbalanced it's from the left
            if(strcmp(strNode->token, root->left->token) < 0){ // left left case
                node* p = root;
                node* c = root->left;

                root = leftleft(p, c);
            }
            else{ // left right case
                node* p = root;
                node* c = root->left;
                node* g = root->left->right;

                root = leftright(p, c, g);
            }
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
    printf("Token: %s, Encoding: %s\n", root->token, root->encoding);
    printTree(root->right);
}

void freeTree(node* root)
{
    if(root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->token);
    free(root->encoding);
    free(root);
}

int getSize(node* root)
{
    if(root == NULL)
        return 0;
    else
        return (1 + getSize(root->left) + getSize(root->right));
}

int maxchild(node* root){
    int result = -1;
    if(root->left != NULL){
        result = root->left->height;
    }
    if(root->right != NULL){
        if(root->right->height > result){
            result = root->right->height;
        }
    }
    return result;
}

node* leftleft(node* p, node* c){
    node* temp = c->right;
    c->right = p;
    p->left = temp;
    p->height = maxchild(p) + 1;
    c->height = maxchild(c) + 1;

    return c;
}

node* rightright(node* p, node* c){
    node* temp = c->left;
    c->left = p;
    p->right = temp;
    p->height = maxchild(p) + 1;
    c->height = maxchild(c) + 1;

    return c;
}

node* leftright(node* p, node* c, node* g){
    // start with right right rotation
    node* temp = g->left;
    g->left = c;
    c->right = temp;
    c->height = maxchild(c) + 1;
    g->height = maxchild(g) + 1;

    // do left left rotation 
    temp = g->right;
    g->right = p;
    p->left = temp;
    p->height = maxchild(p) + 1;
    g->height = maxchild(g) + 1;

    return g;
}

node* rightleft(node* p, node* c, node* g){
    //start with left left rotation
    node* temp = g->right;
    g->right = c;
    c->left = temp;
    c->height = maxchild(c) + 1;
    g->height = maxchild(g) + 1;

    //do right right rotation
    temp = g->left;
    g->left = p;
    p->right = temp;
    p->height = maxchild(p) + 1;
    g->height = maxchild(g) + 1;

    return g;
}

node* AVLSearch(node* root, char* str){ 
    if(root == NULL || strcmp(root->token, str) == 0){ // either end of tree or match
        return root;
    }
    
    else if(strcmp(str, root->token) > 0){ // str is on right side of the AVL
        return AVLSearch(root->right, str);
    }

    else{
        return AVLSearch(root->left, str); // str is on left side of the AVL
    }


}
