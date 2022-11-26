#ifndef STACK_COMMON_H
#define STACK_COMMON_H

//#define NDEBUG

typedef struct Knot* Elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stack_common.h"
#include "common.h"

#define specifier_elem_t "%p"

const Elem_t NULL_SPECIFIER = nullptr;
const size_t HASH_START = 0xBEEF; 

enum STACK_ERROR {
    STACK_ERROR_POINTER_STRUCT_NULL        = 1 << 0,
    STACK_ERROR_POINTER_BUFFER_NULL        = 1 << 1,
    STACK_ERROR_SIZE_SMALLER_ZERO          = 1 << 2,
    STACK_ERROR_CAPACITY_SMALLER_ZERO      = 1 << 3,
    STACK_ERROR_SIZE_BIGGER_CAPACITY       = 1 << 4,
    STACK_ERROR_LEFT_CANARY_BUFFER_DEAD    = 1 << 5, 
    STACK_ERROR_RIGHT_CANARY_BUFFER_DEAD   = 1 << 6,
    STACK_ERROR_LEFT_CANARY_STRUCT_DEAD    = 1 << 7, 
    STACK_ERROR_RIGHT_CANARY_STRUCT_DEAD   = 1 << 8,
    STACK_ERROR_REALLOC_NULLPTR            = 1 << 9,
    STACK_ERROR_DATA_HASH                  = 1 << 10,
    STACK_ERROR_STACK_HASH                 = 1 << 11,
};

#ifndef NDEBUG
#define ASSERT_OK(stack)    do                                      \
                            {                                       \
                                if (StackError (stack) != 0)        \
                                {                                   \
                                    DecoderStackError (stack);      \
                                    StackDump (stack);              \
                                    abort();                        \
                                }                                   \
                            }  while (0)

#define FUNCTION_INFO(stack)    do                                                              \
                                {                                                               \
                                    stack->func_info.number_line_stack_cpp = __LINE__ - 2;      \
                                    stack->func_info.name_function_stack_cpp = __FUNCTION__;    \
                                } while (0)                                                     

#define PRINT_ERROR(code_of_error_programm, error)  if (code_of_error_programm & error)                        \
                                                    {                                                          \
                                                        fprintf (stack->func_info.log_file, "%s\n", #error);   \
                                                    }         
#else 
#define ASSERT_OK(stack)

#define FUNCTION_INFO(stack)                                                       

#define CHECK_ERROR(condition, error) 

#define PRINT_ERROR(code_of_error_programm, error)     
#endif


int StackDump (struct Stack* stack);

int StackError (struct Stack* stack);

int DecoderStackError (struct Stack* stack);

int GnuHashCalculate (char* first_byte, size_t number_bytes, size_t* hash_summ);

int StackHash (struct Stack* stack, size_t* hash_summ_returned);

int DataHash (struct Stack* stack, size_t* hash_summ_returned);

#endif // STACK_COMMON_H