#ifndef STACK_H
#define STACK_H

//#define NDEBUG

#include "stack_common.h"
#include "../enum.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const size_t LEFT_CANARY  = 0xDEADBEEF;
const size_t RIGHT_CANARY = 0xABADBABE;
const size_t DTOR_VALUE   = 0xBABADEDA;

const size_t STACK_HEIGHT = 5;

struct function_info {
    FILE* log_file;
    const char* log_file_name;

    const char* name_stack;
    size_t number_line_stack_name_main; 

#ifndef NDEBUG
    const char* name_function_stack_cpp; 
    size_t number_line_stack_cpp;
#endif
};

struct Stack {
#ifndef NDEBUG
    size_t left_struct_canary;
#endif
    Elem_t* data;
    size_t  size;
    size_t  capacity;
    unsigned int code_of_error;
    struct function_info func_info;
    int result_realloc_pop;
    int result_realloc_push;
    size_t stack_hash;
    size_t data_hash;
#ifndef NDEBUG
    size_t right_struct_canary;
#endif
};

int StackCtor (struct Stack* stack, size_t capacity);

void StackPush (struct Stack* stack, Elem_t value);

Elem_t StackPop (struct Stack* stack);

void StackDtor (struct Stack* stack);

void FillingWithPoisonStack (struct Stack* stack);

int StackRealloc (struct Stack* stack, size_t new_capacity);

#endif // STACK_H