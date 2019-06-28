#include "atom.h"

int main(int argc, char *argv[])
{
    Atom x = make_int(10);
    Atom s = make_sym("hello");
    Atom p = cons(x, s);
    Atom z = cons(make_int(20), make_sym("cool"));
    print_expr(x);
    delete (&x);
    print_expr(s);
    delete (&s);
    print_expr(p);
    delete (&p);
    print_expr(z);
    delete (&z);
    print_expr(x);
    print_expr(s);
    print_expr(p);
    print_expr(z);
    delete_sym_table;
    printf("Symbol table deleted \n");
    print_expr(s);
    return 0;
}