#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "parser.h"

int eval_expr(struct my_expr *expr)
{
    if (expr->type == EXPR_ADDITION)
    {
        return eval_expr(expr->data.children.left)
            + eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_SUBTRACTION)
    {
        return eval_expr(expr->data.children.left)
            - eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_MULTIPLICATION)
    {
        return eval_expr(expr->data.children.left)
            * eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_DIVISION)
    {
        int l = eval_expr(expr->data.children.left);
        int r = eval_expr(expr->data.children.right);
        if (r == 0)
        {
            fprintf(stderr, "Division by zero not allowed!\n");
            exit(1);
        }
        return l / r;
    }
    else if (expr->type == EXPR_NEGATION)
    {
        return -(eval_expr(expr->data.children.left));
    }
    else
    {
        return expr->data.value;
    }
}
