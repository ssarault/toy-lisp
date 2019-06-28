#include "lisp.h"
#include "lisp_environment.h"

const Atom nil = {AtomType_Nil};
Atom sym_table = {AtomType_Nil};
struct Allocation *global_allocations = NULL;

Atom cons(Atom car_val, Atom cdr_val)
{
    struct Allocation *a;
    Atom p;

    a = malloc(sizeof(struct Allocation));
    a->mark = 0;
    a->next = global_allocations;
    global_allocations = a;

    p.type = AtomType_Pair;
    p.value.pair = &a->pair;

    car(p) = car_val;
    cdr(p) = cdr_val;

    return p;
}

Atom make_int(long x)
{
    Atom a;
    a.type = AtomType_Integer;
    a.value.integer = x;
    return a;
}

Atom make_sym(const char *s)
{
	Atom a, p;

	p = sym_table;
	while (!nilp(p)) {
		a = car(p);
		if (strcmp(a.value.symbol, s) == 0)
			return a;
		p = cdr(p);
	}

	a.type = AtomType_Symbol;
	a.value.symbol = strdup(s);
	sym_table = cons(a, sym_table);

	return a;
}

Atom make_builtin(Builtin fn)
{
    Atom a;
    a.type = AtomType_Builtin;
    a.value.builtin = fn;
    return a;
}

int make_closure(Atom env, Atom args, Atom body, Atom *result)
{
    Atom p;

    if (!listp(body))
        return Error_Syntax;

    /* Check argument names are all symbols */
    p = args;
    while (!nilp(p))
    {
        if (p.type == AtomType_Symbol)
            break;
        else if (p.type != AtomType_Pair || car(p).type != AtomType_Symbol)
            return Error_Type;
        p = cdr(p);
    }

    *result = cons(env, cons(args, body));
    result->type = AtomType_Closure;

    return Error_OK;
}

Atom copy_list(Atom list)
{
    Atom a, p;

    if (nilp(list))
        return nil;

    a = cons(car(list), nil);
    p = a;
    list = cdr(list);

    while (!nilp(list))
    {
        cdr(p) = cons(car(list), nil);
        p = cdr(p);
        list = cdr(list);
    }

    return a;
}

int apply(Atom fn, Atom args, Atom *result)
{
    Atom env, arg_names, body;

    if (fn.type == AtomType_Builtin)
        return (*fn.value.builtin)(args, result);
    else if (fn.type != AtomType_Closure)
        return Error_Type;

    env = env_create(car(fn));
    arg_names = car(cdr(fn));
    body = cdr(cdr(fn));

    /* Bind the arguments */
    while (!nilp(arg_names))
    {
        if (arg_names.type == AtomType_Symbol)
        {
            env_set(env, arg_names, args);
            args = nil;
            break;
        }

        if (nilp(args))
            return Error_Args;
        env_set(env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }
    if (!nilp(args))
        return Error_Args;

    /* Evaluate the body */
    while (!nilp(body))
    {
        Error err = eval_expr(car(body), env, result);
        if (err)
            return err;
        body = cdr(body);
    }

    return Error_OK;
}

Atom list_get(Atom list, int k)
{
    while (k--)
        list = cdr(list);
    return car(list);
}

void list_set(Atom list, int k, Atom value)
{
    while (k--)
        list = cdr(list);
    car(list) = value;
}

void list_reverse(Atom *list)
{
    Atom tail = nil;
    while (!nilp(*list))
    {
        Atom p = cdr(*list);
        cdr(*list) = tail;
        tail = *list;
        *list = p;
    }
    *list = tail;
}

void print_expr(Atom atom)
{
    switch (atom.type)
    {
    case AtomType_Nil:
        printf("NIL");
        break;
    case AtomType_Pair:
        putchar('(');
        print_expr(car(atom));
        atom = cdr(atom);
        while (!nilp(atom))
        {
            if (atom.type == AtomType_Pair)
            {
                putchar(' ');
                print_expr(car(atom));
                atom = cdr(atom);
            }
            else
            {
                printf(" . ");
                print_expr(atom);
                break;
            }
        }
        putchar(')');
        break;
    case AtomType_Symbol:
        printf("%s", atom.value.symbol);
        break;
    case AtomType_Integer:
        printf("%ld", atom.value.integer);
        break;
    case AtomType_Builtin:
        printf("#<BUILTIN:%p>", atom.value.builtin);
        break;
    case AtomType_Closure:
        print_expr(cdr(atom));
		break;
    default:
        printf("UNKNOWN TYPE");
    }
}

void gc_mark(Atom root)
{
    struct Allocation *a;

    if (!(root.type == AtomType_Pair || root.type == AtomType_Closure || root.type == AtomType_Macro))
        return;

    a = (struct Allocation *)((char *)root.value.pair - offsetof(struct Allocation, pair));

    if (a->mark)
        return;

    a->mark = 1;

    gc_mark(car(root));
    gc_mark(cdr(root));
}

void gc()
{
    struct Allocation *a, **p;

    gc_mark(sym_table);

    /* Free unmarked allocations */
    p = &global_allocations;
    while (*p != NULL)
    {
        a = *p;
        if (!a->mark)
        {
            *p = a->next;
            free(a);
        }
        else
        {
            p = &a->next;
        }
    }

    /* Clear marks */
    a = global_allocations;
    while (a != NULL)
    {
        a->mark = 0;
        a = a->next;
    }
}

void gc_clear_all()
{
    Atom sa, sb;
    sa = sym_table;
    while (!nilp(sa))
    {
        sb = car(sa);
        free((void *)sb.value.symbol);
        sa = cdr(sa);
    }

    struct Allocation *a, **p;

    p = &global_allocations;
    while (*p != NULL)
    {
        a = *p;   
        *p = a->next;
        free(a);
    }
}