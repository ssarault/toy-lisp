#ifndef LISP_BUILTINS_H
#define LISP_BUILTINS_H
#include "lisp.h"

int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);
int builtin_add(Atom args, Atom *result);
int builtin_subtract(Atom args, Atom *result);
int builtin_multiply(Atom args, Atom *result);
int builtin_divide(Atom args, Atom *result);
int builtin_numeq(Atom args, Atom *result);
int builtin_less(Atom args, Atom *result);
int builtin_greater(Atom args, Atom *result);
int builtin_apply(Atom args, Atom *result);
int builtin_eq(Atom args, Atom *result);
int builtin_pairp(Atom args, Atom *result);
int builtin_concat(Atom args, Atom *result);
int builtin_import(Atom args, Atom *result);
int builtin_exit(Atom args, Atom *result);
int builtin_modulus(Atom args, Atom *result);
int builtin_bin_shift_left(Atom args, Atom *result);
int builtin_bin_shift_right(Atom args, Atom *result);
int builtin_bin_or(Atom args, Atom *result);
int builtin_bin_and(Atom args, Atom *result);
int builtin_bin_xor(Atom args, Atom *result);
int builtin_bin_complement(Atom args, Atom *result);

#endif