#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "libraries/FileProcessing.h"
#include "libraries/common.h"

#include "differentiator.h"

// #define NDEBUG
#ifndef NDEBUG
#define ASSERT_OK_TREE(tree)    do                                                                            \
                                {                                                                             \
                                    if (tree_error (tree) != 0)                                               \
                                    {                                                                         \
                                        tree_graph_dump (tree);                                               \
                                        printf ("code_error = %lu on line %d", tree->code_error, __LINE__);   \
                                        abort ();                                                             \
                                    }                                                                         \
                                } while (0)

#else
#define ASSERT_OK(tree)
#endif                        

const double POISON_VALUE = NAN;
const size_t MAX_LEN_STR = 300;
const size_t DTOR_SIZE_T = 0xBABADEDA;
const int DTOR_INT = 0xBABADEAD;

const size_t MAX_NUMBER_VARIEBLE = 50;

enum TREE_ERROR
{
    TREE_ERROR_SYNTAX_IN_BASE         = 1 << 0,
    TREE_WRONG_NAME_DATA_BASE         = 1 << 1,
    TREE_ERROR_POINTER_STRUCT_NULL    = 1 << 2,
    TREE_ERROR_POINTER_ROOT_NULL      = 1 << 3,
    TREE_ERROR_POINTER_BUFFER_NULL    = 1 << 4,
    TREE_ERROR_POINTER_FILE_INFO_NULL = 1 << 5,
    TREE_ERROR_SIZE_SMALLER_ZERO      = 1 << 6,
    TREE_ERROR_SYNTAX_IN_TREE         = 1 << 7,

    TREE_ERROR_CALLOC         = 1 << 8,
    TREE_ERROR_STRCPY         = 1 << 9,
    TREE_ERROR_BUFFER_CREATER = 1 << 10,
};

enum TYPE {
    OPERATION   = 1,
    NUMBER      = 2,
    VARIABLE    = 3,
    FUNCTION    = 4,
};

enum OPERATION_TYPE {
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,
};

enum MATH_CONST {
    EXP_CONST = 1,
    PI_CONST  = 2,
};

enum MATH_FUNC {
    SIN = 1, 
    COS = 2,
    TG  = 3,
    
    ARCSIN = 4,
    ARCCOS = 5,
    ARCTG  = 6,

    LN = 7,

    SH  = 8,
    CH  = 9,    
    TH  = 10,   

    ARCSH  = 11,
    ARCCH  = 12,
    ARCTH  = 13,

    EXP = 14, 

    SQRT = 15,
};

struct Variable
{
    char variable_name[MAX_LEN_STR];
    double variable_value;
};

struct Knot
{
    struct Knot* prev;

    struct Knot* right;
    struct Knot* left;

    enum TYPE type;
    union 
    {
        double value;
        char* variable;
        enum MATH_FUNC function;
        enum OPERATION_TYPE op_type;
    };
};

struct Tree 
{
    struct FileInfo* file_function;
    struct Text* buffer_function;

    struct Knot* root;

    size_t size;
    size_t code_error;

    FILE* tex;

    struct Variable array_variable [MAX_NUMBER_VARIEBLE];
    size_t variable_number;
};

void tree_ctor (struct Tree* tree);

void tree_dtor (struct Tree* tree);

void knot_dtor (struct Knot* current_knot);

Knot* knot_creater (Knot* prev, Knot* left, Knot* right, enum TYPE TYPE);

Knot* knot_op_creater (Knot* prev, Knot* left, Knot* right, enum OPERATION_TYPE OP);

Knot* knot_num_creater (Knot* prev, Knot* left, Knot* right, int value);

Knot* knot_func_creater (Knot* prev, Knot* left, Knot* right, enum MATH_FUNC FUNC);

Knot* knot_var_creater (Knot* prev, Knot* left, Knot* right, char* variable);

Knot* knot_clone (Knot* current_knot, Knot* prev);

Tree* tree_clone (Knot* current_knot);

Tree* all_tree_clone (Tree* tree);

Knot* all_knot_clone (Knot* current_knot);

//-----------------------------------------------------------------------------------

int tree_error (struct Tree* tree);

void knot_graph (struct Tree* tree, struct Knot* current_knot, FILE* tree_log_graph, int* count);

int tree_graph_dump (struct Tree* tree);

#endif // TREE_H