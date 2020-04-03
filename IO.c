#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "structs.h"
#include "huffman.h"
#include "AVL.h"

node* buildHuffmanFromFile(int fd, node* root)
{
    char buffer = '\0';
    char* str = (char*) malloc(sizeof(char));

    int index = 0;
    int status;

    read(fd, &buffer, 1);
    read(fd, &buffer, 1);
    node* insert;
    str[index] = '\0';

    while((status = read(fd, &buffer, 1)) > 0)
    {
        if(buffer == '\t')
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
            strcpy(nodeStr, str);
            insert = (node*) malloc(sizeof(node));
            insert->encoding = nodeStr;
            free(str);
            str = (char*) malloc(sizeof(char));
            index = 0;
            continue;
        }

        if(buffer == '\n')
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
            strcpy(nodeStr, str);
            insert->token = nodeStr;
            root = buildHuffmanFromEncoding(root, insert);
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

    free(str);

    return root;
}

void compressFile(int oldFD, int compFD, node* root)
{
    int status;
    char buffer = '\0';

    char* str = (char*) malloc(sizeof(char));

    int index = 0;
    str[0] = '\0';

    while((status = read(oldFD, &buffer, 1)) > 0)
    {
        if(iscntrl(buffer) || buffer == ' ')
        {
            if(strlen(str) > 0)
            {
                char* findStr;
                if(str[0] == '/')
                {
                    findStr = (char*) malloc(sizeof(char) * (strlen(str) + 2));
                    findStr[0] = '/';
                    strcpy(&findStr[1], str);
                }
                else
                {
                    findStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
                    strcpy(findStr, str);
                }
                
                node* found = AVLSearch(root, findStr);
                write(compFD, found->encoding, strlen(found->encoding));
                free(findStr);
            }

            char* findStr = (char*) malloc(sizeof(char) * 4);
            findStr[0] = '/';
            findStr[3] = '\0';
            int asciiValue = (int) buffer;
            findStr[1] = (asciiValue / 10) + '0';
            findStr[2] = (asciiValue % 10) + '0';

            node* found = AVLSearch(root, findStr);
            write(compFD, found->encoding, strlen(found->encoding));
            free(findStr);

            free(str);
            str = (char*) malloc(sizeof(char));
            str[0] = '\0';
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
        char* findStr;
        if(str[0] == '/')
        {
            findStr = (char*) malloc(sizeof(char) * (strlen(str) + 2));
            findStr[0] = '/';
            strcpy(&findStr[1], str);
        }
        else
        {
            findStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
            strcpy(findStr, str);
        }
                
        node* found = AVLSearch(root, findStr);
        write(compFD, found->encoding, strlen(found->encoding));
        free(findStr);
    }

    free(str);
}

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
        if(iscntrl(buffer) || buffer == ' ')
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
            char* nodeStr = (char*) (malloc(sizeof(char) * 4));
            nodeStr[0] = '/';
            nodeStr[3] = '\0';
            
            int asciiVal = (int) buffer;
            nodeStr[1] = (asciiVal / 10) + '0';
            nodeStr[2] = (asciiVal % 10) + '0';

            insert->token = nodeStr;
            root = add(root, insert);

            free(str);
            str = (char*) malloc(sizeof(char));
            str[0] = '\0';
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

node* buildAVLFromHuffman(int fd, node* root)
{
    char buffer = '\0';
    char* str = (char*) malloc(sizeof(char));

    int index = 0;
    int status;

    read(fd, &buffer, 1);
    read(fd, &buffer, 1);
    node* insert;
    str[index] = '\0';

    while((status = read(fd, &buffer, 1)) > 0)
    {
        if(buffer == '\t')
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
            strcpy(nodeStr, str);
            insert = (node*) malloc(sizeof(node));
            insert->encoding = nodeStr;
            free(str);
            str = (char*) malloc(sizeof(char));
            index = 0;
            continue;
        }

        if(buffer == '\n')
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1));
            strcpy(nodeStr, str);
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

    free(str);

    return root;
}

