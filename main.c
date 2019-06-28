#include "lisp.h"
#include "lisp_environment.h"
#include "lisp_builtins.h"
#include <readline/readline.h>

int read_expr(const char *input, const char **end, Atom *result);
void load_file(Atom env, const char *path);
int repl_exit_code = 0;
Atom main_env_nil;

int main(int argc, char *argv[])
{
    Atom env;
    char *input;

    main_env_nil = env_create(nil);
    env = main_env_nil;

    /* Set up the initial environment */
    env_set(env, make_sym("T"), make_sym("T"));
    env_set(env, make_sym("CAR"), make_builtin(builtin_car));
    env_set(env, make_sym("CDR"), make_builtin(builtin_cdr));
    env_set(env, make_sym("CONS"), make_builtin(builtin_cons));
    env_set(env, make_sym("+"), make_builtin(builtin_add));
    env_set(env, make_sym("-"), make_builtin(builtin_subtract));
    env_set(env, make_sym("*"), make_builtin(builtin_multiply));
    env_set(env, make_sym("/"), make_builtin(builtin_divide));
    env_set(env, make_sym("="), make_builtin(builtin_numeq));
    env_set(env, make_sym("<"), make_builtin(builtin_less));
    env_set(env, make_sym(">"), make_builtin(builtin_greater));
    env_set(env, make_sym("APPLY"), make_builtin(builtin_apply));
    env_set(env, make_sym("EQ?"), make_builtin(builtin_eq));
    env_set(env, make_sym("PAIR?"), make_builtin(builtin_pairp));
    env_set(env, make_sym("IMPORT"), make_builtin(builtin_import));
    env_set(env, make_sym("EXIT"), make_builtin(builtin_exit));
    env_set(env, make_sym("CONCAT"), make_builtin(builtin_concat));
    env_set(env, make_sym("%"), make_builtin(builtin_modulus));
    env_set(env, make_sym("<<"), make_builtin(builtin_bin_shift_left));
    env_set(env, make_sym(">>"), make_builtin(builtin_bin_shift_right));
    env_set(env, make_sym("|"), make_builtin(builtin_bin_or));
    env_set(env, make_sym("&"), make_builtin(builtin_bin_and));
    env_set(env, make_sym("^"), make_builtin(builtin_bin_xor));
    env_set(env, make_sym("~"), make_builtin(builtin_bin_complement));

    load_file(env, "library.lisp");

    if (argc == 2) {
        load_file(env, argv[1]);
    }

    while ((input = readline("> ")) != NULL)
    {
        const char *p = input;
        Error err;
        Atom expr, result;

        err = read_expr(p, &p, &expr);

        if (!err)
            err = eval_expr(expr, env, &result);

        switch (err)
        {
        case Error_OK:
            print_expr(result);
            putchar('\n');
            break;
        case Error_Syntax:
            puts("Syntax error");
            break;
        case Error_Unbound:
            puts("Symbol not bound");
            break;
        case Error_Args:
            puts("Wrong number of arguments");
            break;
        case Error_Type:
            puts("Wrong type");
            break;
        case Error_Exit:
            puts("Exit");
            goto exit_repl_enivronment;
        }

        free(input);
    }

    exit_repl_enivronment:
        free(input);
        gc_clear_all();

    return repl_exit_code;
}