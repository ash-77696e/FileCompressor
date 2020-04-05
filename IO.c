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

/**
 * Non blocking write function
 * */
void non_blocking_write(char* buffer, int size, int fd)
{
    int status = 1;
    int bytesWrote = 0;

    do
    {
        status = write(fd, buffer+bytesWrote, size-bytesWrote);
        bytesWrote += status;
    } while (status > 0 && bytesWrote < size);
}

/**
 * Traverse through compressed file, and write to decompressed file the decoded token
 * */
void decompressFile(char* oldPath, char* newPath, node* root)
{
    int compFD = open(oldPath, O_RDONLY);
    char buffer = '\0';
    int status;

    if(compFD == -1)
    {
        printf("Fatal Error: file %s does not exist\n", oldPath);
        return;
    }
    if((status = read(compFD, &buffer, 1) == 0))
    {
        printf("Warning: file %s is empty\n", oldPath);
        return;
    }
    close(compFD);
    compFD = open(oldPath, O_RDONLY);
    if(access(newPath, F_OK) != -1)
    {
        printf("Warning: File %s already exists, will be deleted and/or replaced\n", newPath); //errors
        remove(newPath);
    }
    int newFD = open(newPath, O_RDWR | O_CREAT, 00600);

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
            if(val == 0 && ptr->left != NULL) //keep going left or right depending on value read
                ptr = ptr->left;
            if(val == 1 && ptr->right != NULL)
                ptr = ptr->right;
            
            if(ptr->left == NULL && ptr->right == NULL) //if leaf node is reached then we can get the token
            {
                if(ptr->token[0] == '/')
                {
                    if(ptr->token[1] != '/')
                    {
                        char ascii[2]; //escape character conversion
                        int asciiVal = 0;
                        memcpy(ascii, &(ptr->token[1]), 2);
                        if(ascii[0] == '0')
                            asciiVal = atoi(&ascii[1]);
                        else
                            asciiVal = atoi(ascii);

                        char c = asciiVal;
                        write(newFD, &c, 1); //write
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

/**
 * Create Huffman Tree from Huffman Codebook
 * */
node* buildHuffmanFromFile(char* path, node* root)
{
    int fd = open(path, O_RDONLY);

    if(fd == -1)
    {
        printf("Fatal Error: file %s does not exist\n", path);
        return NULL;
    }

    char buffer = '\0';
    char* str = (char*) malloc(sizeof(char));

    int index = 0;
    int status;

    read(fd, &buffer, 1); //read escape char
    read(fd, &buffer, 1); //read new line
    node* insert;
    str[index] = '\0';

    while((status = read(fd, &buffer, 1)) > 0)
    {
        if(buffer == '\t') //read tab
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1)); //create endcoding token
            strcpy(nodeStr, str);
            insert = (node*) malloc(sizeof(node));
            insert->encoding = nodeStr;
            free(str);
            str = (char*) malloc(sizeof(char));
            index = 0;
            continue;
        }

        if(buffer == '\n') //read new line
        {
            char* nodeStr = (char*) malloc(sizeof(char) * (strlen(str) + 1)); //create string token
            strcpy(nodeStr, str);
            insert->token = nodeStr;
            root = buildHuffmanFromEncoding(root, insert); //insert token into huffman tree
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

/**
 * Write compressed file
 * */
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

    while((status = read(oldFD, &buffer, 1)) > 0) //read through non compressed file
    {
        if(iscntrl(buffer) || buffer == ' ') //if control character or space
        {
            if(strlen(str) > 0)
            {
                char* findStr;
                if(str[0] == '/') //if token starts with /, prepend another /
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
                
                node* found = AVLSearch(root, findStr); //find token and encoding in AVL tree
                write(compFD, found->encoding, strlen(found->encoding));
                free(findStr);
            }

            char* findStr = (char*) malloc(sizeof(char) * 4); //convert control char to ascii value
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

/**
 * Construct AVL from file
 * */
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
        if(iscntrl(buffer) || buffer == ' ') //if control char is read (delimeter)
        {
            
            if(strlen(str) > 0)
            {
                node* insert = (node*) malloc(sizeof(node));
                char* nodeStr;
                if(str[0] == '/') //prepend additional /
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

            node* insert = (node*) malloc(sizeof(node)); //convert control char and insert
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

/**
 * Write Huffman Codebook
 * */
void writeHuffman(int fd, node* root)
{
    if(root == NULL)
        return;

    if(root->encoding != NULL)
    {
        write(fd, root->encoding, strlen(root->encoding)); //write encoding
        write(fd, "\t", 1);
        write(fd, root->token, strlen(root->token)); ///write token
        write(fd, "\n", 1);
    }

    writeHuffman(fd, root->left); //pre order traversal
    writeHuffman(fd, root->right);
}

void writeHuffmanCodebook(int fd, node* root)
{
    char* escape = "/\n\0";
    write(fd, escape, strlen(escape));
    writeHuffman(fd, root);
}

/**
 * Create AVL from Huffman Codebook
 * */
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