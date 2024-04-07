#include "simple.h"
int is_simblink(char *name)
{
    struct stat statbuf;
    if (errstat(lstat(name, &statbuf) == 1))
    {
        fprintf(stderr, "myfind: eval_type : problem with stat function\n");
        exit(1);
    }
    return S_ISLNK(statbuf.st_mode);
}
int asprint(char **res, const char *arg1, const char *arg2, const char *mid)
{
    *res = calloc(strlen(arg1) + strlen(arg2) + strlen(mid) + 2, sizeof(char));
    if (*res == NULL)
    {
        return -1;
    }
    if (sprintf(*res, "%s%s%s", arg1, mid, arg2) < -1)
    {
        fprintf(stderr, "myfind:asprintf : problem with sprintf\n");
    }
    return 0;
}

int eval_print(char *arg, char *fol)
{
    arg = arg;
    printf("%s\n", fol);
    return 1;
}
int eval_type(char *arg, char *fol)
{
    struct stat statbuf;
    if (errstat(lstat(fol, &statbuf) == 1))
    {
        fprintf(stderr, "myfind: eval_type : problem with stat function\n");
        exit(1);
    }
    if (!strcmp(arg, "b"))
    {
        return S_ISBLK(statbuf.st_mode);
    }
    else if (!strcmp(arg, "c"))
    {
        return S_ISCHR(statbuf.st_mode);
    }
    else if (!strcmp(arg, "d"))
    {
        return S_ISDIR(statbuf.st_mode);
    }
    else if (!strcmp(arg, "f"))
    {
        return S_ISREG(statbuf.st_mode);
    }
    else if (!strcmp(arg, "l")) //
    {
        return (S_ISLNK(statbuf.st_mode));
    }
    else if (!strcmp(arg, "p"))
    {
        return S_ISFIFO(statbuf.st_mode);
    }
    else if (!strcmp(arg, "s")) //
    {
        return (S_ISSOCK(statbuf.st_mode));
    }
    else
    {
        fprintf(stderr,
                "myfind: eval_type : test 'type' took a wrong argument\n");
        exit(1);
    }
}
// need to see if name arg contains a '/' and return an error
int eval_name(char *arg, char *fol)
{
    const char *pat;
    int i = 0;
    pat = fol;
    while (fol[i] != '\0')
    {
        if (fol[i] == '/')
        {
            pat = fol + i + 1;
        }
        i = i + 1;
    }
    if (!fnmatch(arg, pat, FNM_NOESCAPE))
    {
        return 1;
    }
    return 0;
}
int eval_newer(char *arg, char *fol)
{
    struct stat cur;
    if (errstat(lstat(fol, &cur) == 1))
    {
        fprintf(stderr, "myfind: eval_type : problem with stat function\n");
        exit(1);
    }
    struct stat sarg;
    if (errstat(lstat(arg, &sarg) == 1))
    {
        fprintf(stderr, "myfind: eval_type : problem with stat function\n");
        exit(1);
    }
    if (cur.st_mtim.tv_sec > sarg.st_mtim.tv_sec)
    {
        return 1;
    }
    if (cur.st_mtim.tv_sec < sarg.st_mtim.tv_sec)
    {
        return 0;
    }
    if (cur.st_mtim.tv_nsec > sarg.st_mtim.tv_nsec)
    {
        return 1;
    }
    return 0;
}
int eval_perm(char *arg, char *fol)
{
    arg = arg;
    struct stat cur;
    if (errstat(lstat(fol, &cur) == 1))
    {
        fprintf(stderr, "myfind: eval_type : problem with stat function\n");
        exit(1);
    }
    int base = cur.st_mode & 0777;
    // printf("base %d\n",base);
    char *reso;
    if (arg[0] == '-')
    {
        char *cfol = arg + 1;
        int numarg = strtol(cfol, &reso, 8);
        return (numarg & base) == numarg;
    }
    else if (arg[0] == '/')
    {
        char *cfol = arg + 1;
        int numarg = strtol(cfol, &reso, 8);
        return (base & numarg) > 0;
    }
    else
    {
        int numarg = strtol(arg, &reso, 8);
        return numarg == base;
    }
}
int eval_user(char *arg, char *fol)
{
    struct stat cur;
    if (errstat(lstat(fol, &cur) == 1))
    {
        fprintf(stderr, "myfind: eval_user : problem with stat function\n");
        exit(1);
    }
    struct passwd *pwd = getpwuid(cur.st_uid);
    return !strcmp(arg, pwd->pw_name);
}
int eval_group(char *arg, char *fol)
{
    struct stat cur;
    if (errstat(lstat(fol, &cur) == 1))
    {
        fprintf(stderr, "myfind: eval_group : problem with stat function\n");
        exit(1);
    }
    struct group *group;
    group = getgrgid(cur.st_gid);
    return !strcmp(arg, group->gr_name);
}

int eval_delete(char *arg, char *fol)
{
    arg = arg;
    if (remove(fol) == 0)
    {
        return 1;
    }
    fprintf(stderr, "myfind:theres an error in the deletion\n");
    exit(1);
}

static int evaluate_rec(struct my_expr *head, struct function tab[],
                        int func_number, char *fichier)
{
    if (head->type == EXPR_OR)
    {
        int res1 =
            evaluate_rec(head->data.children.left, tab, func_number, fichier);
        if (res1 == 1)
        {
            return res1;
        }
        int res2 =
            evaluate_rec(head->data.children.right, tab, func_number, fichier);
        return res1 | res2;
    }
    else if (head->type == EXPR_AND)
    {
        int res1 =
            evaluate_rec(head->data.children.left, tab, func_number, fichier);
        if (res1 == 0)
        {
            return res1;
        }
        int res2 =
            evaluate_rec(head->data.children.right, tab, func_number, fichier);
        return res1 & res2;
    }
    else if (head->type == EXPR_NEGATION)
    {
        return !(
            evaluate_rec(head->data.children.left, tab, func_number, fichier));
    }
    else if (head->type != EXPR_TOKEN)
    {
        fprintf(stderr,
                "myfind: evaluate_rec : found not a token on leaf while "
                "evaluating\n");
        exit(1);
    }
    else
    {
        for (int i = 0; i < func_number; i++)
        {
            if (tab[i].todo == head->data.tok.func)
            {
                if (head->data.tok.isarg == 1)
                {
                    return tab[i].fun(head->data.tok.argument, fichier);
                }
                return tab[i].fun("", fichier);
            }
        }
        fprintf(stderr,
                "myfind:evaluate rec:  did not find any function given\n");
        exit(1);
    }
}

static int eval_rec(struct my_expr *head, struct function tab[],
                    int func_number, char *fichier, int hasfunc)
{
    DIR *dir = opendir(fichier);
    if (opendirerr(dir) == 1)
    {
        return 1;
    }
    if (evaluate_rec(head, tab, func_number, fichier) && hasfunc == 0)
    {
        eval_print("", fichier);
    }
    struct dirent *entry = readdir(dir);
    int ret = 0;
    for (; entry; entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char *concat;
            int asret;
            if (fichier[strlen(fichier) - 1] == '/')
            {
                asret = asprint(&concat, fichier, entry->d_name, "");
                // asret = asprintf(&concat,"%s%s",fichier,entry->d_name);
            }
            else
            {
                // asret = asprintf(&concat,"%s/%s",fichier,entry->d_name);
                asret = asprint(&concat, fichier, entry->d_name, "/");
            }
            if (asret == -1)
            {
                fprintf(stderr, "my_find: Error in concat\n");
                exit(1);
            }
            if (isDirectory(entry->d_name) == -1)
            {
                fprintf(stderr,
                        "myfind: simple_find_rec: error when openning dir\n");
                ret = 1;
            }
            else
            {
                if (isDirectory(concat) && !is_simblink(concat))
                {
                    ret =
                        ret | eval_rec(head, tab, func_number, concat, hasfunc);
                }
                else
                {
                    if (evaluate_rec(head, tab, func_number, concat)
                        && hasfunc == 0)
                    {
                        eval_print("", concat);
                    }
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
int evaluate(struct my_expr *head, char **arr, int size, int isfunc)
{
    struct function ev_print;
    ev_print.fun = eval_print;
    ev_print.todo = PRINT;
    struct function ev_type;
    ev_type.fun = eval_type;
    ev_type.todo = TYPE;
    struct function ev_name;
    ev_name.fun = eval_name;
    ev_name.todo = NAME;
    struct function ev_newer;
    ev_newer.fun = eval_newer;
    ev_newer.todo = NEWER;
    struct function ev_perm;
    ev_perm.fun = eval_perm;
    ev_perm.todo = PERM;
    struct function ev_user;
    ev_user.fun = eval_user;
    ev_user.todo = USER;
    struct function ev_group;
    ev_group.fun = eval_group;
    ev_group.todo = GROUP;
    struct function ev_delete;
    ev_delete.fun = eval_delete;
    ev_delete.todo = DELETE;

    struct function tab[] = { ev_print, ev_type, ev_name,  ev_newer,
                              ev_perm,  ev_user, ev_group, ev_delete };
    int func_number = 8;
    int x = 0;
    if (head == NULL)
    {
        x = 1;
        head = calloc(1, sizeof(struct my_expr));
        isfunc = 1;
        init_expr("-print", head);
    }
    int res = 0;
    for (int i = 0; i < size; i++)
    {
        int resx = eval_rec(head, tab, func_number, arr[i], isfunc);
        res = res || resx;
    }
    if (size == 0)
    {
        res = eval_rec(head, tab, func_number, ".", isfunc);
    }
    if (x == 1)
    {
        free(head);
    }
    return res;
}
