#include "stack_common.h"
#include "stack.h"

//#define NDEBUG

int StackDump (struct Stack* stack)
{
    return GOOD_WORKING;
}

int StackError (struct Stack* stack)
{
#ifndef NDEBUG
    int pointer_stack_check_null = ((!stack) ? STACK_ERROR_POINTER_STRUCT_NULL : 0);

    if (pointer_stack_check_null == 0)
    {
        stack->code_of_error |= CHECK_ERROR (!stack->data,                               STACK_ERROR_POINTER_BUFFER_NULL);
        stack->code_of_error |= CHECK_ERROR (stack->size < 0,                            STACK_ERROR_SIZE_SMALLER_ZERO);
        stack->code_of_error |= CHECK_ERROR (stack->capacity < 0,                        STACK_ERROR_CAPACITY_SMALLER_ZERO);
        stack->code_of_error |= CHECK_ERROR (stack->size > stack->capacity,              STACK_ERROR_SIZE_BIGGER_CAPACITY);
        stack->code_of_error |= CHECK_ERROR (stack->left_struct_canary  != LEFT_CANARY,  STACK_ERROR_LEFT_CANARY_STRUCT_DEAD);
        stack->code_of_error |= CHECK_ERROR (stack->right_struct_canary != RIGHT_CANARY, STACK_ERROR_RIGHT_CANARY_STRUCT_DEAD);

        if (stack->code_of_error & STACK_ERROR_POINTER_BUFFER_NULL)
        {
            stack->code_of_error |= CHECK_ERROR (stack->result_realloc_pop == ERROR_REALLOC || stack->result_realloc_push == ERROR_REALLOC, STACK_ERROR_REALLOC_NULLPTR);
            stack->code_of_error |= CHECK_ERROR (*((size_t*)stack->data - 1) != LEFT_CANARY,                                                STACK_ERROR_LEFT_CANARY_BUFFER_DEAD);
            stack->code_of_error |= CHECK_ERROR (*((size_t*)(stack->data + stack->capacity)) != RIGHT_CANARY,                               STACK_ERROR_LEFT_CANARY_BUFFER_DEAD);
        }
    }

    return stack->code_of_error;

#else
    return GOOD_WORKING;
#endif
}

int DecoderStackError (struct Stack* stack)
{
#ifndef NDEBUG

    stack->func_info.log_file = fopen (stack->func_info.log_file_name, "ab");
    if (stack->func_info.log_file == nullptr)
    {
        return ERROR_FILE_OPEN;
    }

    PRINT_ERROR (stack->code_of_error, STACK_ERROR_POINTER_BUFFER_NULL);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_SIZE_SMALLER_ZERO);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_CAPACITY_SMALLER_ZERO);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_SIZE_BIGGER_CAPACITY);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_LEFT_CANARY_BUFFER_DEAD);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_RIGHT_CANARY_BUFFER_DEAD);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_LEFT_CANARY_STRUCT_DEAD);
    PRINT_ERROR (stack->code_of_error, STACK_ERROR_RIGHT_CANARY_STRUCT_DEAD);

    if (fclose (stack->func_info.log_file) < 0)
    {
        return ERROR_FILE_CLOSE;
    }

#endif

    return GOOD_WORKING;
}

int GnuHashCalculate (char* first_byte, size_t number_bytes, size_t* hash_summ)
{
    *hash_summ = HASH_START;

    for (int i = 0; i < number_bytes; i++)
    {
        *hash_summ *= 19;
        *hash_summ += *first_byte;
        first_byte++;
    }

    return GOOD_WORKING;
}

int StackHash (struct Stack* stack, size_t* hash_summ_returned)
{
    size_t hash_summ = HASH_START;
    size_t previous_stack_hash_summ = stack->stack_hash;

    size_t stack_size = sizeof (struct Stack);
    char* stack_byte = (char*) stack;

    stack->stack_hash = 0;

    int hash_calc = GnuHashCalculate (stack_byte, stack_size, &hash_summ);

    if (hash_summ_returned == nullptr)
    {
        stack->stack_hash = hash_summ;
    }
    else
    {
        stack->stack_hash = previous_stack_hash_summ;
        *hash_summ_returned = hash_summ;
    }

    return *hash_summ_returned;
}

int DataHash (struct Stack* stack, size_t* hash_summ_returned)
{
    size_t hash_summ = HASH_START;
    size_t previous_data_hash_summ = stack->data_hash;

    size_t data_size = sizeof (stack->data) * stack->capacity;
    char* data_byte = (char*) stack->data;

    stack->data_hash = 0;

    int hash_calc = GnuHashCalculate (data_byte, data_size, &hash_summ);

    if (hash_summ_returned == nullptr)
    {
        stack->data_hash = hash_summ;
    }
    else
    {
        stack->data_hash = previous_data_hash_summ;
        *hash_summ_returned = hash_summ;
    }

    return *hash_summ_returned;
}
