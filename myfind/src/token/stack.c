#include "expression.h"

struct stack *stack_push(struct stack *s, struct my_expr *e)
{
    struct stack *toret = calloc(1, sizeof(struct stack));
    toret->exp = e;
    if (s == NULL)
    {
        toret->next = NULL;

        return toret;
    }
    else
    {
        toret->next = s;
    }
    return toret;
}

struct stack *stack_pop(struct stack *s)
{
    struct stack *toret = s->next;
    free(s);
    return toret;
}
