#include "../basic_find/simple.h"
static void check_perm(char *num)
{
    char *checker;
    char *cnum;
    if (num[0] == '-' || num[0] == '/')
    {
        cnum = num + 1;
    }
    else
    {
        cnum = num;
    }
    int i = 0;
    while (cnum[i] != '\0')
    {
        if (cnum[i] - '0' >= 8)
        {
            fprintf(stderr, "myfind: check_perm : argument is weird \n");
            exit(1);
        }
        i = i + 1;
    }
    int res = strtol(cnum, &checker, 8);
    if (strlen(checker) != 0)
    {
        fprintf(stderr, "myfind: check_perm : argument is weird 1 \n");
        exit(1);
    }
    res = res;
}

static void print_tok(struct token tok)
{
    if (tok.func == 0)
    {
        printf("PRINT");
    }
    else if (tok.func == 1)
    {
        printf("NAME");
    }
    else if (tok.func == 2)
    {
        printf("TYPE");
    }
    else if (tok.func == 3)
    {
        printf("NEWER");
    }
    else if (tok.func == 4)
    {
        printf("PERM");
    }
    else if (tok.func == 5)
    {
        printf("USER");
    }
    else if (tok.func == 6)
    {
        printf("GROUP");
    }
    else if (tok.func == 7)
    {
        printf("DELETE");
    }
}
static void print_expr(struct my_expr *exp)
{
    if (exp->type == 0)
    {
        printf("OR");
    }
    else if (exp->type == 1)
    {
        printf("AND");
    }
    else if (exp->type == 2)
    {
        printf("!");
    }
    else if (exp->type == 3)
    {
        printf("(");
    }
    else if (exp->type == 4)
    {
        printf(")");
    }
    else
    {
        fprintf(stderr, "myfind: print_expr a token wrong type\n");
        exit(1);
    }
}
void print_args(struct my_expr **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i]->type == EXPR_TOKEN)
        {
            printf("|");
            print_tok(arr[i]->data.tok);
            if (arr[i]->data.tok.isarg == 1)
            {
                printf("<%s>", arr[i]->data.tok.argument);
            }
            printf("| ");
        }
        else
        {
            printf("|");
            print_expr(arr[i]);
            printf("| ");
        }
    }
    printf("\n");
}
int hasfunc(struct my_expr **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i]->type == EXPR_TOKEN
            && (arr[i]->data.tok.func == PRINT
                || arr[i]->data.tok.func == DELETE))
        {
            return 1;
        }
    }
    return 0;
}
void free_arg(struct my_expr **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

static struct token init_token(char *name)
{
    struct token tok;
    if (strcmp(name, "-name") == 0)
    {
        tok.func = NAME;
    }
    else if (strcmp(name, "-print") == 0)
    {
        tok.func = PRINT;
    }
    else if (strcmp(name, "-type") == 0)
    {
        tok.func = TYPE;
    }
    else if (strcmp(name, "-newer") == 0)
    {
        tok.func = NEWER;
    }
    else if (strcmp(name, "-perm") == 0)
    {
        tok.func = PERM;
    }
    else if (strcmp(name, "-user") == 0)
    {
        tok.func = USER;
    }
    else if (strcmp(name, "-group") == 0)
    {
        tok.func = GROUP;
    }
    else if (strcmp(name, "-delete") == 0)
    {
        tok.func = DELETE;
    }
    else
    {
        fprintf(
            stderr,
            "myfind: init_token:  Unknown command or  Unimplemented command\n");
        exit(1);
    }
    return tok;
}

void init_expr(char *str, struct my_expr *toput)
{
    if (!strcmp(str, ")"))
    {
        toput->type = EXPR_RIGHT_PAR;
    }
    else if (!strcmp(str, "("))
    {
        toput->type = EXPR_LEFT_PAR;
    }
    else if (!strcmp(str, "-o"))
    {
        toput->type = EXPR_OR;
    }
    else if (!strcmp(str, "-a"))
    {
        toput->type = EXPR_AND;
    }
    else if (!strcmp(str, "!"))
    {
        toput->type = EXPR_NEGATION;
    }
    else
    {
        toput->type = EXPR_TOKEN;
        toput->data.tok = init_token(str);
    }
    if (toput->type != EXPR_TOKEN)
    {
        toput->data.children.left = NULL;
        toput->data.children.right = NULL;
    }
}

static int putand(char *str, int pos, int deb)
{
    return (str[0] == '-' && str[1] != 'o' && str[1] != 'a')
        || (strcmp(str, "(") == 0 && pos != deb) || str[0] == '!';
}

static void check_expr(struct my_expr **list, int size)
{
    for (int i = 0; i < size; i++)
    {
        struct my_expr *cur = list[i];
        if (cur->type == EXPR_TOKEN)
        {
            struct token tcur = cur->data.tok;
            if (tcur.func == TYPE)
            {
                if (!(!strcmp("b", tcur.argument) || !strcmp("c", tcur.argument)
                      || !strcmp("d", tcur.argument)
                      || !strcmp("f", tcur.argument)
                      || !strcmp("l", tcur.argument)
                      || !strcmp("p", tcur.argument)
                      || !strcmp("s", tcur.argument)))

                {
                    fprintf(stderr, "myfind: check_expr: Undefined command \n");
                    exit(1);
                }
            }

            if (tcur.func == PERM)
            {
                check_perm(tcur.argument);
            }
        }
    }
}

struct my_expr **create_exp(int deb, int fin, char *argv[], int *fsize)
{
    struct my_expr **toret = calloc(1, sizeof(struct my_expr *));
    if (toret == NULL)
    {
        fprintf(stderr, "Myfind: create_exp :  calloc my_expr** failed\n");
    }
    int size = 1;

    int c = 0; // number of starting we have
    for (int i = deb; i < fin; i++)
    {
        struct my_expr *toput = calloc(1, sizeof(struct my_expr));
        if (toput == NULL)
        {
            fprintf(stderr, "Myfind: create_exp :  calloc my_expr* failed\n");
        }
        init_expr(argv[i], toput);
        if (i + 1 < fin && toput->data.tok.func != PRINT
            && toput->data.tok.func != DELETE)
        {
            toput->data.tok.isarg = 1;
            toput->data.tok.argument = argv[i + 1];
            i = i + 1;
        }
        else
        {
            if (toput->data.tok.func != PRINT && toput->data.tok.func != DELETE)
            {
                fprintf(stderr,
                        "myfind: create exp : An argument always expect a "
                        "parametre\n");
                exit(1);
            }
        }
        if (c >= size)
        {
            size = size * 2;
            toret = realloc(toret, size * sizeof(char *));
        }
        toret[c] = toput;
        c = c + 1;
        if (i + 1 < fin)
        {
            if (putand(argv[i + 1], i, deb) && strcmp("(", argv[i])
                && strcmp("!", argv[i]) && strcmp("-o", argv[i])
                && strcmp("-a", argv[i]))
            {
                struct my_expr *extra = calloc(1, sizeof(struct my_expr));
                init_expr("-a", extra);
                if (toput == NULL)
                {
                    fprintf(stderr,
                            "Myfind: create_exp :  calloc my_expr* failed\n");
                }
                if (c >= size)
                {
                    size = size * 2;
                    toret = realloc(toret, size * sizeof(char *));
                }
                toret[c] = extra;
                c = c + 1;
            }
        }
    }
    *fsize = c;
    check_expr(toret, c);
    return toret;
}
