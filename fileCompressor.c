#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int isDirectory(const char* path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) != 0)
        return 0;
    
    return S_ISDIR(statbuf.st_mode);
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

        if(!strcmp(".git", entry->d_name)) { } //.git is there cuz of github
        else if(isDirectory(path))
            printAllFiles(path);
        else
        {
            printf("%s\n", entry->d_name);
        }
    }
    
    closedir(dir);
}

int main(int argc, char* argv[])
{
    printAllFiles("./");
    return 0;
}