#include "stack.h"

int StackCtor (struct Stack* stack, size_t capacity)
{
    assert (stack);

#ifndef NDEBUG
    stack->left_struct_canary  = LEFT_CANARY;
    stack->right_struct_canary = RIGHT_CANARY;

    stack->data = (Elem_t*) calloc (1, capacity * sizeof(Elem_t) + 2 * sizeof (LEFT_CANARY));
    if (stack->data == nullptr)
    {
        return ERROR_CALLOC;
    }

    *((size_t*)(stack->data)) = LEFT_CANARY;
    stack->data = (Elem_t*)((size_t*)stack->data + 1);
    *((size_t*)(stack->data + capacity)) = RIGHT_CANARY;

#else
    stack->data = (Elem_t*) calloc (capacity, sizeof(Elem_t));
    if (stack->data == nullptr)
    {
        return ERROR_CALLOC;
    }
#endif

    for (int i = 0; i < capacity; i++)
    {
        stack->data[i] = NULL_SPECIFIER;
    }

    stack->capacity = capacity;
    stack->size = 0;
    stack->code_of_error = 0;

    ASSERT_OK (stack);

    return GOOD_WORKING;
}

void StackPush (struct Stack* stack, Elem_t value)
{   
    FUNCTION_INFO (stack);

    ASSERT_OK (stack);

    if (stack->size >= stack->capacity)
    {
        stack->result_realloc_push = StackRealloc (stack, stack->capacity*2);
        if (stack->result_realloc_push != ERROR_REALLOC)
        {
            stack->capacity *= 2;         
        }
        else 
        {
            stack->result_realloc_push = StackRealloc (stack, stack->capacity + STACK_HEIGHT);
            if (stack->result_realloc_push != ERROR_REALLOC)
            {
                stack->capacity += STACK_HEIGHT;         
            }
            else
            {
                ASSERT_OK (stack);
            }
        }
    }

    stack->data[stack->size++] = value;

    ASSERT_OK (stack);
}

Elem_t StackPop (struct Stack* stack)
{
    ASSERT_OK (stack);

    Elem_t element_pop = stack->data[--stack->size];
    stack->data[stack->size] = NULL_SPECIFIER;

    if (stack->size <= ((stack->capacity/2) - 1) && stack->capacity != 1)
    {
        stack->result_realloc_pop = StackRealloc (stack, stack->capacity/2);
        if (stack->result_realloc_pop != ERROR_REALLOC)
        {
            stack->capacity /= 2;         
        }
    }

    ASSERT_OK (stack);   

    return element_pop;
}

void StackDtor (struct Stack* stack)
{
    ASSERT_OK (stack);

#ifndef NDEBUG
    stack->left_struct_canary = DTOR_VALUE;
    stack->right_struct_canary = DTOR_VALUE;
    stack->func_info.name_function_stack_cpp = nullptr;
    stack->func_info.number_line_stack_cpp = DTOR_VALUE;

    if (((size_t*)stack->data - 1) != nullptr)
    {
        free (((size_t*)stack->data - 1));
    }
#else
    for (int i = 0; i < stack->capacity; i++)
    {  
        stack->data[i] = NULL_SPECIFIER;
    }
    free (stack->data);
#endif
    FillingWithPoisonStack (stack);
}

void FillingWithPoisonStack (struct Stack* stack)
{
    if (stack->data != nullptr)
    {
        stack->data = nullptr;
    }

    stack->size = DTOR_VALUE;
    stack->capacity = DTOR_VALUE;
    stack->code_of_error = DTOR_VALUE;

    stack->func_info.log_file = nullptr;
    stack->func_info.log_file_name = nullptr;
    stack->func_info.name_stack = nullptr;
    stack->func_info.number_line_stack_name_main = DTOR_VALUE;
}

int StackRealloc (struct Stack* stack, size_t new_capacity)
{
    ASSERT_OK (stack);

#ifndef NDEBUG

    Elem_t* help_variable = (Elem_t*) realloc ((char*)stack->data - sizeof(LEFT_CANARY), new_capacity * sizeof(Elem_t) + 2 * sizeof(LEFT_CANARY));
    
    if (help_variable == nullptr)
    {
        *((size_t*)(stack->data + stack->capacity)) = RIGHT_CANARY;
        return ERROR_REALLOC;
    }
    else
    {
        stack->data = help_variable;

        *((size_t*)(stack->data)) = LEFT_CANARY;
        stack->data = (Elem_t*)((size_t*)stack->data + 1);
        *((size_t*)(stack->data + new_capacity)) = RIGHT_CANARY;
    }  
#else
    Elem_t* help_variable = (Elem_t*) realloc (stack->data, new_capacity * sizeof(Elem_t));
    if (help_variable == nullptr)
    {
        return ERROR_REALLOC;
    }
    else
    {
        stack->data = help_variable;
    }
#endif

    for (int i = stack->size; i < new_capacity; i++)
    {
        stack->data[i] = NULL_SPECIFIER;
    }

    ASSERT_OK (stack);

    return GOOD_WORKING;
}


