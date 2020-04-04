#include "fileCompressor.h"

int main(int argc, char* argv[])
{
    //if build is selected
    
    /*node* root = NULL;
    char* huffmanPath = createHuffmanPath(argv[2]);
    root = buildAVLFromFile(argv[2], root);
    build(root, huffmanPath);*/
    

    //if recursive build is selected
    /*node* root = NULL;
    recursiveBuild(root, "./test");*/

    //if compress is selected
    /*node* root = NULL;
    compress(root, "./test/HuffmanCodebook", "./test/hi");*/

    //if recursive compress is selected
    /*node* root;
    root = buildAVLFromHuffman("./test/HuffmanCodebook", root);
    if(root == NULL)
    {
        printf("Error: file can't be compressed\n");
        return;
    }
    recursiveCompress("./test", "./test/HuffmanCodebook", root);
    freeTree(root);*/

    //if decompress
    /*node* root;
    root = (node*) malloc(sizeof(node));
    root->encoding = NULL;
    root->token = NULL;
    root = buildHuffmanFromFile("./test/HuffmanCodebook", root);
    if(root == NULL)
    {
        printf("Error: file can't be decompressed\n");
        return;
    }
    decompress(root, "./test/HuffmanCodebook", "./test/niggatest/hello.pdf.hcz");
    free(root);*/

    //if recursive decompress
    /*node* root;
    root = (node*) malloc(sizeof(node));
    root->encoding = NULL;
    root->token = NULL;
    root = buildHuffmanFromFile("./test/HuffmanCodebook", root);
    if(root == NULL)
    {
        printf("Error: file can't be decompressed\n");
        return;
    }
    recursiveDecompress(root, "./test/HuffmanCodebook", "./test");
    free(root);*/


    return 0;
}

/**
 * Recursive decompress function
 * */
void recursiveDecompress(node* root, char* codebookPath, char* dirPath)
{
    DIR* dir = opendir(dirPath);

    if(dir == NULL)
    {
        printf("Fatal Error: directory: %s does not exist\n", dirPath);
        return;
    }

    readdir(dir);
    readdir(dir);

    char path[10000];
    memset(path, '\0', 10000);
    struct dirent* entry;

    while((entry = readdir(dir)) != NULL)
    {
        strcpy(path, dirPath);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if(isDirectory(path))
            recursiveDecompress(root, codebookPath, path);
        else if(strcmp("hcz", getFileExtension(path)) == 0)
        {
            decompress(root, codebookPath, path);       
        }
    }

    closedir(dir);
}

/**
 * Decompress function
 * Creates Huffman Tree from codebook and walks through compressed file
 * Converts compressed file to decompressed form
 * */
void decompress(node* root, char* codebookPath, char* oldFilePath)
{
    char* newFilePath = getDecompFileName(oldFilePath);
    decompressFile(oldFilePath, newFilePath, root);
    free(newFilePath);
}

/**
 * Recursive build function
 * */
void recursiveBuild(node* root, char* path)
{
    char* huffmanPath = (char*) malloc(sizeof(char) * (strlen(path) + 2));
    memset(huffmanPath, '\0', strlen(path)+2);
    strcpy(huffmanPath, path);
    strcat(huffmanPath, "/");
    huffmanPath = createHuffmanPath(huffmanPath);
    if(access(huffmanPath, F_OK) != -1)
    {
        printf("Warning: Codebook already exists in directory, will be deleted and/or replaced\n");
        remove(huffmanPath);
    }
    root = buildAVLRecursive(path, root);
    build(root, huffmanPath);
}

/**
 * Base build function
 * */
void build(node* root, char* huffmanPath)
{
    int heapSize = getSize(root);
    if(heapSize == 0)
    {
        printf("Fatal Error: no valid directory/file(s) to build from\n");
        free(huffmanPath);
        return;
    }
    if(access(huffmanPath, F_OK) != -1)
    {
        printf("Warning: Codebook already exists in directory, will be deleted and/or replaced\n");
        remove(huffmanPath);
    }
    node** heap = (node**) malloc(sizeof(node*) * heapSize);
    createHeap(heap, root, 0);
    buildHeap(heap, heapSize);
    buildHuffmanTree(heap, &heapSize);
    int huffmanSize = getSize(heap[0]);
    int* huffmanCodeArr = (int*) malloc(sizeof(int) * huffmanSize);
    int lengthOfEncoding = 0;
    encode(heap[0], huffmanCodeArr, lengthOfEncoding);
    int codebookFD = open(huffmanPath, O_RDWR | O_CREAT, 00600);
    writeHuffmanCodebook(codebookFD, heap[0]);
    close(codebookFD);
    freeHuffman(heap[0]);
    free(heap);
    free(huffmanCodeArr);
    free(huffmanPath);
}

/**
 * Go through each directory/file and send to compressFile function of IO.c
 * */
void recursiveCompress(char* basePath, char* huffmanPath, node* root)
{
    DIR* dir = opendir(basePath);

    if(dir == NULL)
    {
        printf("Fatal Error: directory: %s does not exist\n", basePath);
        return;
    }

    readdir(dir);
    readdir(dir);

    char path[10000];
    memset(path, '\0', 10000);
    struct dirent* entry;

    while((entry = readdir(dir)) != NULL)
    {
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if(isDirectory(path))
            recursiveCompress(path, huffmanPath, root);
        else if((strcmp(path, huffmanPath)) != 0 && (strcmp("hcz", getFileExtension(path))) != 0)
        {
            char* compFile = getCompressedFileName(path);
            compressFile(path, compFile, root);   
            free(compFile);       
        }
    }

    closedir(dir);
}

/**
 * Create AVL tree based off Huffman Tree, and send file to be compressed to IO.c function
 * */
void compress(node* root, char* huffmanPath, char* oldFile)
{
    root = buildAVLFromHuffman(huffmanPath, root);
    if(root == NULL)
    {
        printf("Error: file can't be compressed\n");
        return;
    }
    char* compFile = getCompressedFileName(oldFile);
    compressFile(oldFile, compFile, root);
    freeTree(root);
    free(compFile);
}

/**
 * Check if path is a directory
 * */
int isDirectory(char* path)
{
    int fd = open(path, O_RDWR);

    if(fd == -1 && errno == EISDIR)
        return 1;

    close(fd);
    return 0;
}

/**
 * Recurse through all files/directories and construct one AVL tree
 * */
node* buildAVLRecursive(char* basePath, node* root)
{
    DIR* dir = opendir(basePath); 
    if(dir == NULL)
    {
        printf("Fatal Error: directory can't be accessed\n");
        return root;
    }
    // 2 reads to skip past parent and current directory
    readdir(dir);
    readdir(dir);

    char path[10000];

    struct dirent* entry;

    while((entry = readdir(dir)) != NULL)
    { // still an entry to be read from the directory
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if(isDirectory(path))
            root = buildAVLRecursive(path, root);
        else if(strcmp("hcz", getFileExtension(path)) != 0)
            root = buildAVLFromFile(path, root);   
    }

    closedir(dir);

    return root;
}

/**
 * Get file extension
 * */
char* getFileExtension(char* fileName)
{
    char* ext = strrchr(fileName, '.');
    if(ext == NULL)
        return "";
    return ext+1;
}

/**
 * Create Huffman path
 * Adds HuffmanCodebook to end of path
 * */
char* createHuffmanPath(char* path)
{
    int index = strlen(path);
    while(index != 0 && path[index] != '/')
    {
        index--;
    }
    char* huffmanPath = (char*) malloc(sizeof(char) * (index+17));
    memset(huffmanPath, '\0', index+12);
    memcpy(huffmanPath, path, index+1);
    strcat(huffmanPath, "HuffmanCodebook");

    return huffmanPath;
}

/**
 * Create compressed path
 * Adds .hcz to end of path
 * */
char* getCompressedFileName(char* path)
{
    char* compFileName = (char*) malloc(sizeof(char) * (strlen(path) + 5));
    memset(compFileName, '\0', strlen(path) + 5);
    strcpy(compFileName, path);
    strcat(compFileName, ".hcz");
    return compFileName;
}

char* getDecompFileName(char* path)
{
    int index = strlen(path)-1;
    while(index != 0 && path[index] != '.')
    {
        index--;
    }
    char* decompFile = (char*) malloc(sizeof(char) * index+1);
    memset(decompFile, '\0', index+1);
    memcpy(decompFile, path, index);
    return decompFile;
}