#include "Recursive_descent.h"

const char* str_all;

Knot* GetG (const char* str)
{
    str_all = str;
    pass_space ();

    Knot* current_knot = GetE ();

    pass_space ();

    assert (*str_all == '\0');

    return current_knot;
}

Knot* GetE ()
{
    pass_space ();

    Knot* current_knot = GetT ();

    while (*str_all == '+' || *str_all == '-')
    {
        char op = *str_all;
        str_all++;

        Knot* value_knot = GetT ();

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

    pass_space ();
    return current_knot;
}

Knot* GetT ()
{
    pass_space ();
    Knot* current_knot = GetPow ();

    while (*str_all == '*' || *str_all == '/')
    {
        char op = *str_all;
        str_all++;

        Knot* value_knot = GetPow ();

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

    pass_space ();
    return current_knot;
}

Knot* GetPow ()
{
    pass_space ();
    Knot* current_knot = GetP ();

    while (*str_all == '^')
    {
        char op = *str_all;
        str_all++;

        Knot* indicator_degrees = GetP ();

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

    pass_space ();
    return current_knot;
}

Knot* GetP ()
{
    pass_space ();

    int unary_sign = is_unary_sign ();

    Knot* current_knot = nullptr;

    if (*str_all == '(')
    {
        str_all++;
        current_knot = GetE ();

        assert (*str_all == ')');

        str_all++;
    }
    else
    {
        current_knot = GetFunc ();
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

    pass_space ();
    return current_knot;
}

Knot* GetFunc ()
{
    pass_space ();

    Knot* current_knot = nullptr;

    int math_func = is_math_func ();

    if (math_func != 0)
    {
        current_knot = GetP ();

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
        current_knot = GetConst ();
    }

    pass_space ();
    return current_knot;
}

Knot* GetConst ()
{
    pass_space ();

    Knot* current_knot = nullptr;
    int math_const = is_math_const ();

    if (math_const != 0)
    {
        current_knot = knot_creater (nullptr, nullptr, nullptr, CONST);
        if (math_const == PI_CONST)
        {
            current_knot->const_val = PI_CONST;
        }
        else if (math_const == EXP_CONST)
        {
            current_knot->const_val = EXP_CONST;
        }
    }
    else
    {
        current_knot = GetV ();
    }

    pass_space ();

    return current_knot;
}

Knot* GetV ()
{
    pass_space ();

    Knot* current_knot = nullptr;

    if (isdigit(*str_all) == 0)
    {
        current_knot = knot_creater (nullptr, nullptr, nullptr, VARIABLE);
        current_knot->variable = (char*) calloc (MAX_LEN_STR, sizeof (char));

        int index = 0;
        while (isalpha (*str_all) != 0)
        {
            *(current_knot->variable + index) = *str_all;

            index++;
            str_all++;
        }
    }
    else
    {
        current_knot = GetN();
    }

    pass_space ();
    return current_knot;
}

Knot* GetN ()
{
    pass_space ();
    double value = 0;

    Knot* current_knot = knot_creater (nullptr, nullptr, nullptr, NUMBER);

    const char* s_old = str_all;
    while ('0' <= *str_all && *str_all <= '9')
    {
        value = value*10 + *str_all - '0';
        str_all++;
    }
    assert (str_all != s_old);

    current_knot->value = value;

    pass_space ();
    return current_knot;
}

//==============================================

int is_unary_sign ()
{
    pass_space ();

    if (*str_all == '-')
    {
        str_all++;
        return UNARY_MINUS;
    }
    else if (*str_all == '+')
    {
        str_all++;
        return UNARY_PLUS;
    }

    pass_space ();
    return NO_UNARY_SIGN;
}

int is_math_func ()
{
    char math_func[250] = {0};

    const char* str_old = str_all;
    int index = 0;

    while (isalpha (*str_all) != 0)
    {
        math_func[index] = *str_all;

        index++;
        str_all++;
    }

    if (strcmp (math_func, "sin") == 0)
    {
        return SIN;
    }
    else if (strcmp (math_func, "cos") == 0)
    {
        return COS;
    }
    else if (strcmp (math_func, "tg") == 0)
    {
        return TG;
    }
    else if (strcmp (math_func, "ctg") == 0)
    {
        return CTG;
    }
    else if (strcmp (math_func, "arcsin") == 0)
    {
        return ARCSIN;
    }
    else if (strcmp (math_func, "arccos") == 0)
    {
        return ARCCOS;
    }
    else if (strcmp (math_func, "arctg") == 0)
    {
        return ARCTG;
    }
    else if (strcmp (math_func, "arcctg") == 0)
    {
        return ARCCTG;
    }
    else if (strcmp (math_func, "ln") == 0)
    {
        return LN;
    }

    str_all = str_old;
    return 0;
}

int is_math_const ()
{
    char math_const[250] = {0};

    const char* str_old = str_all;
    int index = 0;

    while (isalpha (*str_all) != 0)
    {
        math_const[index] = *str_all;

        index++;
        str_all++;
    }

    if (strcmp (math_const, "pi") == 0)
    {
        return PI_CONST;
    }
    else if (strcmp (math_const, "exp") == 0)
    {
        return EXP_CONST;
    }

    str_all = str_old;
    return 0;
}

void pass_space ()
{
    while (isspace (*str_all) != 0)
    {
        str_all++;
    }
}

