#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int isDirectory(const char* path)
{
    return (open(path, O_RDWR) == -1 && errno == EISDIR);
}

void printAllFiles(char* basePath, int fd)
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
            printAllFiles(path, fd);
        else
        {
            char buffer[1000];
            strcpy(buffer, entry->d_name);
            strcat(buffer, "\n");
            write(fd, buffer, strlen(buffer));        
        }
    }
    
    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(access("./fileCompressor.test", F_OK) != -1)
        remove("./fileCompressor.test");
    
    int fd = open("./fileCompressor.test", O_RDWR | O_CREAT, 00600);
    printAllFiles("./test", fd);

    return 0;
}