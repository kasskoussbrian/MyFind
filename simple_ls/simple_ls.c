#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
int errdir(DIR *dir)
{
    if (dir == NULL)
    {
        if (errno == EACCES)
        {
            fprintf(stderr, "simple_ls: Permission denied\n");
        }
        else if (errno == EBADF)
        {
            fprintf(stderr,
                    "simple_ls: fd is not a valid file descriptor opened for "
                    "reading.\n");
        }
        else if (errno == EMFILE)
        {
            fprintf(stderr,
                    "simple_ls: total number of open file descriptors has been "
                    "reached\n");
        }
        else if (errno == ENFILE)
        {
            fprintf(
                stderr,
                "simple_ls: total number of open files has been reached.\n");
        }
        else if (errno == ENOENT)
        {
            fprintf(stderr,
                    "simple_ls: Directory does not exist, or name is an empty "
                    "string.\n");
        }
        else if (errno == ENOTDIR)
        {
            fprintf(stderr, "simple_ls: name is not a directory.\n");
        }
        else if (errno == ENOMEM)
        {
            fprintf(stderr,
                    "simple_ls: Insufficient memory to complete operation\n");
        }
        else
        {
            fprintf(stderr, "simple_ls: Unknown error\n");
        }
        return 1;
    }
    return 0;
}
int errdirent(struct dirent *entry)
{
    if (entry == NULL)
    {
        if (errno == EBADF)
        {
            fprintf(stderr, "simple_ls: Unknown error\n");
            return 1;
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "simple_ls: Wrong number of argument\n");
        return 1;
    }
    char *dirname = argv[1];
    if (argc != 2)
    {
        dirname = ".";
    }
    DIR *dir = opendir(dirname);
    if (errdir(dir) == 1)
    {
        return 1;
    }
    struct dirent *entry = readdir(dir);
    for (; entry; entry = readdir(dir))
    {
        if (entry->d_name[0] != '.')
        {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
    if (errdirent(entry) == 1)
    {
        return 1;
    }
    return 0;
}
