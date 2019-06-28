#ifndef ATOM_H
#define ATOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom {
    enum {
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol,
        AtomType_Integer,
        AtomType_Builtin
    } type;

    union {
        struct Pair *pair;
        const char *symbol;
        long integer;
        Builtin builtin;
    } value;
};

struct Pair {
    struct Atom atom[2];
};

typedef struct Atom Atom;

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
#define nilp(atom) ((atom).type == AtomType_Nil)

extern const Atom nil;
extern Atom sym_table;

Atom cons(Atom car_val, Atom cdr_val);

Atom make_int(long x);

Atom make_sym(const char *s);

Atom make_builtin(Builtin fn);

Atom copy_list(Atom list);

int apply(Atom fn, Atom args, Atom *result);

Atom copy(Atom a);

void _print_expr(Atom atom);

void delete(Atom *a);

void _delete_sym_table(Atom *a);

#define delete_sym_table \
do { \
    _delete_sym_table(&sym_table); \
} while(0)

#define print_expr(atom) \
do { \
    _print_expr((atom)); \
    putchar('\n'); \
} while(0)

#endif