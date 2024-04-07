#include "expression.h"

char **Give_Starting(int start, int argc, char *argv[], int *fsize)
{
    // init double array
    char **toret = calloc(1, sizeof(char *));
    if (toret == NULL)
    {
        fprintf(stderr, "Myfind: Give_starting malloc char** failed\n");
    }
    int size = 1;

    int c = 0; // number of starting we have
    for (int i = start; i < argc; i++)
    {
        if (argv[i][0] == '-' || argv[i][0] == '(' || argv[i][0] == '!')
        {
            break;
        }
        char *toput = calloc(strlen(argv[i]) + 1, sizeof(char));
        if (toput == NULL)
        {
            fprintf(stderr, "Myfind: Give_starting malloc char* failed\n");
        }
        if (c >= size)
        {
            size = size * 2;
            toret = realloc(toret, size * sizeof(char *));
        }
        toret[c] = strcpy(toput, argv[i]);
        c = c + 1;
    }
    *fsize = c;
    return toret;
}

void free_starting(char **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}
