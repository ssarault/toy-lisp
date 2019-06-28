#include "lisp.h"
#include <ctype.h>

int listp(Atom expr);

int builtin_car(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    if (nilp(car(args)))
        *result = nil;
    else if (car(args).type != AtomType_Pair)
        return Error_Type;
    else
        *result = car(car(args));

    return Error_OK;
}

int builtin_cdr(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    if (nilp(car(args)))
        *result = nil;
    else if (car(args).type != AtomType_Pair)
        return Error_Type;
    else
        *result = cdr(car(args));

    return Error_OK;
}

int builtin_cons(Atom args, Atom *result)
{
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    *result = cons(car(args), car(cdr(args)));

    return Error_OK;
}

int builtin_add(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer + b.value.integer);

    return Error_OK;
}

int builtin_subtract(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer - b.value.integer);

    return Error_OK;
}

int builtin_multiply(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer * b.value.integer);

    return Error_OK;
}

int builtin_divide(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer / b.value.integer);

    return Error_OK;
}

int builtin_numeq(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = ((a.value.integer == b.value.integer) ? make_sym("T") : nil);

    return Error_OK;
}

int builtin_less(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = ((a.value.integer < b.value.integer) ? make_sym("T") : nil);

    return Error_OK;
}

int builtin_greater(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = ((a.value.integer > b.value.integer) ? make_sym("T") : nil);

    return Error_OK;
}

int builtin_apply(Atom args, Atom *result)
{
    Atom fn;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    fn = car(args);
    args = car(cdr(args));

    if (!listp(args))
        return Error_Syntax;

    return apply(fn, args, result);
}

int builtin_eq(Atom args, Atom *result)
{
    Atom a, b;
    int eq;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type == b.type)
    {
        switch (a.type)
        {
        case AtomType_Nil:
            eq = 1;
            break;
        case AtomType_Pair:
        case AtomType_Closure:
        case AtomType_Macro:
            eq = (a.value.pair == b.value.pair);
            break;
        case AtomType_Symbol:
            eq = (a.value.symbol == b.value.symbol);
            break;
        case AtomType_Integer:
            eq = (a.value.integer == b.value.integer);
            break;
        case AtomType_Builtin:
            eq = (a.value.builtin == b.value.builtin);
            break;
        }
    }
    else
    {
        eq = 0;
    }

    *result = (eq ? make_sym("T") : nil);
    return Error_OK;
}

int builtin_pairp(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    *result = ((car(args).type == AtomType_Pair) ? make_sym("T") : nil);
    return Error_OK;
}

void load_file(Atom env, const char *path);

int builtin_import(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    Atom a = car(args);

    if (a.type != AtomType_Symbol)
        return Error_Type;
    
    char * filename = strdup(a.value.symbol);
    char *p = filename;

    while (*p != '\0') {
        *p = tolower(*p);
        p++;
    }

    size_t len = strlen(filename);
    char * full_filename = malloc(len + 6);
    strcpy(full_filename, filename);
    strcpy(full_filename + len, ".lisp");
    load_file(main_env_nil, full_filename);

    free(filename);
    free(full_filename);

    *result = nil;

    return Error_OK;
}

int builtin_exit(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    Atom a = car(args);

    if (a.type != AtomType_Integer)
        return Error_Type;

    *result = nil;

    repl_exit_code = a.value.integer;

    return Error_Exit;
}

int builtin_concat(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Symbol || b.type != AtomType_Symbol)
        return Error_Type;

    const char *sa, *sb;
    size_t len_a, len_b;

    sa = a.value.symbol;
    sb = b.value.symbol;

    len_a = strlen(sa);
    len_b = strlen (sb);

    char *p = malloc(len_a + len_b + 1);
    strcpy(p, sa);
    strcpy(p + len_a, sb);

    *result = make_sym(p);

    free(p);

    return Error_OK;
}

int builtin_modulus(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer % b.value.integer);

    return Error_OK;
}

int builtin_bin_shift_left(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer << b.value.integer);

    return Error_OK;
}

int builtin_bin_shift_right(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer >> b.value.integer);

    return Error_OK;
}

int builtin_bin_or(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer | b.value.integer);

    return Error_OK;
}

int builtin_bin_and(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer & b.value.integer);

    return Error_OK;
}

int builtin_bin_xor(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
        return Error_Args;

    a = car(args);
    b = car(cdr(args));

    if (a.type != AtomType_Integer || b.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(a.value.integer ^ b.value.integer);

    return Error_OK;
}

int builtin_bin_complement(Atom args, Atom *result)
{
    Atom a;

    if (nilp(args) || !nilp(cdr(args)))
        return Error_Args;

    a = car(args);

    if (a.type != AtomType_Integer)
        return Error_Type;

    *result = make_int(~(a.value.integer));

    return Error_OK;
}