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

Knot* GetG (const char* str);

Knot* GetE ();

Knot* GetT ();

Knot* GetPow ();

Knot* GetP ();

Knot* GetFunc ();

Knot* GetConst ();

Knot* GetN ();

Knot* GetV ();

//==============================================

int is_unary_sign ();

int is_math_func ();

int is_math_const ();

void pass_space ();

#endif //RECURSIVE_DESCENT_H