#include "fileCompressor.h"

int main(int argc, char* argv[])
{
    node* root = NULL;
    root = buildAVLFromFile("./test/test1/gg", root);
    int heapSize = getSize(root);
    node** heap = (node**) malloc(sizeof(node*) * heapSize);
    createHeap(heap, root, 0);
    buildHeap(heap, heapSize);
    buildHuffmanTree(heap, &heapSize);
    int huffmanSize = getSize(heap[0]);
    int* huffmanCodeArr = (int*) malloc(sizeof(int) * huffmanSize);
    int lengthOfEncoding = 0;
    encode(heap[0], huffmanCodeArr, lengthOfEncoding);
    printHuffman(heap[0]);
    freeHuffman(heap[0]);
    free(heap);
    free(huffmanCodeArr);

    return 0;
}

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