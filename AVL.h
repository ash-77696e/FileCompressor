#ifndef AVL_H
#define AVL_H

node* add(node* root, node* strNode);

int height(node* root);

void printTree(node* root);

void freeTree(node* root);

#endif