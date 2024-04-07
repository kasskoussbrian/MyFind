#include "simple.h"
int opendirerr(DIR *dir)
{
    if (dir == NULL)
    {
        if (errno == EACCES)
        {
            fprintf(stderr, "myfind: Permission denied\n");
        }
        else if (errno == EBADF)
        {
            fprintf(stderr,
                    "myfind: fd is not a valid file descriptor opened for "
                    "reading.\n");
        }
        else if (errno == EMFILE)
        {
            fprintf(stderr,
                    "myfind: total number of open file descriptors has been "
                    "reached\n");
        }
        else if (errno == ENFILE)
        {
            fprintf(stderr,
                    "myfind: total number of open files has been reached.\n");
        }
        else if (errno == ENOENT)
        {
            fprintf(stderr,
                    "myfind: Directory does not exist, or name is an empty "
                    "string.\n");
        }
        else if (errno == ENOTDIR)
        {
            fprintf(stderr, "myfind: name is not a directory.\n");
        }
        else if (errno == ENOMEM)
        {
            fprintf(stderr,
                    "myfind: Insufficient memory to complete operation\n");
        }
        else
        {
            fprintf(stderr, "myfind: Unknown error\n");
        }
        return 1;
    }
    return 0;
}
int readerr(struct dirent *entry)
{
    if (entry == NULL)
    {
        if (errno == EBADF)
        {
            fprintf(stderr, "myfind: Unknown error\n");
            return 1;
        }
    }
    return 0;
}

int errstat(int ret)
{
    if (ret == 0)
    {
        return 0;
    }
    if (errno == EACCES)
    {
        fprintf(stderr, "my_find: Permission denied\n");
    }
    else if (errno == EBADF)
    {
        fprintf(stderr,
                "myfind: fd is not a valid file descriptor opened for "
                "reading.\n");
    }
    else if (errno == EOVERFLOW)
    {
        fprintf(stderr,
                "myfind: OVERFLOW"
                "reached\n");
    }
    else if (errno == ENOENT)
    {
        fprintf(stderr,
                "myfind: Pathname does not exist, or name is a gling "
                "symbol link.\n");
    }
    else if (errno == ENOTDIR)
    {
        fprintf(stderr, "myfind: name is not a directory.\n");
    }
    else
    {
        fprintf(stderr, "myfind: Unknown error check man page\n");
    }
    return 1;
}
