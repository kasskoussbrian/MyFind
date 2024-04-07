#include "basic_find/simple.h"
#include "token/expression.h"

int main(int argc, char *argv[])
{
    int size;
    char **arr = Give_Starting(1, argc, argv, &size);
    int size2;
    struct my_expr **exp = create_exp(size + 1, argc, argv, &size2);
    struct my_expr *hey = get_tree(exp, 0, size2);
    int ret = evaluate(hey, arr, size, hasfunc(exp, size2));

    free_starting(arr, size);
    free_arg(exp, size2);
    return ret;
}
