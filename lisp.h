#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef enum {
    Error_OK = 0,
    Error_Syntax,
    Error_Unbound,
    Error_Args,
    Error_Type,
    Error_Exit
} Error;

typedef struct Atom Atom;

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom {
    enum {
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol,
        AtomType_Integer,
        AtomType_Builtin,
        AtomType_Closure,
        AtomType_Macro
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

struct Allocation
{
    struct Pair pair;
    int mark : 1;
    struct Allocation *next;
};

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
#define nilp(atom) ((atom).type == AtomType_Nil)

extern const Atom nil;
extern Atom main_env_nil; // main nil enivronment
extern Atom sym_table;
extern int repl_exit_code;

Atom cons(Atom car_val, Atom cdr_val);

Atom make_int(long x);

Atom make_sym(const char *s);

Atom make_builtin(Builtin fn);

int make_closure(Atom env, Atom args, Atom body, Atom *result);

Atom copy_list(Atom list);

int apply(Atom fn, Atom args, Atom *result);

Atom list_get(Atom list, int k);

void list_set(Atom list, int k, Atom value);

void list_reverse(Atom *list);

void print_expr(Atom atom);

void gc_mark(Atom root);

void gc();

void gc_clear_all();

#endif