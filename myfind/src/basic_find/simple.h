#ifndef SIMPLE_H
#define SIMPLE_H

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../token/expression.h"

int opendirerr(DIR *dir); // check result of opendir
int readerr(struct dirent *entry); // check result of readdir
int simple_find(int argc, char *argv[]); // act as ls1 wihtout argument
int simple_find_rec(char *start);
int errstat(int ret); // check error for stat

int eval_print(char *arg, char *fol);
int eval_type(char *arg, char *fol);
int eval_name(char *arg, char *fol);
int eval_newer(char *arg, char *fol);
int eval_perm(char *arg, char *fol);
int eval_user(char *arg, char *fol);
int eval_group(char *arg, char *fol);
int eval_delete(char *arg, char *fol);

int evaluate(struct my_expr *head, char **arr, int size, int isfunc);
int isDirectory(const char *path);
int asprint(char **res, const char *arg1, const char *arg2, const char *mid);

struct function
{
    enum functype todo;
    int (*fun)(char *, char *);
};

#endif /* ! SIMPLE_H */
