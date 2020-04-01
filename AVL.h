#ifndef AVL_H
#define AVL_H

node* add(node* root, node* strNode);

int height(node* root);

void printTree(node* root);

void freeTree(node* root);

int getSize(node* root);

int maxchild(node* root);

node* leftleft(node* p, node* c);

node* rightright(node* p, node* c);

node* leftright(node* p, node* c, node* g);

node* rightleft(node* p, node* c, node* g);

#endif