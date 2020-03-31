#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "structs.h"
#include "AVL.h"

node* buildAVLFromFile(const char* path, node* root)
{
    int fd = open(path, O_RDONLY);
    int status;

    char buffer = '.';
    char* str = (char*) malloc(sizeof(char));
    
    int index = 0;
    str[0] = '\0';

    while((status = read(fd, &buffer, 1)) > 0)
    {
        if(buffer == '\n' || buffer == '\t' || buffer == ' ')
        {
            
            if(strlen(str) > 0)
            {
                node* insert = (node*) malloc(sizeof(node));
                char* nodeStr;
                if(str[0] == '/')
                {
                    nodeStr = (char*) malloc(sizeof(char) * strlen(str) + 2);
                    nodeStr[0] = '/';
                    strcpy(&nodeStr[1], str);
                }
                else
                {
                    nodeStr = (char*) malloc(sizeof(char) * strlen(str) + 1);
                    strcpy(nodeStr, str);
                }

                insert->token = nodeStr;
                root = add(root, insert); 
            }

            node* insert = (node*) malloc(sizeof(node));
            char* nodeStr = (char*) (malloc(sizeof(char) * 3));
            nodeStr[0] = '/';
            nodeStr[2] = '\0';

            if(buffer == ' ')
                nodeStr[1] = 's';
            else if(buffer == '\n')
                nodeStr[1] = 'n';
            else
                nodeStr[1] = 't';
            
            insert->token = nodeStr;
            root = add(root, insert);

            free(str);
            str = (char*) malloc(sizeof(char));
            index = 0;
            continue;
        }

        str[index++] = buffer;
        char* tmp = (char*) realloc(str, sizeof(char)*(index+1));
        str = tmp;
        str[index] = '\0';
    }

    if(strlen(str) > 0)
    {
        node* insert = (node*) malloc(sizeof(node));
        char* nodeStr;
        if(str[0] == '/')
        {
            nodeStr = (char*) malloc(sizeof(char) * strlen(str) + 2);
            nodeStr[0] = '/';
            strcpy(&nodeStr[1], str);
        }
        else
        {
            nodeStr = (char*) malloc(sizeof(char) * strlen(str) + 1);
            strcpy(nodeStr, str);
        }

        insert->token = nodeStr;
        root = add(root, insert); 
    }

    free(str);

    return root;
}

void writeHuffman(int fd, node* root)
{
    if(root == NULL)
        return;
    char* tab = "\t";
    char* newLine = "\n";

    if(root->encoding != NULL)
    {
        write(fd, root->encoding, strlen(root->encoding));
        write(fd, tab, 1);
        write(fd, root->token, strlen(root->token));
        write(fd, newLine, 1);
    }

    writeHuffman(fd, root->left);
    writeHuffman(fd, root->right);
}

void writeHuffmanCodebook(int fd, node* root)
{
    char* escape = "/\n\0";
    write(fd, escape, strlen(escape));
    writeHuffman(fd, root);
}