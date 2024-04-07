#include "simple.h"
int isDirectory(const char *path)
{
    struct stat statbuf;
    if (errstat(stat(path, &statbuf) == 1))
        return -1;
    return S_ISDIR(statbuf.st_mode);
}

int simple_find_rec(char *dirname)
{
    DIR *dir = opendir(dirname);
    if (opendirerr(dir) == 1)
    {
        return 1;
    }
    printf("%s\n", dirname);
    struct dirent *entry = readdir(dir);
    int ret = 0;
    for (; entry; entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char *concat;
            int asret;
            if (dirname[strlen(dirname) - 1] == '/')
            {
                asret = asprint(&concat, dirname, entry->d_name, "");
                // asret = asprintf(&concat,"%s%s",dirname,entry->d_name);
            }
            else
            {
                asret = asprint(&concat, dirname, entry->d_name, "/");
                // asret = asprintf(&concat,"%s/%s",dirname,entry->d_name);
            }

            if (asret == -1)
            {
                fprintf(stderr, "my_find: Error in concat\n");
                exit(1);
            }

            if (isDirectory(entry->d_name) == -1)
            {
                fprintf(stderr,
                        "my_find: simple_find_rec: error when openninf dir\n");
                ret = 1;
            }
            else
            {
                if (isDirectory(concat))
                {
                    ret = ret | simple_find_rec(concat);
                }
                else
                {
                    printf("%s\n", concat);
                }
            }
            free(concat);
        }
    }
    closedir(dir);
    if (readerr(entry) == 1)
    {
        return 1;
    }
    return ret;
}
int simple_find(int size, char **arr)
{
    if (size == 0)
    {
        char *dirname = ".";
        return simple_find_rec(dirname);
    }
    int ret = 0;
    for (int i = 0; i < size; i++)
    {
        ret = ret | simple_find_rec(arr[0]);
    }
    return ret;
}
