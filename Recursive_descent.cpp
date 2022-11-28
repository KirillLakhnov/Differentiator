#include "Recursive_descent.h"

Knot* GetG (char** str)
{
    pass_space (str);

    Knot* current_knot = GetE (str);

    pass_space (str);

    assert (**str == '\0');

    return current_knot;
}

Knot* GetE (char** str)
{
    pass_space (str);

    Knot* current_knot = GetT (str);

    while (**str == '+' || **str == '-')
    {
        char op = **str;
        (*str)++;

        Knot* value_knot = GetT (str);

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

Knot* GetT (char** str)
{
    pass_space (str);
    Knot* current_knot = GetPow (str);

    while (**str == '*' || **str == '/')
    {
        char op = **str;
        (*str)++;

        Knot* value_knot = GetPow (str);

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

Knot* GetPow (char** str)
{
    pass_space (str);
    Knot* current_knot = GetP (str);

    while (**str == '^')
    {
        char op = **str;
        (*str)++;

        Knot* indicator_degrees = GetP (str);

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

Knot* GetP (char** str)
{
    pass_space (str);

    int unary_sign = is_unary_sign (str);

    Knot* current_knot = nullptr;

    if (**str == '(')
    {
        (*str)++;
        current_knot = GetE (str);

        assert (**str == ')');

        (*str)++;
    }
    else
    {
        current_knot = GetFunc (str);
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

Knot* GetFunc (char** str)
{
    pass_space (str);

    Knot* current_knot = nullptr;

    int math_func = is_math_func (str);

    if (math_func != 0)
    {
        current_knot = GetP (str);

        Knot* function_knot = knot_creater (current_knot->prev, nullptr, current_knot, FUNCTION);
        current_knot->prev = function_knot;
        
        if (math_func == SIN)
        {
            function_knot->function = SIN;
        }
        else if (math_func == COS)
        {
            function_knot->function = COS;
        }
        else if (math_func == TG)
        {
            function_knot->function = TG;
        }
        else if (math_func == CTG)
        {
            function_knot->function = CTG;
        }
        else if (math_func == ARCSIN)
        {
            function_knot->function = ARCSIN;
        }
        else if (math_func == ARCCOS)
        {
            function_knot->function = ARCCOS;
        }
        else if (math_func == ARCTG)
        {
            function_knot->function = ARCTG;
        }
        else if (math_func == ARCCTG)
        {
            function_knot->function = ARCCTG;
        }
        else if (math_func == LN)
        {
            function_knot->function = LN;
        }

        current_knot = function_knot;
    }
    else
    {
        current_knot = GetV (str);
    }

    pass_space (str);
    return current_knot;
}

Knot* GetV (char** str)
{
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
    }
    else
    {
        current_knot = GetN(str);
    }

    pass_space (str);
    return current_knot;
}

Knot* GetN (char** str)
{
    pass_space (str);
    double value = 0;

    Knot* current_knot = knot_creater (nullptr, nullptr, nullptr, NUMBER);

    const char* s_old = *str;
    while ('0' <= **str && **str <= '9')
    {
        value = value*10 + **str - '0';
        (*str)++;
    }
    assert (*str != s_old);

    current_knot->value = value;

    pass_space (str);
    return current_knot;
}

//==============================================

int is_unary_sign (char** str)
{
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
    char math_func[250] = {0};

    char* str_old = *str;
    int index = 0;

    while (isalpha (**str) != 0)
    {
        math_func[index] = **str;

        index++;
        (*str)++;
    }

    IS_FUNCTION (math_func, SIN);
    IS_FUNCTION (math_func, COS);
    IS_FUNCTION (math_func, TG);
    IS_FUNCTION (math_func, CTG);
    IS_FUNCTION (math_func, ARCSIN);
    IS_FUNCTION (math_func, ARCCOS);
    IS_FUNCTION (math_func, ARCTG);
    IS_FUNCTION (math_func, ARCCTG);
    IS_FUNCTION (math_func, LN);
    IS_FUNCTION (math_func, SH);
    IS_FUNCTION (math_func, CH);
    IS_FUNCTION (math_func, TH);
    IS_FUNCTION (math_func, CTH);
    IS_FUNCTION (math_func, ARCSH);
    IS_FUNCTION (math_func, ARCCH);
    IS_FUNCTION (math_func, ARCTH);
    IS_FUNCTION (math_func, EXP);

    *str = str_old;
    return 0;
}

void pass_space (char** str)
{
    while (isspace (**str) != 0)
    {
        (*str)++;
    }
}

