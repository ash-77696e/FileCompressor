#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "structs.h"
#include "huffman.h"
#include "AVL.h"

void decompressFile(char* oldPath, char* newPath, node* root)
{
    int compFD = open(oldPath, O_RDONLY);
    if(compFD == -1)
    {
        printf("Fatal Error: file %s does not exist\n", oldPath);
        return;
    }
    if(access(newPath, F_OK) != -1)
    {
        printf("Warning: File %s already exists, will be deleted and/or replaced\n", newPath);
        remove(newPath);
    }
    int newFD = open(newPath, O_RDWR | O_CREAT, 00600);
    char buffer = '\0';
    int status;

    bool traversing = false;
    node* ptr;

    while((status = read(compFD, &buffer, 1)) > 0)
    {
        if(!traversing)
        {
            traversing = true;
            ptr = root;
        }

        int val = (int) (buffer - '0');

        if(traversing)
        {
            if(val == 0 && ptr->left != NULL)
                ptr = ptr->left;
            if(val == 1 && ptr->right != NULL)
                ptr = ptr->right;
            
            if(ptr->left == NULL && ptr->right == NULL)
            {
                if(ptr->token[0] == '/')
                {
                    if(ptr->token[1] != '/')
                    {
                        char ascii[2];
                        int asciiVal = 0;
                        memcpy(ascii, &(ptr->token[1]), 2);
                        if(ascii[0] == '0')
                            asciiVal = atoi(&ascii[1]);
                        else
                            asciiVal = atoi(ascii);

                        char c = asciiVal;
                        write(newFD, &c, 1);
                        traversing = false;
                    }
                    else
                    {
                        write(newFD, &(ptr->token[1]), strlen(&(ptr->token[1])));
                        traversing = false;
                    }
                    
                }
                else
                {  
                    write(newFD, ptr->token, strlen(ptr->token));
                    traversing = false;
                }
            }
        }
    }

    close(compFD);
}

node* buildHuffmanFromFile(char* path, node* root)
{
    int fd = open(path, O_RDONLY);

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

void compressFile(char* oldFilePath, char* newFilePath, node* root)
{
    int oldFD = open(oldFilePath, O_RDONLY);
    int status;
    char buffer = '\0';

    if(oldFD == -1)
    {
        printf("Fatal Error: %s does not exist\n", oldFilePath);
        return;
    }

    if((status = read(oldFD, &buffer, 1)) == 0)
    {
        printf("Fatal Error: File: %s is empty, can't be compressed\n", oldFilePath);
        close(oldFD);
        return;
    }

    close(oldFD);
    oldFD = open(oldFilePath, O_RDONLY);
    
    if(access(newFilePath, F_OK) != -1)
    {
        printf("Warning: File %s already exists, will be deleted and/or replaced\n", newFilePath);
        remove(newFilePath);
    }
    
    int compFD = open(newFilePath, O_RDWR | O_CREAT, 00600);

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

    close(oldFD);
    close(compFD);
    free(str);
}

node* buildAVLFromFile(const char* path, node* root)
{
    int fd = open(path, O_RDONLY);
    int status;
    char buffer = '.';

    if(fd == -1)
    {
        printf("Error: File: %s does not exist\n", path);
        return root;
    }

    if((status = read(fd, &buffer, 1)) == 0)
    {
        printf("Error: File: %s is empty\n", path);
        return root;
    }
    
    close(fd);
    fd = open(path, O_RDONLY);

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

    close(fd);
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

node* buildAVLFromHuffman(char* path, node* root)
{
    int fd = open(path, O_RDONLY);
    int status;

    char buffer = '\0';
    char* str = (char*) malloc(sizeof(char));

    if(fd == -1)
    {
        printf("Fatal Error: Huffman Codebook: %s does not exist\n", path);
        return root;
    }

    status = read(fd, &buffer, 1);

    if(status == 0)
    {
        printf("Fatal Error: Codebook does not exist\n");
        return root;
    }

    if(buffer != '/')
    {
        printf("Fatal Error: Invalid Huffman Codebook provided. Please build using accompaning funcion\n");
        return root;
    }

    close(fd);
    fd = open(path, O_RDONLY);

    int index = 0;

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
    close(fd);

    return root;
}