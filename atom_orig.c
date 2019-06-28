#include "atom.h"
#include "atom_error.h"

const Atom nil = {AtomType_Nil};
Atom sym_table = {AtomType_Nil};

Atom cons(Atom car_val, Atom cdr_val)
{
    Atom p;

    p.type = AtomType_Pair;
    p.value.pair = malloc(sizeof(struct Pair));

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
    if (fn.type == AtomType_Builtin)
        return (*fn.value.builtin)(args, result);

    return Error_Type;
}

Atom copy(Atom a)
{
    switch (a.type) {
    case AtomType_Nil:
        break;
    case AtomType_Integer:
        break;
    case AtomType_Symbol:
        break;
    case AtomType_Pair:
        a = cons(copy(a.value.pair->atom[0]), copy(a.value.pair->atom[1]));
        break;
    }

    return a;
}

void delete(Atom *a)
{
    if (a->type == AtomType_Pair) {
        delete (&(a->value.pair->atom[0]));
        delete (&(a->value.pair->atom[1]));
        free(a->value.pair);
    }

    a->type = AtomType_Nil;
}

void _delete_sym_table(Atom *a)
{
    switch(a->type) {
        case AtomType_Nil:
            break;
        case AtomType_Integer:
            break;
        case AtomType_Symbol:
            free((void *)(a->value.symbol));
            break;
        case AtomType_Pair:
            _delete_sym_table(&(a->value.pair->atom[0])); 
            _delete_sym_table(&(a->value.pair->atom[1]));
            free(a->value.pair);
            break;
    }

    a->type = AtomType_Nil;
}

void _print_expr(Atom atom)
{
    switch (atom.type)
    {
    case AtomType_Nil:
        printf("NIL");
        break;
    case AtomType_Pair:
        putchar('(');
        _print_expr(car(atom));
        atom = cdr(atom);
        while (!nilp(atom))
        {
            if (atom.type == AtomType_Pair)
            {
                putchar(' ');
                _print_expr(car(atom));
                atom = cdr(atom);
            }
            else
            {
                printf(" . ");
                _print_expr(atom);
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
    }
}