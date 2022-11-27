#include "differentiator.h"

void start_programm (struct Tree* tree)
{
    tree_ctor (tree);
    simplification_tree (tree, tree->root);

    TEX_TEXT_PRINT(tree->tex, tree, "\\\\\\\\Ваша изначальная функция: \\\\");

    main_menu (tree);
}

void main_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Выберите, что хотите сдлеать с функцией, записанной в файле?\n");

    printf ("[" RED_TEXT(1) "] Узнать значение функции в точке\n");
    printf ("[" RED_TEXT(2) "] Взять проивзодную\n");
    printf ("[" RED_TEXT(3) "] Разложить по Маклорену\n");
    printf ("[" RED_TEXT(4) "] Разложить по Тейлеру\n");
    printf ("[" RED_TEXT(5) "] Узнать функцию касательной\n");
    printf ("[" RED_TEXT(6) "] Вывести дамп\n");
    printf ("[" RED_TEXT(7) "] Выйти из программы\n");

    processing_selected_mode (tree);
}

void processing_selected_mode (struct Tree* tree)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:  
            screen_clear ();
            menu_value_function_at_point (tree);
            break;
        case COMMAND_2: 
            screen_clear (); 
            menu_derivative (tree);
            break;
        case COMMAND_3: 
            screen_clear ();
            menu_maclaurin_decomposition (tree);
            break;
        case COMMAND_4:
            screen_clear ();

            break;
        case COMMAND_5:
            screen_clear ();
            menu_tangent_equation (tree);
            break;
        case COMMAND_6:
            tree_graph_dump (tree);
            graph_open (tree);
            screen_clear ();
            main_menu (tree);
            break;
        case COMMAND_7:
            tex_close (tree->tex);
            tree_dtor (tree);
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
    }
}

//---------------------------------------------------------------------------------

void menu_value_function_at_point (struct Tree* tree)
{
    screen_clear ();

    double point_value = 0;

    printf ("Введите значение точки:\t");
    scanf ("%lf", &point_value);

    clear_input ();

    Tree* tree_copy = value_function_at_point_counter (tree, point_value);

    TEX_TEXT_PRINT(tree->tex, tree_copy, "\\\\\\\\Значение вашей функции в точке %lf: \\\\", point_value);

    tree_dtor (tree_copy);

    main_menu (tree);
}

Tree* value_function_at_point_counter (struct Tree* tree, double point_value)
{
    Tree* tree_copy = tree_clone (tree->root);

    Stack* find_varieble = tree_search_varieble (tree_copy->root);

    while (find_varieble->size != 0)
    {
        Knot* varieble_knot = find_varieble->data[find_varieble->size - 1];
        free (varieble_knot->variable);
        varieble_knot->type = NUMBER;
        varieble_knot->value = point_value;

        StackDtor (find_varieble);
        if (find_varieble != nullptr)
        {
            free (find_varieble);
            find_varieble = nullptr;
        }

        find_varieble = tree_search_varieble (tree_copy->root);
    }

    simplification_tree (tree_copy, tree_copy->root);

    return tree_copy;
}

//---------------------------------------------------------------------------------

void menu_derivative (struct Tree* tree)
{
    screen_clear ();

    printf ("Производную какого порядка вы хотите посчитать?\t");

    int num = 0;
    scanf ("%d", &num);

    simplification_tree (tree, tree->root);

    for (int i = 0; i < num; i++)
    {
        tree->root = derivative (tree->root);
        simplification_tree (tree, tree->root);
        tree_graph_dump (tree);
        TEX_TEXT_PRINT(tree->tex, tree, "\\\\\\\\Очередная производная: \\\\");
    }

    clear_input ();
    screen_clear ();

    printf ("Хотите найти значение данной производной в функции?\n");

    printf ("[" RED_TEXT(1) "] Да\n");
    printf ("[" RED_TEXT(2) "] Нет\n");

    processing_selected_derivative_mode (tree);
}

void processing_selected_derivative_mode (struct Tree* tree)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:  
            screen_clear ();
            menu_value_function_at_point (tree);
            break;
        case COMMAND_2: 
            screen_clear (); 
            main_menu (tree);
            break;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
    }
}

Knot* derivative (struct Knot* current_knot)
{
    Tree* tree_copy = tree_clone (current_knot);
    switch (current_knot->type)
    {
        case OPERATION:
        {
            switch (current_knot->op_type)
            {
                case ADD:
                {
                    current_knot->left = derivative (tree_copy->root->left);
                    current_knot->right = derivative (tree_copy->root->right);
                    break;
                }
                case SUB:
                {
                    current_knot->left = derivative (tree_copy->root->left);
                    current_knot->right = derivative (tree_copy->root->right);
                    break;
                }
                case MUL:
                {
                    current_knot->op_type = ADD;

                    Knot* mul_knot_left = knot_creater (current_knot, nullptr, current_knot->right, OPERATION);
                    Knot* mul_knot_right = knot_creater (current_knot, nullptr, current_knot->left, OPERATION);

                    mul_knot_left->op_type = MUL;
                    mul_knot_right->op_type = MUL;

                    current_knot->left = mul_knot_left;
                    current_knot->right = mul_knot_right;

                    current_knot->right->prev = mul_knot_left;
                    current_knot->left->prev = mul_knot_right;

                    mul_knot_left->left = derivative (tree_copy->root->left);
                    mul_knot_right->left = derivative (tree_copy->root->right);

                    mul_knot_left->left->prev = mul_knot_left;
                    mul_knot_right->right->prev = mul_knot_right;
                    break;
                }
                case DIV:
                {
                    Knot* denominator = knot_creater (current_knot, nullptr, nullptr, OPERATION);
                    denominator->op_type = POW;

                    denominator->right = knot_creater (denominator, nullptr, nullptr, NUMBER);
                    denominator->right->value = 2;

                    denominator->left = all_knot_clone (current_knot->right);
                    current_knot->right->prev = denominator;

                    Knot* numerator = knot_creater (current_knot, nullptr, nullptr, OPERATION);
                    numerator->op_type = SUB;

                    numerator->left = knot_creater (numerator, nullptr, nullptr, OPERATION);
                    numerator->right = knot_creater (numerator, nullptr, nullptr, OPERATION);
                    numerator->left->op_type = MUL;
                    numerator->right->op_type = MUL;

                    numerator->left->left = derivative (tree_copy->root->left);
                    numerator->left->right = current_knot->right;

                    numerator->right->left = derivative (tree_copy->root->right);
                    numerator->right->right = current_knot->left;

                    current_knot->left = numerator;
                    current_knot->right = denominator;
                    break;
                }
                case POW:
                {
                    Stack* find_varieble_left = tree_search_varieble (current_knot->left);
                    Stack* find_varieble_right = tree_search_varieble (current_knot->right);
                    // ОЧИСТКА СТЕКА

                    if (find_varieble_left->size == 0 && find_varieble_right->size == 0)
                    {
                        current_knot = knot_creater (current_knot->prev, nullptr, nullptr, NUMBER);
                        current_knot->value = 0;
                    }
                    else if (find_varieble_left->size != 0 && find_varieble_right->size == 0)
                    {
                        current_knot->op_type = MUL;

                        Knot* left_knot = current_knot->right;

                        Knot* right_knot = knot_creater (current_knot, nullptr, nullptr, OPERATION);
                        right_knot->op_type = MUL;
                        right_knot->left = derivative (tree_copy->root->left);

                        right_knot->right = knot_creater (right_knot, current_knot->left, nullptr, OPERATION);
                        right_knot->right->op_type = POW;

                        right_knot->right->right = knot_creater (right_knot->right, nullptr, nullptr, OPERATION);
                        right_knot->right->right->op_type = SUB;

                        right_knot->right->right->left = all_knot_clone (current_knot->right);
                        right_knot->right->right->right = knot_creater (right_knot->right->right, nullptr, nullptr, NUMBER);
                        right_knot->right->right->right->value = 1;

                        current_knot->left = left_knot;
                        current_knot->right = right_knot;
                    }
                    else if (find_varieble_left->size == 0 && find_varieble_right->size != 0)
                    {   
                        Knot* right_knot = all_knot_clone (current_knot);
                        Knot* left_knot = knot_creater (current_knot, nullptr, derivative (tree_copy->root->right), OPERATION);
                        left_knot->op_type = MUL;

                        left_knot->left = knot_creater (left_knot, nullptr, all_knot_clone (current_knot->left), FUNCTION);
                        left_knot->left->function = LN;

                        current_knot->op_type = MUL;

                        current_knot->left = left_knot;
                        current_knot->right = right_knot;
                    }
                    else
                    {
                        Knot* right_knot = all_knot_clone (current_knot);

                        Knot* left_knot = knot_creater (current_knot, nullptr, all_knot_clone (current_knot->right), OPERATION);
                        left_knot->op_type = MUL;
                        left_knot->left = knot_creater (left_knot, nullptr, all_knot_clone (current_knot->left), FUNCTION);
                        left_knot->left->function = LN;

                        left_knot = derivative (left_knot);

                        current_knot->op_type = MUL;

                        current_knot->left = left_knot;
                        current_knot->right = right_knot;
                    }
                    break;
                }
                default:
                {

                }
            }
            break;
        }
        case NUMBER:
        {
            current_knot->value = 0;
            break;
        }
        case VARIABLE:
        {
            free (current_knot->variable);
            current_knot->type = NUMBER;
            current_knot->value = 1;
            break;
        }
        case FUNCTION:
        {
            switch (current_knot->function)
            {
                case (SIN):
                {
                    break;
                }
                case (COS):
                {
                    break;
                }
                case (TG):
                {
                    break;
                }
                case (CTG):
                {
                    break;
                }
                case (LN):
                {
                    current_knot->type = OPERATION;
                    current_knot->op_type = DIV;
                    current_knot->left = derivative (tree_copy->root->right);
                }
                default:
                {

                }
            }
            break;
        }
        case CONST:
        {
            break;
        }
        default:
        {

        }
    }
    return current_knot;
}

//---------------------------------------------------------------------------------

void menu_maclaurin_decomposition (struct Tree* tree)
{
    int order = 0;

    printf ("До какого порядка вы хотите разложить вашу функцию? ");
    scanf ("%d", &order);

    clear_input ();

    maclaurin_decomposition (tree, order);

    simplification_tree (tree, tree->root);

    TEX_TEXT_PRINT (tree->tex, tree, "\\\\\\\\Разложение вашей функции по Маклорену до %d порядка: \\\\", order);

    fprintf (tree->tex, "+ {o({x}^{%d})}", order);

    main_menu (tree);
}

void maclaurin_decomposition (struct Tree* tree, int order)
{
    Tree* tree_copy = tree_clone (tree->root);
    Tree* tree_value_zero = value_function_at_point_counter (tree, 0);
    knot_dtor (tree->root);
    tree->root = tree_value_zero->root;

    for (int i = 1; i <= order; i++)
    {
        tree_copy->root = derivative (tree_copy->root);
        Tree* tree_copy_point_zero = value_function_at_point_counter (tree_copy, 0);

        Knot* add_knot = knot_creater (tree->root->prev, tree->root, nullptr, OPERATION);
        add_knot->op_type = ADD;

        Knot* div_knot = knot_creater (add_knot, nullptr, nullptr, OPERATION);
        div_knot->op_type = DIV;
        add_knot->right = div_knot;

        Knot* mul_knot = knot_creater (div_knot, tree_copy_point_zero->root, nullptr, OPERATION);
        mul_knot->op_type = MUL;
        div_knot->left = mul_knot;
        tree_copy_point_zero->root->prev = mul_knot;

        Knot* pow_knot = knot_creater(mul_knot, nullptr, nullptr, OPERATION);
        pow_knot->op_type = POW;
        mul_knot->right = pow_knot;

        Knot* factorial_knot = knot_creater (div_knot, nullptr, nullptr, NUMBER);
        factorial_knot->value = factorial (i);
        div_knot->right = factorial_knot;

        Knot* varieble_knot = knot_creater (pow_knot, nullptr, nullptr, VARIABLE);
        pow_knot->left = varieble_knot;
        //заполнить имя переменной нормально
        varieble_knot->variable = (char*) calloc (MAX_LEN_STR, sizeof(char));
        strcpy (varieble_knot->variable, "x");
        //заполнить имя переменной нормально

        Knot* pow_degree_knot = knot_creater (pow_knot, nullptr, nullptr, NUMBER);
        pow_degree_knot->value = i;
        pow_knot->right = pow_degree_knot;

        tree->root->prev = add_knot;
        tree->root = add_knot;
    }

    tree_dtor (tree_copy);
}

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

void menu_tangent_equation (struct Tree* tree)
{
    screen_clear ();

    double point_value = 0;

    printf ("Введите значение точки, в которой хотите найти уравнение касательной для вашей функции: ");
    scanf ("%lf", &point_value);

    clear_input ();

    tangent_equation_at_point (tree, point_value);
    TEX_TEXT_PRINT (tree->tex, tree, "\\\\\\\\Уравнение касательная к вашей функции в точке %lf: \\\\", point_value);

    main_menu (tree);
}

void tangent_equation_at_point (struct Tree* tree, double point_value)
{
    Tree* tree_value_point = value_function_at_point_counter (tree, point_value);
    tree->root = derivative (tree->root);
    Tree* tree_derivative_value_point = value_function_at_point_counter (tree, point_value);

    knot_dtor (tree->root);
    tree->root = tree_derivative_value_point->root;

    Knot* add_knot = knot_creater (tree_value_point->root->prev, tree_value_point->root, nullptr, OPERATION);
    add_knot->op_type = ADD;
    tree_value_point->root->prev = add_knot;

    Knot* mul_knot = knot_creater (add_knot, tree->root, nullptr, OPERATION);
    mul_knot->op_type = MUL;
    add_knot->right = mul_knot;
    tree->root->prev = mul_knot;

    Knot* sub_knot = knot_creater (mul_knot, nullptr, nullptr, OPERATION);
    sub_knot->op_type = SUB;
    mul_knot->right = sub_knot;

    sub_knot->right = knot_creater (sub_knot, nullptr, nullptr, NUMBER);
    sub_knot->right->value = point_value;

    sub_knot->left = knot_creater (sub_knot, nullptr, nullptr, VARIABLE);
    //заполнить имя переменной нормально
    sub_knot->left->variable = (char*) calloc (MAX_LEN_STR, sizeof(char));
    strcpy (sub_knot->left->variable, "x");
    //заполнить имя переменной нормально

    tree->root = add_knot;

    simplification_tree (tree, tree->root);
}

//---------------------------------------------------------------------------------

int get_command ()
{
    struct termios oldt = {};
    struct termios newt = {};

    int command = 0;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    command = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return command;
}

void graph_open (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int number_graph = tree_graph_dump (tree);
    char command [MAX_STR_SIZE] = "";
    snprintf (command, MAX_STR_SIZE, "open graph/graph_log_tree_%d.png", number_graph);
    system (command);
}

//---------------------------------------------------------------------------------

void simplification_tree (struct Tree* tree, struct Knot* current_knot)
{
    convolution_const (tree, current_knot);
    neutralization (tree, current_knot);
}

void convolution_const (struct Tree* tree, struct Knot* current_knot)
{
    if (!type_knot(current_knot))
    {
        if (current_knot->left != nullptr) 
        {
            convolution_const (tree, current_knot->left);
        }
        if (current_knot->right != nullptr) 
        {
            convolution_const (tree, current_knot->right);
        }
    }
    else
    {
        double value = calculate_knot (current_knot);

        struct Knot* equal = knot_creater (current_knot->prev, nullptr, nullptr, NUMBER);
        equal->value = value;
        
        if (current_knot == tree->root) 
        {
            tree->root = equal;
        }
        else
        {
            if (current_knot->prev->left == current_knot) 
            {
                current_knot->prev->left = equal;
            }
            if (current_knot->prev->right == current_knot) 
            {
                current_knot->prev->right = equal;
            }
        }
    }
}

void neutralization (struct Tree* tree, struct Knot* current_knot)
{
    if (current_knot != nullptr)
    {
        if (current_knot->left != nullptr) 
        {
            neutralization (tree, current_knot->left);
        }
        if (current_knot->right != nullptr) 
        {
            neutralization (tree, current_knot->right);
        }
    }
    else
    {
        return;
    }

    if (current_knot->type == OPERATION)
    {
        switch (current_knot->op_type)
        {
            case ADD:
            case SUB:
            {
                if (current_knot->left != nullptr)
                {
                    if ((current_knot->left->type == NUMBER) && current_knot->left->value == 0)
                    {
                        relinking_subtree(tree, current_knot, current_knot->right, current_knot->left);
                        return;
                    }
                }
                if (current_knot->right != nullptr)
                {
                    if ((current_knot->right->type == NUMBER) && current_knot->right->value == 0)
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }
                break;
            }
            case MUL:
            {
                if (current_knot->left != nullptr)
                {
                    if ((current_knot->left->type == NUMBER) && current_knot->left->value == 0)
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }
                if (current_knot->right != nullptr)
                {
                    if ((current_knot->right->type == NUMBER) && current_knot->right->value == 0)
                    {
                        relinking_subtree(tree, current_knot, current_knot->right, current_knot->left);
                        return;
                    }
                }
                if (current_knot->left != nullptr)
                {
                    if ((current_knot->left->type == NUMBER) && current_knot->left->value == 1)
                    {
                        relinking_subtree(tree, current_knot, current_knot->right, current_knot->left);
                        return;
                    }
                }
                if (current_knot->right != nullptr)
                {
                    if ((current_knot->right->type == NUMBER) && current_knot->right->value == 1)
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }
                break;
            }
            
            case DIV:
            {   
                if (current_knot->left != nullptr)
                {
                    if ((current_knot->left->type == NUMBER) && current_knot->left->value == 0)
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }

                if (current_knot->right != nullptr)
                {
                    if ((current_knot->right->type == NUMBER) && current_knot->right->value == 1)
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }

                break;
            }
            case POW:
            {
                if (current_knot->left != nullptr)
                {
                    if ((current_knot->left->type == NUMBER) && (current_knot->left->value == 0))
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                    if ((current_knot->left->type == NUMBER) && (current_knot->left->value == 1))
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }
                if (current_knot->right != nullptr)
                {
                    if ((current_knot->right->type == NUMBER) && (current_knot->right->value == 0))
                    {
                        current_knot->right->value = 1;
                        relinking_subtree(tree, current_knot, current_knot->right, current_knot->left);
                        return;
                    }
                    if ((current_knot->right->type == NUMBER) && (current_knot->right->value == 1))
                    {
                        relinking_subtree(tree, current_knot, current_knot->left, current_knot->right);
                        return;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else if (current_knot->type == FUNCTION)
    {
        switch (current_knot->function)
        {
            case LN:
            case ARCCOS:
            {
                if ((current_knot->right->type == NUMBER) && current_knot->right->value == 1)
                {
                    current_knot->type = NUMBER;
                    current_knot->value = 0;
                    return;
                }
                break;
            }
            case SIN:
            case TG:
            case ARCSIN:
            case ARCTG:
            {
                if ((current_knot->right->type == NUMBER) && current_knot->right->value == 0)
                {
                    current_knot->type = NUMBER;
                    current_knot->value = 0;
                    return;
                }
                break;
            }
            case COS:
            {
                if ((current_knot->right->type == NUMBER) && current_knot->right->value == 0)
                {
                    current_knot->type = NUMBER;
                    current_knot->value = 1;
                    return;
                }
                break;
            }
            default: 
                break;
        }
    }
}

void relinking_subtree (struct Tree* tree, struct Knot* current_knot, struct Knot* subtree_linking, struct Knot* subtree_delete)
{
    if (current_knot == tree->root) 
    {
        tree->root = subtree_linking;
        subtree_linking->prev = nullptr;
    }
    else
    {
        subtree_linking->prev = current_knot->prev;

        if (current_knot->prev != nullptr && current_knot->prev->left == current_knot) 
        {
            current_knot->prev->left = subtree_linking;
        }
        if (current_knot->prev != nullptr && current_knot->prev->right == current_knot) 
        {
            current_knot->prev->right = subtree_linking;
        }
    }
}

int type_knot (struct Knot* current_knot)
{
    if (current_knot == nullptr) 
    {
        return 0;
    }

    if (current_knot->type == VARIABLE)       
    {
        return 0;
    }
    if (current_knot->type == NUMBER) 
    {
        return 1;
    }
    if (current_knot->type == FUNCTION)   
    {
        return 0;
    }
    if (current_knot->type == OPERATION)   
    {
        return type_knot (current_knot->left) * type_knot(current_knot->right);
    }

    return 0;   
}

double calculate_knot (struct Knot* current_knot)
{   
    assert(current_knot != nullptr);

    switch (current_knot->type)
    {
        case NUMBER: 
            return current_knot->value;
        case OPERATION:
            switch (current_knot->op_type)
            {
                case ADD: return      calculate_knot (current_knot->left) + calculate_knot (current_knot->right);
                case SUB: return      calculate_knot (current_knot->left) - calculate_knot (current_knot->right);
                case MUL: return      calculate_knot (current_knot->left) * calculate_knot (current_knot->right);
                case DIV: return      calculate_knot (current_knot->left) / calculate_knot (current_knot->right);     
                case POW: return pow (calculate_knot (current_knot->left) , calculate_knot (current_knot->right));
                default:
                    break;
            }
            break;

        case FUNCTION:
            switch (current_knot->function)
            {
                case LN:  return     log(calculate_knot (current_knot->right));
                case SIN: return     sin(calculate_knot (current_knot->right));
                case COS: return     cos(calculate_knot (current_knot->right));
                case TG:  return     tan(calculate_knot (current_knot->right));
                case CTG: return 1 / tan(calculate_knot (current_knot->right));
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return 0;
}

//---------------------------------------------------------------------------------

struct Stack* tree_search_varieble (struct Knot* current_knot)
{
    //ASSERT_OK_TREE (tree);

    struct Stack* path_element = (struct Stack*) calloc (1, sizeof(Stack));

    StackCtor (path_element, 10);

    knot_varieble_search(current_knot, path_element);

    //ASSERT_OK_TREE (tree);
    
    return path_element;
}

int knot_varieble_search (struct Knot* current_knot, struct Stack* path_element)
{
    assert (current_knot);
    assert (path_element);

    StackPush(path_element, current_knot);

    if (current_knot->type == VARIABLE) 
    {
        return 1;
    }
    else
    {
        if (current_knot->left != nullptr && current_knot->right != nullptr)
        {
            if (knot_varieble_search(current_knot->left, path_element) || knot_varieble_search (current_knot->right, path_element))
            {
                return 1;
            }
            else
            {
                StackPop(path_element);
                return 0;
            }
        }
        else if (current_knot->left == nullptr && current_knot->right == nullptr)
        {
            StackPop(path_element);
            return 0;
        }
        else if (current_knot->left == nullptr && current_knot->right != nullptr)
        {
            if (knot_varieble_search (current_knot->right, path_element))
            {
                return 1;
            }
            else
            {
                StackPop (path_element);
                return 0;
            }
        }

        return 0;
    }
}

//---------------------------------------------------------------------------------

FILE* tex_open ()
{
    FILE* tex = fopen ("tex/diff.tex", "wb");

    if (tex == nullptr)
    {
        printf ("ERROR FOPEN in line %d in differentiator.cpp", __LINE__);
        abort ();
    }

    fprintf (tex,   "\\documentclass[12pt,a4paper]{scrartcl}\n"
                    "\\usepackage[utf8]{inputenc}\n"
                    "\\usepackage[english,russian]{babel}\n"
                    "\\usepackage{indentfirst}\n"
                    "\\usepackage{misccorr}\n"
                    "\\usepackage{graphicx}\n"
                    "\\usepackage{amsmath}\n"
                    "\\begin{document}\n");

    return tex;
}

void tex_tree_print (FILE* tex, struct Tree* tree, char* text)
{
    assert (tex);
    assert (tree);
    assert (text);

    fprintf (tex, "\n");
    fprintf (tex, "%s", text);
    fprintf (tex, "\n");

    fprintf (tex, "\\\\{$");
    tree_print (tex, tree->root);
    fprintf (tex, "$}");
}

void tree_print (FILE* tex, struct Knot* knot)
{
    fprintf (tex, "{");
    if (knot->type != NUMBER && knot->type != VARIABLE)
    {
        fprintf (tex, "(");
    }

    if (!knot)
    {
        return;
    }
    if (knot->type == OPERATION)
    {
        if (knot->op_type == DIV)
        {
            fprintf (tex, "\\frac");
        }
    }

    if (knot->left)
    {
        tree_print (tex, knot->left);
    }

    if (knot->type == NUMBER)
    {
        if (knot->value == (int)knot->value)
        {
            fprintf (tex, "%d", (int)knot->value);
        }
        else
        {
            fprintf (tex, "%lf", knot->value);
        }
    }
    else if (knot->type == VARIABLE)
    {
        fprintf (tex, "%s", knot->variable);
    }
    else if (knot->type == OPERATION)
    {
        switch (knot->op_type)
        {
            case (ADD): fprintf (tex, "+"); break;
            case (SUB): fprintf (tex, "-"); break;
            case (MUL): fprintf (tex, "*"); break;
            case (POW): fprintf (tex, "^"); break;
            case (DIV):                     break;
            default:
            {

            }
        }
    }
    else if (knot->type == FUNCTION)
    {
        switch (knot->function)
        {
            case (SIN): fprintf (tex, "sin"); break;
            case (COS): fprintf (tex, "cos"); break;
            case (TG):  fprintf (tex, "tg");  break;
            case (CTG): fprintf (tex, "ctg"); break;
            case (LN):  fprintf (tex, "ln");  break;
            default:
            {

            }
        }
    }
    
    if (knot->right)
    {
        tree_print (tex, knot->right);
    }

    if (knot->type != NUMBER && knot->type != VARIABLE)
    {
        fprintf (tex, ")");
    }
    fprintf (tex, "}");
}

void tex_close (FILE* tex)
{
    assert (tex);

    fprintf (tex, "\n\n\\end{document}");
    
    fclose (tex);
}