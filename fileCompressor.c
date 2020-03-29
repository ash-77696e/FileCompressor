#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

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
        else
            readFile(path);
    }
    
    closedir(dir);
}

int readFile(const char* path)
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
                if(str[0] == '/')
                printf("/");
                printf("%s", str);
            }

            if(buffer == '\n')
                printf("/n");
            else if(buffer == '\t')
                printf("/t");
            else if(buffer == ' ')
                printf("/s");
            
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
        if(str[0] == '/')
            printf("/");
        printf("%s", str);
    }

    printf("\n");

    free(str);

    return 0;
}

int main(int argc, char* argv[])
{
    printAllFiles("./test");
    return 0;
}