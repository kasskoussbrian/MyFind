#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum functype
{
    PRINT = 0,
    NAME,
    TYPE,
    NEWER,
    PERM,
    USER,
    GROUP,
    DELETE
};
struct token
{
    enum functype func;
    int isarg;
    char *argument;
};
enum my_expr_type
{
    EXPR_OR = 0,
    EXPR_AND,
    EXPR_NEGATION,
    EXPR_LEFT_PAR,
    EXPR_RIGHT_PAR,
    EXPR_TOKEN
};

struct my_expr
{
    enum my_expr_type type;
    union
    {
        struct
        {
            struct my_expr *left;
            struct my_expr *right;
        } children;
        struct token tok;
    } data;
};

struct stack
{
    struct my_expr *exp;
    struct stack *next;
};
void init_expr(char *str, struct my_expr *toput);

struct my_expr *get_tree(struct my_expr **list, int deb, int size);
struct stack *stack_push(struct stack *s, struct my_expr *e);
struct stack *stack_pop(struct stack *s);

void free_expr(struct my_expr *expr);

char **Give_Starting(int start, int argc, char *argv[], int *fsize);

void free_starting(char **arr, int size);
int hasfunc(struct my_expr **arr, int size);

void free_arg(struct my_expr **arr, int size);

struct my_expr **create_exp(int deb, int fin, char *argv[], int *fsize);
void print_args(struct my_expr **arr, int size);
#endif /* ! EXPRESSION_H */
