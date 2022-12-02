#ifndef RECURSIVE_DESCENT_H
#define RECURSIVE_DESCENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "tree.h"                                     
                                        
enum UNARY_SIGN {
    UNARY_PLUS    = 1,
    UNARY_MINUS   = 2,
    NO_UNARY_SIGN = 3,
};

Knot* GetG (char** str, struct Tree* tree);

Knot* GetE (char** str, struct Tree* tree);

Knot* GetT (char** str, struct Tree* tree);

Knot* GetPow (char** str, struct Tree* tree);

Knot* GetP (char** str, struct Tree* tree);

Knot* GetFunc (char** str, struct Tree* tree);

Knot* GetN (char** str, struct Tree* tree);

Knot* GetV (char** str, struct Tree* tree);

//==============================================

int is_unary_sign (char** str);

int is_math_func (char** str);

void pass_space (char** str);

#endif //RECURSIVE_DESCENT_H