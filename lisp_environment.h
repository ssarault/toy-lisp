#ifndef LISP_ENV_H
#define LISP_ENV_H
#include "lisp.h"

Atom env_create(Atom parent);
int env_get(Atom env, Atom symbol, Atom *result);
int env_set(Atom env, Atom symbol, Atom value);
int listp(Atom expr);
int eval_expr(Atom expr, Atom env, Atom *result);

#endif