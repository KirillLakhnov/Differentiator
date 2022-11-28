#ifndef RECURSIVE_DESCENT_H
#define RECURSIVE_DESCENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "tree.h"

#define IS_FUNCTION(math_func, func)    if (stricmp (math_func, #func) == 0)    \
                                        {                                       \
                                            return func;                        \
                                        }                                       
                                        
enum UNARY_SIGN {
    UNARY_PLUS    = 1,
    UNARY_MINUS   = 2,
    NO_UNARY_SIGN = 3,
};

Knot* GetG (char** str);

Knot* GetE (char** str);

Knot* GetT (char** str);

Knot* GetPow (char** str);

Knot* GetP (char** str);

Knot* GetFunc (char** str);

Knot* GetN (char** str);

Knot* GetV (char** str);

//==============================================

int is_unary_sign (char** str);

int is_math_func (char** str);

void pass_space (char** str);

#endif //RECURSIVE_DESCENT_H