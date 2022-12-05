#include "Recursive_descent.h"

Knot* GetG (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);

    Knot* current_knot = GetE (str, tree);

    pass_space (str);

    if (**str != '\0')
    {
        tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE;
        printf ("syntax error: in Recursive_descent.cpp on %d, code_error = %lu", __LINE__, tree->code_error);
        abort ();
    }

    return current_knot;
}

Knot* GetE (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);

    Knot* current_knot = GetT (str, tree);

    while (**str == '+' || **str == '-')
    {
        char op = **str;
        (*str)++;

        Knot* value_knot = GetT (str, tree);

        Knot* operation_knot = knot_creater (nullptr, nullptr, nullptr, OPERATION);
        if (op == '+')
        {
            operation_knot->op_type = ADD;
        }
        else
        {
            operation_knot->op_type = SUB;
        }

        operation_knot->left  = current_knot;
        operation_knot->right = value_knot;

        current_knot->prev = operation_knot;
        value_knot->prev = operation_knot;

        current_knot = operation_knot;
    }

    pass_space (str);
    return current_knot;
}

Knot* GetT (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);
    Knot* current_knot = GetPow (str, tree);

    while (**str == '*' || **str == '/')
    {
        char op = **str;
        (*str)++;

        Knot* value_knot = GetPow (str, tree);

        Knot* operation_knot = knot_creater (nullptr, nullptr, nullptr, OPERATION);
        if (op == '*')
        {
            operation_knot->op_type = MUL;
        }
        else
        {
            operation_knot->op_type = DIV;
        }

        operation_knot->left  = current_knot;
        operation_knot->right = value_knot;

        current_knot->prev = operation_knot;
        value_knot->prev = operation_knot;

        current_knot = operation_knot;
    }

    pass_space (str);
    return current_knot;
}

Knot* GetPow (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);
    Knot* current_knot = GetP (str, tree);

    while (**str == '^')
    {
        char op = **str;
        (*str)++;

        Knot* indicator_degrees = GetP (str, tree);

        Knot* operation_knot = knot_creater (nullptr, nullptr, nullptr, OPERATION);
        if (op == '^')
        {
            operation_knot->op_type = POW;
        }

        operation_knot->left  = current_knot;
        operation_knot->right = indicator_degrees;

        current_knot->prev = operation_knot;
        indicator_degrees->prev = operation_knot;

        current_knot = operation_knot;
    }

    pass_space (str);
    return current_knot;
}

Knot* GetP (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);

    int unary_sign = is_unary_sign (str);

    Knot* current_knot = nullptr;

    if (**str == '(')
    {
        (*str)++;
        current_knot = GetE (str, tree);

        assert (**str == ')');

        (*str)++;
    }
    else
    {
        current_knot = GetFunc (str, tree);
    }

    if (unary_sign != 0)
    {
        if (unary_sign == UNARY_MINUS)
        {
            Knot* operation_knot = knot_creater (current_knot->prev, nullptr, current_knot, OPERATION);
            operation_knot->op_type = MUL;
            current_knot->prev = operation_knot;

            Knot* unary_sign_knot = knot_creater (operation_knot, nullptr, nullptr, NUMBER);
            unary_sign_knot->value = -1;
            operation_knot->left = unary_sign_knot;

            current_knot = operation_knot;
        }
    }

    pass_space (str);
    return current_knot;
}

Knot* GetFunc (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);

    Knot* current_knot = nullptr;

    int math_func = is_math_func (str);

    if (math_func != 0)
    {
        current_knot = GetP (str, tree);

        Knot* function_knot = knot_creater (current_knot->prev, nullptr, current_knot, FUNCTION);
        current_knot->prev = function_knot;
        
        switch (math_func)
        {
            #define DEF_FUNC(FUNC, func, func_tex, ...) case (FUNC): function_knot->function = FUNC; break;

            #include "code_generation.h"

            #undef DEF_FUNC

            default: 
                break;
        }

        current_knot = function_knot;
    }
    else
    {
        current_knot = GetV (str, tree);
    }

    pass_space (str);
    return current_knot;
}

Knot* GetV (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);

    Knot* current_knot = nullptr;

    if (isdigit(**str) == 0)
    {
        current_knot = knot_creater (nullptr, nullptr, nullptr, VARIABLE);
        current_knot->variable = (char*) calloc (MAX_LEN_STR, sizeof (char));

        int index = 0;
        while (isalpha (**str) != 0)
        {
            *(current_knot->variable + index) = **str;

            index++;
            (*str)++;
        }

        int number_variable = find_variable (tree, current_knot->variable);

        if (number_variable == -1)
        {
            strcpy (tree->array_variable[tree->variable_number].variable_name, current_knot->variable);
            tree->variable_number++;
        }
    }
    else
    {
        current_knot = GetN (str, tree);
    }

    pass_space (str);
    return current_knot;
}

Knot* GetN (char** str, struct Tree* tree)
{
    assert (str);
    assert (tree);

    pass_space (str);
    double value = 0;

    Knot* current_knot = knot_creater (nullptr, nullptr, nullptr, NUMBER);

    const char* s_old = *str;
    while ('0' <= **str && **str <= '9')
    {
        value = value*10 + **str - '0';
        (*str)++;
    }

    if (*str == s_old)
    {
        tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE;
        printf ("syntax error: in Recursive_descent.cpp on %d, code_error = %lu", __LINE__, tree->code_error);
        abort ();
    }

    current_knot->value = value;

    pass_space (str);
    return current_knot;
}

//==============================================

int is_unary_sign (char** str)
{
    assert (str);

    pass_space (str);

    if (**str == '-')
    {
        (*str)++;
        return UNARY_MINUS;
    }
    else if (**str == '+')
    {
        (*str)++;
        return UNARY_PLUS;
    }

    pass_space (str);
    return NO_UNARY_SIGN;
}

int is_math_func (char** str)
{
    assert (str);

    char math_func[250] = {0};

    char* str_old = *str;
    int index = 0;

    while (isalpha (**str) != 0)
    {
        math_func[index] = **str;

        index++;
        (*str)++;
    }

    #define DEF_FUNC(FUNC, func, func_tex, ...) if (stricmp (math_func, #FUNC) == 0)    \
                                                {                                       \
                                                    return FUNC;                        \
                                                }

    #include "code_generation.h"

    #undef DEF_FUNC

    *str = str_old;
    return 0;
}

void pass_space (char** str)
{
    assert (str);
    
    while (isspace (**str) != 0)
    {
        (*str)++;
    }
}

