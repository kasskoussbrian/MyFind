#include "expression.h"
void free_expr(struct my_expr *expr)
{
    if (expr->type != EXPR_TOKEN)
    {
        if (expr->data.children.right)
            free_expr(expr->data.children.right);
        if (expr->data.children.left)
            free_expr(expr->data.children.left);
    }
    free(expr);
}

struct my_expr *get_tree(struct my_expr **list, int deb, int size)
{
    struct stack *op_stack = NULL;
    struct stack *exp_stack = NULL;
    for (int i = deb; i < size; i++)
    {
        if (list[i]->type == EXPR_LEFT_PAR)
        {
            op_stack = stack_push(op_stack, list[i]);
        }
        else if (list[i]->type == EXPR_TOKEN)
        {
            exp_stack = stack_push(exp_stack, list[i]);
        }
        else if (list[i]->type != EXPR_TOKEN && list[i]->type != EXPR_RIGHT_PAR)
        {
            while (op_stack != NULL)
            {
                if (op_stack->exp->type == EXPR_LEFT_PAR)
                {
                    break;
                }
                if (op_stack->exp->type < list[i]->type)
                {
                    break;
                }
                struct my_expr *topush = op_stack->exp;
                op_stack = stack_pop(op_stack);
                if (topush->type == EXPR_NEGATION)
                {
                    if (exp_stack == NULL)
                    {
                        fprintf(stderr,
                                "myfind: shaunting tree theres an error in the "
                                "expression given\n");
                        exit(1);
                    }
                    topush->data.children.left = exp_stack->exp;
                }
                else
                {
                    if (exp_stack == NULL)
                    {
                        fprintf(stderr,
                                "myfind: shaunting tree theres an error in the "
                                "expression given\n");
                        exit(1);
                    }
                    topush->data.children.right = exp_stack->exp;
                    exp_stack = stack_pop(exp_stack);
                    if (exp_stack == NULL)
                    {
                        fprintf(stderr,
                                "myfind: shaunting tree theres an error in the "
                                "expression given\n");
                        exit(1);
                    }
                    topush->data.children.left = exp_stack->exp;
                }
                exp_stack = stack_pop(exp_stack);
                exp_stack = stack_push(exp_stack, topush);
            }
            op_stack = stack_push(op_stack, list[i]);
        }
        else if (list[i]->type == EXPR_RIGHT_PAR)
        {
            while (op_stack != NULL)
            {
                if (op_stack->exp->type == EXPR_LEFT_PAR)
                {
                    break;
                }
                struct my_expr *topush = op_stack->exp;
                op_stack = stack_pop(op_stack);
                if (topush->type == EXPR_NEGATION)
                {
                    topush->data.children.left = exp_stack->exp;
                }
                else
                {
                    if (exp_stack == NULL)
                    {
                        fprintf(stderr,
                                "myfind: shaunting tree theres an error in the "
                                "expression given\n");
                        exit(1);
                    }
                    topush->data.children.right = exp_stack->exp;
                    exp_stack = stack_pop(exp_stack);
                    if (exp_stack == NULL)
                    {
                        fprintf(stderr,
                                "myfind: shaunting tree theres an error in the "
                                "expression given\n");
                        exit(1);
                    }
                    topush->data.children.left = exp_stack->exp;
                }
                exp_stack = stack_pop(exp_stack);
                exp_stack = stack_push(exp_stack, topush);
            }
            op_stack = stack_pop(op_stack);
        }
        else
        {
            fprintf(stderr,
                    "myfind: get_tree : theres is an error parsing the token "
                    "list\n");
        }
    }
    while (op_stack != NULL)
    {
        struct my_expr *topush = op_stack->exp;
        if (exp_stack == NULL)
        {
            fprintf(stderr,
                    "myfind: shaunting tree theres an error in the expression "
                    "given\n");
            exit(1);
        }
        op_stack = stack_pop(op_stack);
        if (topush->type == EXPR_NEGATION)
        {
            topush->data.children.left = exp_stack->exp;
        }
        else
        {
            topush->data.children.right = exp_stack->exp;
            exp_stack = stack_pop(exp_stack);
            if (exp_stack == NULL)
            {
                fprintf(stderr,
                        "myfind: shaunting tree theres an error in the "
                        "expression given\n");
                exit(1);
            }
            topush->data.children.left = exp_stack->exp;
        }
        exp_stack = stack_pop(exp_stack);
        exp_stack = stack_push(exp_stack, topush);
    }

    if (deb != size)
    {
        struct my_expr *fin = exp_stack->exp;
        exp_stack = stack_pop(exp_stack);
        return fin;
    }
    return NULL;
}
