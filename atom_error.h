#ifndef ATOM_ERROR_H
#define ATOM_ERROR_H

typedef enum {
    Error_OK = 0,
    Error_Syntax,
    Error_Unbound,
    Error_Args,
    Error_Type
} Error;

#endif