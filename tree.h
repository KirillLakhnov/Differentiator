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

const size_t MAX_LEN_STR = 300;
const size_t DTOR_SIZE_T = 0xBABADEDA;
const int DTOR_INT = 0xBABADEAD;

enum TREE_ERROR
{
    TREE_ERROR_SYNTAX_IN_BASE         = 1 << 0,
    TREE_WRONG_NAME_DATA_BASE         = 1 << 1,
    TREE_ERROR_POINTER_STRUCT_NULL    = 1 << 2,
    TREE_ERROR_POINTER_ROOT_NULL      = 1 << 3,
    TREE_ERROR_POINTER_BUFFER_NULL    = 1 << 4,
    TREE_ERROR_POINTER_FILE_INFO_NULL = 1 << 5,
    TREE_ERROR_SIZE_SMALLER_ZERO      = 1 << 6,

    TREE_ERROR_CALLOC         = 1 << 3,
    TREE_ERROR_STRCPY         = 1 << 4,
    TREE_ERROR_BUFFER_CREATER = 1 << 5,
};

enum TYPE {
    OPERATION   = 1,
    NUMBER      = 2,
    VARIABLE    = 3,
    FUNCTION    = 4,
    CONST       = 5,
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
    CTG = 3,
    TG  = 4,
    
    ARCSIN = 5,
    ARCCOS = 6,
    ARCCTG = 7,
    ARCTG  = 8,

    LN = 9,
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
        enum MATH_CONST const_val;
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
};

void tree_ctor (struct Tree* tree);

void tree_dtor (struct Tree* tree);

void knot_dtor (struct Knot* current_knot);

Knot* knot_creater (Knot* prev, Knot* left, Knot* right, enum TYPE type);

Knot* knot_clone (Knot* current_knot, Knot* prev);

Tree* tree_clone (Knot* current_knot);

Knot* all_knot_clone (Knot* current_knot);

//-----------------------------------------------------------------------------------

int tree_error (struct Tree* tree);

void knot_graph (struct Tree* tree, struct Knot* current_knot, FILE* tree_log_graph, int* count);

int tree_graph_dump (struct Tree* tree);

#endif // TREE_H