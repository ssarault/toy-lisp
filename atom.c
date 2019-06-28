#include "atom.h"

const Atom nil = {AtomType_Nil};

Atom cons(Atom car_val, Atom cdr_val)
{
    Atom p;

    p.type = AtomType_Pair;
    p.value.pair = malloc(sizeof(struct Pair));

    // if (car_val.type == AtomType_Symbol)
    //     car_val.value.symbol = strdup(car_val.value.symbol);

    // if (cdr_val.type == AtomType_Symbol)
    //     cdr_val.value.symbol = strdup(cdr_val.value.symbol);

    // if (car_val.type == AtomType_Pair)
    //     car_val = cons(car_val.value.pair->atom[0], car_val.value.pair->atom[1]);

    // if (cdr_val.type == AtomType_Pair)
    //     cdr_val = cons(cdr_val.value.pair->atom[0], cdr_val.value.pair->atom[1]);

    car(p) = car_val;
    cdr(p) = cdr_val;

    return p;
}

Atom copy(Atom a)
{
    switch(a.type) {
        case AtomType_Nil:
            break;
        case AtomType_Integer:
            break;
        case AtomType_Symbol:
            // a.value.symbol = strdup(a.value.symbol);
            break;
        case AtomType_Pair:
            // Atom p;
            // p.type = AtomType_Pair;
            // p.value.pair = malloc(sizeof(struct Pair));
            // car(p) = copy(a.value.pair->atom[0]);
            // cdr(p) = copy(a.value.pair->atom[1]);
            // a = p;
            a = cons(copy(a.value.pair->atom[0]), copy(a.value.pair->atom[1]));
            break;
    }

    return a;
}

Atom make_int(long x)
{
    Atom a;
    a.type = AtomType_Integer;
    a.value.integer = x;
    return a;
}

Atom sym_table = {AtomType_Nil};

Atom make_sym(const char *s)
{
    Atom a, p;

    p = sym_table;
    while (!nilp(p))
    {
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
    }
}

void delete(Atom *a)
{
    
    // switch(a->type) {
    //     case AtomType_Nil:
    //         break;
    //     case AtomType_Integer:
    //         break;
    //     case AtomType_Symbol:
    //         // free((void *)(a->value.symbol));
    //         break;
    //     case AtomType_Pair:
    //         delete(&(a->value.pair->atom[0])); 
    //         delete(&(a->value.pair->atom[1]));
    //         free(a->value.pair);
    //         break;
    // }
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

void dispose(Atom *a)
{
    a->type = AtomType_Nil;
}