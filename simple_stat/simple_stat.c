#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

int errstat(int ret)
{
    if (ret == 0)
    {
        return 0;
    }
    if (errno == EACCES)
    {
        fprintf(stderr, "simple_stat: Permission denied\n");
    }
    else if (errno == EBADF)
    {
        fprintf(stderr,
                "simple_stat: fd is not a valid file descriptor opened for "
                "reading.\n");
    }
    else if (errno == EOVERFLOW)
    {
        fprintf(stderr,
                "simple_stat: OVERFLOW"
                "reached\n");
    }
    else if (errno == ENOENT)
    {
        fprintf(stderr,
                "simple_stat: Pathname does not exist, or name is a gling "
                "symbol link.\n");
    }
    else if (errno == ENOTDIR)
    {
        fprintf(stderr, "simple_stat: name is not a directory.\n");
    }
    else
    {
        fprintf(stderr, "simple_stat: Unknown error check man page\n");
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }
    struct stat buf;
    for (int i = 1; i < argc; i++)
    {
        int ret = stat(argv[i], &buf);
        if (errstat(ret) == 1)
        {
            return 1;
        }
        printf("st_dev=%lu\n", buf.st_dev);
        printf("st_ino=%lu\n", buf.st_ino);
        printf("st_mode=0%o\n", buf.st_mode);
        printf("st_nlink=%lu\n", buf.st_nlink);
        printf("st_uid=%u\n", buf.st_uid);
        printf("st_gid=%u\n", buf.st_gid);
        printf("st_rdev=%lu\n", buf.st_rdev);
        printf("st_size=%ld\n", buf.st_size);
        printf("st_atime=%ld\n", buf.st_atime);
        printf("st_mtime=%ld\n", buf.st_mtime);
        printf("st_ctime=%ld\n", buf.st_ctime);
        printf("st_blksize=%ld\n", buf.st_blksize);
        printf("st_blocks=%ld\n", buf.st_blocks);
    }
    return 0;
}
