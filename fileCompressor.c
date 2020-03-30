#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "structs.h"
#include "AVL.h"
#include "heap.h"
#include "IO.h"

int isDirectory(const char* path)
{
    int fd = open(path, O_RDWR);

    if(fd == -1 && errno == EISDIR)
    {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

void printAllFiles(char* basePath)
{
    DIR* dir = opendir(basePath);
    readdir(dir);
    readdir(dir);
    char path[10000];

    struct dirent* entry;

    while((entry = readdir(dir)) != NULL)
    {
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if(isDirectory(path))
            printAllFiles(path);
        //else
            //buildReadFile(path);
    }
    
    closedir(dir);
}

int main(int argc, char* argv[])
{
    node* root = NULL;
    root = buildAVLFromFile("./test/test1/gg", root);
    //printTree(root);
    int size = getSize(root);
    node** heap = (node**) malloc(sizeof(node*) * size);
    createHeap(heap, root, 0);
    buildHeap(heap, size);

    int i = 0;
    for(i = 0; i < size; i++)
        printf("%d\n", heap[i]->freq);
    
    free(heap);
    freeTree(root);

    return 0;
}