#include "differentiator.h"

void start_programm (struct Tree* tree)
{
    tree_ctor (tree);
    simplification_tree (tree, tree->root);

    TEX_TEXT_PRINT(tree->tex, tree, "\\\\\\\\\\section{Ваша изначальная функция:} \\\\");

    main_menu (tree);
}

void main_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Выберите, что хотите сделать с функцией, записанной в файле?\n");

    printf ("[" RED_TEXT(1) "] Узнать значение функции в точке\n");
    printf ("[" RED_TEXT(2) "] Взять проивзодную\n");
    printf ("[" RED_TEXT(3) "] Разложить по Тейлеру\n");
    printf ("[" RED_TEXT(4) "] Узнать функцию касательной\n");
    printf ("[" RED_TEXT(5) "] Построить график функции\n");
    printf ("[" RED_TEXT(6) "] Полное исследование функции\n");
    printf ("[" RED_TEXT(7) "] Вывести дамп\n");
    printf ("[" RED_TEXT(8) "] Выйти из программы\n");

    processing_selected_mode (tree);
}

void processing_selected_mode (struct Tree* tree)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:  
            screen_clear ();
            menu_value_function_at_point (tree, 1);
            break;
        case COMMAND_2: 
            screen_clear (); 
            menu_derivative (tree, 1);
            break;
        case COMMAND_3:
            screen_clear ();
            menu_taylor_decomposition (tree, 1);
            break;
        case COMMAND_4:
            screen_clear ();
            menu_tangent_equation (tree, 1);
            break;
        case COMMAND_5:
            screen_clear ();
            menu_graph_function (tree, 1);
            break;
        case COMMAND_6:
            menu_full_function_study (tree);
            break;
        case COMMAND_7:
            tree_graph_dump (tree);
            graph_open (tree);
            screen_clear ();
            main_menu (tree);
            break;
        case COMMAND_8:
            tex_close (tree->tex);
            tree_dtor (tree);
            system ("pdflatex tex/diff.tex");
            //system ("open diff.pdf");
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
    }
}

//---------------------------------------------------------------------------------

void menu_value_function_at_point (struct Tree* tree, int operating_mode)
{
    screen_clear ();

    double point_value = 0;

    Tree* tree_copy = all_tree_clone (tree);

    for (int i = 0; i < tree->variable_number; i++)
    {
        screen_clear ();

        printf ("Хотите учитывать значение переменной \"%s\"?\n", tree_copy->array_variable[i].variable_name);

        printf ("[" RED_TEXT(1) "] Да\n");
        printf ("[" RED_TEXT(2) "] Нет\n");

        processing_selected_value_at_point_mode (tree, i);

        clear_input ();
    }

    for (int i = 0; i < tree->variable_number; i++)
    {
        if (!(tree->array_variable[i].variable_value != tree->array_variable[i].variable_value))
        {
            tree_copy = value_function_at_point_counter (tree_copy, tree->array_variable[i]);
        }
    }

    fprintf (tree->tex, "\n\\section{Значение функции в точке.}");
    fprintf (tree->tex, "\n\\\\\\\\ \\noindent \\textbf {Значения переменных при рассчете значения функции:}\n");
    fprintf (tree->tex, "\\begin{enumerate}\n");

    for (int i = 0; i < tree->variable_number; i++)
    {
        if (!(tree->array_variable[i].variable_value != tree->array_variable[i].variable_value))
        {
            char* var_name = tree->array_variable[i].variable_name;
            double var_value = tree->array_variable[i].variable_value;

            fprintf (tree->tex, "\t\\item %s = %lf\n",  var_name,
                                                        var_value == (int)var_value ? (int)var_value : var_value);
        }
    }

    fprintf (tree->tex, "\\end{enumerate}\n");

    TEX_TEXT_PRINT(tree->tex, tree_copy, "\\noindent \\textbf {Значение функции в заданной точке:}\\\\");

    tree_dtor (tree_copy);

    if (operating_mode == 1)
    {
        main_menu (tree);
    }
}

void processing_selected_value_at_point_mode (struct Tree* tree, int counter)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:
            screen_clear ();
            printf ("Введите значение переменной \"%s\": ", tree->array_variable[counter].variable_name);
            scanf ("%lf", &tree->array_variable[counter].variable_value);
            break;
        case COMMAND_2: 
            break;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_value_at_point_mode (tree, counter);
    }
}

Tree* value_function_at_point_counter (struct Tree* tree, struct Variable variable)
{
    Tree* tree_copy = tree_clone (tree->root);

    Stack* find_varieble = tree_search_variable (tree_copy->root, variable);

    while (find_varieble->size != 0)
    {
        Knot* varieble_knot = find_varieble->data[find_varieble->size - 1];
        free (varieble_knot->variable);
        varieble_knot->type = NUMBER;
        varieble_knot->value = variable.variable_value;

        StackDtor (find_varieble);
        if (find_varieble != nullptr)
        {
            free (find_varieble);
            find_varieble = nullptr;
        }

        find_varieble = tree_search_variable (tree_copy->root, variable);
    }

    simplification_tree (tree_copy, tree_copy->root);

    return tree_copy;
}

//---------------------------------------------------------------------------------

void menu_derivative (struct Tree* tree, int operating_mode)
{
    screen_clear ();

    int number_variable = 0;

    if (tree->variable_number > 1)
    {
        printf ("Производную от какой переменной вы хотите посчитать? ");

        char variable[MAX_LEN_STR] = {0};
        scanf("%s", variable);

        number_variable = find_variable (tree, variable);

        if (number_variable == -1)
        {
            printf ("Введенной вами переменной \"%s\" нет в данной вами функции\n", variable);
            system ("sleep 3");
            menu_derivative (tree, operating_mode);
        }
        else
        {
            sub_menu_derivative (tree, number_variable, operating_mode);
        }
    }
    else
    {
        sub_menu_derivative (tree, number_variable, operating_mode);
    }
}

void sub_menu_derivative (struct Tree* tree, int number_variable, int operating_mode)
{
    screen_clear ();

    printf ("Производную какого порядка вы хотите посчитать? ");

    int num = 0;
    scanf ("%d", &num);

    fprintf (tree->tex, "\n\\section{Производная функции по переменной %s.}", tree->array_variable[number_variable].variable_name);

    Tree* tree_copy = all_tree_clone (tree);

    for (int i = 1; i <= num; i++)
    {
        tree_copy->root = derivative (tree_copy, tree_copy->root, number_variable);
        simplification_tree (tree_copy, tree_copy->root);

        TEX_TEXT_PRINT(tree->tex, tree_copy, "\\\\\\\\ \\noindent \\textbf%d производная изначальной функции:} \\\\", i);
    }

    clear_input ();
    screen_clear ();

    printf ("Хотите найти значение данной производной в функции?\n");

    printf ("[" RED_TEXT(1) "] Да\n");
    printf ("[" RED_TEXT(2) "] Нет\n");

    processing_selected_derivative_mode (tree, tree_copy, num, number_variable, operating_mode);
}

void processing_selected_derivative_mode (struct Tree* tree, struct Tree* tree_copy, int number, int number_variable, int operating_mode)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:  
            screen_clear ();
            fprintf (tree->tex, "\n\\section{Значение %d производной функции по переменной %s.}", number, tree->array_variable[number_variable].variable_name);
            menu_value_function_at_point (tree_copy, operating_mode);
            break;
        case COMMAND_2: 
            screen_clear ();
            tree_dtor (tree_copy); 
            if (operating_mode == 1)
            {
                main_menu (tree);
            }
            break;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_derivative_mode (tree, tree_copy, number, number_variable, operating_mode); 
    }
}

Knot* derivative (struct Tree* tree, struct Knot* current_knot, int number_variable)
{
    Tree* tree_copy = tree_clone (KNOT);

    if (LKNOT != nullptr && RKNOT != nullptr)
    {
        Stack* find_varieble_left  = tree_search_variable (LKNOT, tree->array_variable[number_variable]);
        Stack* find_varieble_right = tree_search_variable (RKNOT, tree->array_variable[number_variable]);
        
        if (find_varieble_left->size == 0 && find_varieble_right->size == 0)
        {
            KNOT = knot_creater (PKNOT, nullptr, nullptr, NUMBER);
            VAL_KNOT = 0;

            return KNOT;
        }

        StackDtor (find_varieble_left);
        StackDtor (find_varieble_right);
    }

    switch (TYPE_KNOT)
    {
        case OPERATION:
        {
            switch (OP_KNOT)
            {
                case ADD:
                case SUB:
                {
                    LKNOT = dL;
                    RKNOT = dR;
                    break;
                }
                case MUL:
                {
                    OP_KNOT = ADD;

                    Knot* mul_knot_left  = knot_op_creater (KNOT, dL, RKNOT, MUL);
                    Knot* mul_knot_right = knot_op_creater (KNOT, dR, LKNOT, MUL);

                    LKNOT = mul_knot_left;
                    RKNOT = mul_knot_right;

                    RKNOT->prev = mul_knot_left;
                    LKNOT->prev = mul_knot_right;

                    L(mul_knot_left)->prev = mul_knot_left;
                    R(mul_knot_right)->prev = mul_knot_right;

                    break;
                }
                case DIV:
                {
                    Knot* denominator = knot_op_creater (KNOT, RKNOT_CLONE, nullptr, POW);
                    R(denominator) = knot_num_creater (denominator, nullptr, nullptr, 2);

                    Knot* numerator  = knot_op_creater (KNOT, nullptr, nullptr, SUB);
                    L(numerator) = knot_op_creater (numerator, dL, RKNOT, MUL);
                    R(numerator) = knot_op_creater (numerator, dR, LKNOT, MUL);

                    LKNOT = numerator;
                    RKNOT = denominator;
                    break;
                }
                case POW:
                {
                    Stack* find_varieble_left  = tree_search_variable (LKNOT,  tree->array_variable[number_variable]);
                    Stack* find_varieble_right = tree_search_variable (RKNOT, tree->array_variable[number_variable]);
        
                    if (find_varieble_left->size != 0 && find_varieble_right->size == 0)
                    {
                        OP_KNOT = MUL;

                        Knot* left_knot = RKNOT;

                        Knot* right_knot = knot_op_creater (KNOT, dL, nullptr, MUL);

                        R(right_knot) = knot_op_creater (right_knot, LKNOT, nullptr, POW);

                        RR(right_knot) = knot_op_creater (R(right_knot), nullptr, nullptr, SUB);

                        RRL(right_knot) = RKNOT_CLONE;
                        RRR(right_knot) = knot_num_creater (RR(right_knot), nullptr, nullptr, 1);

                        LKNOT = left_knot;
                        RKNOT = right_knot;
                    }
                    else if (find_varieble_left->size == 0 && find_varieble_right->size != 0)
                    {   
                        Knot* right_knot = KNOT_CLONE;
                        Knot* left_knot = knot_op_creater (KNOT, nullptr, dR, MUL);

                        L(left_knot) = knot_func_creater (left_knot, nullptr, LKNOT_CLONE, LN);

                        OP_KNOT = MUL;

                        LKNOT = left_knot;
                        RKNOT = right_knot;
                    }
                    else
                    {
                        Knot* right_knot = KNOT_CLONE;
                        Knot* left_knot = knot_op_creater (KNOT, nullptr, RKNOT_CLONE, MUL);

                        L(left_knot) = knot_func_creater (left_knot, nullptr, LKNOT_CLONE, LN);

                        left_knot = d(left_knot);

                        OP_KNOT = MUL;
                        LKNOT = left_knot;
                        RKNOT = right_knot;
                    }

                    StackDtor (find_varieble_left);
                    StackDtor (find_varieble_right);

                    break;
                }
                default: {}
            }
            break;
        }
        case NUMBER:
        {
            VAL_KNOT = 0;
            break;
        }
        case VARIABLE:
        {
            char variable[MAX_LEN_STR] = {0};
            strcpy (variable, VAR_KNOT);

            free (VAR_KNOT);
            TYPE_KNOT = NUMBER;

            if (find_variable (tree, variable) == number_variable)
            {
                VAL_KNOT = 1;
            }
            else
            {
                VAL_KNOT = 0;
            }
            break;
        }
        case FUNCTION:
        {
            switch (FUNC_KNOT)
            {
                #define DEF_FUNC(FUNC, func, func_tex, ...) case (FUNC): __VA_ARGS__ break;

                #include "code_generation.h"

                #undef DEF_FUNC

                default:
                {

                }
            }
            break;
        }
        default:
        {

        }
    }

    return KNOT;
}

//---------------------------------------------------------------------------------

void menu_taylor_decomposition (struct Tree* tree, int operating_mode)
{
    int number_variable = 0;

    if (tree->variable_number > 1)
    {
        printf ("Раскладывать по какой переменной вы хотите? ");

        char variable[MAX_LEN_STR] = {0};
        scanf("%s", variable);

        number_variable = find_variable (tree, variable);

        if (number_variable == -1)
        {
            printf ("Введенной вами переменной \"%s\" нет в данной вами функции\n", variable);
            system ("sleep 5");
            menu_taylor_decomposition (tree, operating_mode);
        }
        else
        {
            sub_menu_taylor_decomposition (tree, number_variable, operating_mode);
        }
    }
    else
    {
        sub_menu_taylor_decomposition (tree, number_variable, operating_mode);
    }
}

void sub_menu_taylor_decomposition (struct Tree* tree, int number_variable, int operating_mode)
{
    double point = 0;
    int    order = 0;

    printf ("В окрестности какой точки вы хоите разложить вашу функцию? ");
    scanf ("%lf", &point);

    screen_clear ();
    clear_input ();

    printf ("До какого порядка вы хотите разложить вашу функцию? ");
    scanf ("%d", &order);

    screen_clear ();
    clear_input ();

    taylor_decomposition (tree, order, point, number_variable);

    simplification_tree (tree, tree->root);

    fprintf (tree->tex, "\n \\\\\\\\ \\section{Разложение вашей функции по Тейлору до %d порядка в окрестности точки ", order);

    if (point == (int)point)
    {
        fprintf (tree->tex, "%d:} \\\\ \n", (int)point);
    }
    else
    {
        fprintf (tree->tex, "%lf:} \\\\ \n", point);
    }

    fprintf (tree->tex, "\\begin{center}");
    fprintf (tree->tex, "\\\\{$");
    tree_print (tree->tex, tree->root);
    fprintf (tree->tex, "$}");
    
    if (point == 0)
    {
        fprintf (tree->tex, "+ {o({%s}^{%d})}", tree->array_variable[number_variable].variable_name, order);
    }
    else if (point == (int)point)
    {
        fprintf (tree->tex, "+ {o({(%s - %d)}^{%d})}", tree->array_variable[number_variable].variable_name, (int)point, order);
    }
    else
    {
        fprintf (tree->tex, "+ {o({(%s - %lf)}^{%d})}", tree->array_variable[number_variable].variable_name, point, order);
    }

    fprintf (tree->tex, "\\end{center}");

    if (operating_mode == 1)
    {
        main_menu (tree);
    }
}

void taylor_decomposition (struct Tree* tree, int order, double point, int number_variable)
{
    Tree* tree_copy = tree_clone (tree->root);

    tree->array_variable[number_variable].variable_value = point;
    Tree* tree_value_zero = value_function_at_point_counter (tree, tree->array_variable[number_variable]);
    knot_dtor (tree->root);
    tree->root = tree_value_zero->root;

    for (int i = 1; i <= order; i++)
    {
        tree_copy->root = dC;
        Tree* tree_copy_point_zero = value_function_at_point_counter (tree_copy, tree->array_variable[number_variable]);

        Knot* add_knot = knot_op_creater (tree->root->prev, tree->root, nullptr, ADD);

        Knot* div_knot = knot_op_creater (add_knot, nullptr, nullptr, DIV);
        R(add_knot) = div_knot;

        Knot* mul_knot = knot_op_creater (div_knot, tree_copy_point_zero->root, nullptr, MUL);
        L(div_knot) = mul_knot;
        tree_copy_point_zero->root->prev = mul_knot;

        Knot* pow_knot = knot_op_creater(mul_knot, nullptr, nullptr, POW);
        R(mul_knot) = pow_knot;

        Knot* factorial_knot = knot_num_creater (div_knot, nullptr, nullptr, factorial (i));
        R(div_knot) = factorial_knot;

        Knot* sub_knot = knot_op_creater (pow_knot, nullptr, nullptr, SUB);
        L(pow_knot) = sub_knot;
        
        Knot* varieble_knot = knot_var_creater (sub_knot, nullptr, nullptr, tree->array_variable[number_variable].variable_name);
        L(sub_knot) = varieble_knot;

        Knot* point_knot = knot_num_creater (sub_knot, nullptr, nullptr, point);
        R(sub_knot) = point_knot;

        Knot* pow_degree_knot = knot_num_creater (pow_knot, nullptr, nullptr, i);
        R(pow_knot) = pow_degree_knot;

        tree->root->prev = add_knot;
        tree->root = add_knot;
    }

    tree_dtor (tree_copy);
}
//---------------------------------------------------------------------------------

void menu_tangent_equation (struct Tree* tree, int operating_mode)
{
    screen_clear ();

    int number_variable = 0;

    if (tree->variable_number > 1)
    {
        printf ("По какой переменной искать уравнение касательной (другие переменные считаюся параметрами)? ");

        char variable[MAX_LEN_STR] = {0};
        scanf("%s", variable);

        number_variable = find_variable (tree, variable);

        if (number_variable == -1)
        {
            printf ("Введенной вами переменной нет в данной вами функции\n");
            system ("sleep 5");
            menu_tangent_equation (tree, operating_mode);
        }
        else
        {
            sub_menu_tangent_equation (tree, number_variable, operating_mode);
        }
    }
    else
    {
        sub_menu_tangent_equation (tree, number_variable, operating_mode);
    }
}

void sub_menu_tangent_equation (struct Tree* tree, int number_variable, int operating_mode)
{
    screen_clear ();

    double point_value = 0;

    printf ("Введите значение точки, в которой хотите найти уравнение касательной для вашей функции: ");
    scanf ("%lf", &point_value);

    clear_input ();

    tangent_equation_at_point (tree, point_value, number_variable);

    if (point_value == (int)point_value)
    {
        TEX_TEXT_PRINT (tree->tex, tree, "\\\\\\\\ \\section{Уравнение касательная к вашей функции в точке %d:} \\\\ \n", (int)point_value);
    }
    else
    {
        TEX_TEXT_PRINT (tree->tex, tree, "\\\\\\\\ \\section{Уравнение касательная к вашей функции в точке %lf:} \\\\ \n", point_value);
    }

    if (operating_mode == 1)
    {
        main_menu (tree);
    }
}

void tangent_equation_at_point (struct Tree* tree, double point_value, int number_variable)
{
    tree->array_variable[number_variable].variable_value = 3;

    Tree* tree_value_point = value_function_at_point_counter (tree, tree->array_variable[number_variable]);
    tree->root = d(tree->root);
    Tree* tree_derivative_value_point = value_function_at_point_counter (tree, tree->array_variable[number_variable]);

    knot_dtor (tree->root);
    tree->root = tree_derivative_value_point->root;

    Knot* add_knot = knot_op_creater (tree_value_point->root->prev, tree_value_point->root, nullptr, ADD);
    tree_value_point->root->prev = add_knot;

    Knot* mul_knot = knot_op_creater (add_knot, tree->root, nullptr, MUL);
    R(add_knot) = mul_knot;
    tree->root->prev = mul_knot;

    Knot* sub_knot = knot_op_creater (mul_knot, nullptr, nullptr, SUB);
    R(mul_knot) = sub_knot;

    R(sub_knot) = knot_num_creater (sub_knot, nullptr, nullptr, point_value);

    L(sub_knot) = knot_var_creater (sub_knot, nullptr, nullptr, tree->array_variable[number_variable].variable_name);

    tree->root = add_knot;

    simplification_tree (tree, tree->root);
}

//---------------------------------------------------------------------------------

void menu_graph_function (struct Tree* tree, int operating_mode)
{
    screen_clear ();

    FILE* graph_gnuplot = fopen ("graph_func/graph_function.gnuplot", "w");
    fprintf (graph_gnuplot, "reset\n");

    printf ("Будете ли вы выбирать значения ограничений для графика?\n");

    printf ("[" RED_TEXT(1) "] Да\n");
    printf ("[" RED_TEXT(2) "] Нет\n");

    processing_selected_graph_function_mode (tree, graph_gnuplot);

    fprintf (graph_gnuplot, "set grid\n");
    fprintf (graph_gnuplot, "set ylabel \"y\"\n");
    fprintf (graph_gnuplot, "set xlabel \"x\"\n");
    fprintf (graph_gnuplot, "set terminal png\n");
    fprintf (graph_gnuplot, "set output \"graph_func/graph.png\"\n");
    fprintf (graph_gnuplot, "plot ");
    tree_print_file (tree->root, graph_gnuplot);

    fclose (graph_gnuplot);

    system ("gnuplot graph_func/graph_function.gnuplot");

    fprintf (tree->tex, "\\\\ \\section{График вашей функции:}");
    fprintf (tree->tex, "\\\\ \\includegraphics{graph_func/graph.png}\\\\");

    if (operating_mode == 1)
    {
        main_menu (tree);
    }
}

void processing_selected_graph_function_mode (struct Tree* tree, FILE* graph_gnuplot)
{
    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:  
        {
            screen_clear ();

            printf ("Введите максимальное значение по оси X: ");
            int max_value_x = 0;
            scanf ("%d", &max_value_x);
            printf ("\n");

            printf ("Введите минимальное значение по оси X: ");
            int min_value_x = 0;
            scanf ("%d", &min_value_x);
            printf ("\n");

            fprintf (graph_gnuplot, "set xrange[%d:%d]\n", min_value_x, max_value_x);

            printf ("Введите максимальное значение по оси Y: ");
            int max_value_y = 0;
            scanf ("%d", &max_value_y);
            printf ("\n");

            printf ("Введите минимальное значение по оси Y: ");
            int min_value_y = 0;
            scanf ("%d", &min_value_y);
            printf ("\n");

            clear_input ();

            fprintf (graph_gnuplot, "set yrange[%d:%d]\n", min_value_y, max_value_y);

            return;
        }
        case COMMAND_2: 
            screen_clear (); 
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
    }
}

void tree_print_file (struct Knot* knot, FILE* graph_gnuplot)
{
    fprintf (graph_gnuplot, "(");

    if (!knot)
    {
        return;
    }

    if (knot->left)
    {
        tree_print_file (knot->left, graph_gnuplot);
    }

    if (knot->type == NUMBER)
    {
        if (knot->value == (int)knot->value)
        {
            fprintf (graph_gnuplot, "%d", (int)knot->value);
        }
        else
        {
            fprintf (graph_gnuplot, "%lf", knot->value);
        }
    }
    else if (knot->type == VARIABLE)
    {
       fprintf (graph_gnuplot, "%s", knot->variable);
    }
    else if (knot->type == OPERATION)
    {
        switch (knot->op_type)
        {
            case (ADD): fprintf (graph_gnuplot, "+");  break;
            case (SUB): fprintf (graph_gnuplot, "-");  break;
            case (MUL): fprintf (graph_gnuplot, "*");  break;
            case (POW): fprintf (graph_gnuplot, "**"); break;
            case (DIV): fprintf (graph_gnuplot, "/");  break;
            default:
            {

            }
        }
    }
    else if (knot->type == FUNCTION)
    {
        switch (knot->function)
        {
            #define DEF_FUNC(FUNC, func_str, func_tex, ...) case (FUNC): fprintf (graph_gnuplot, #func_str); break;

            #include "code_generation.h"

            #undef DEF_FUNC

            default: {}
        }
    }
    
    if (knot->right)
    {
        tree_print_file (knot->right, graph_gnuplot);
    }

    fprintf (graph_gnuplot, ")");
}

//---------------------------------------------------------------------------------

void menu_full_function_study (struct Tree* tree)
{
    screen_clear ();

    printf ("Узнаем значение функции в точке!\n");
    system ("sleep 2");

    menu_value_function_at_point (tree, 0);

    screen_clear ();

    printf ("Посчитаем производные!\n");
    system ("sleep 2");

    Tree* tree_copy = all_tree_clone (tree);
    menu_derivative (tree_copy, 0);
    tree_dtor (tree_copy);

    screen_clear ();

    printf ("Пораскладываем вашу функцию!\n");
    system ("sleep 2");

    screen_clear ();

    tree_copy = all_tree_clone (tree);
    menu_taylor_decomposition (tree_copy, 0);
    tree_dtor (tree_copy);

    screen_clear ();

    printf ("А теперь найдем уравнение касательной к данной функции!\n");
    system ("sleep 2");

    tree_copy = all_tree_clone (tree);
    menu_tangent_equation (tree_copy, 0);
    tree_dtor (tree_copy);

    screen_clear ();

    printf ("А теперь самое время построить график!\n");
    system ("sleep 2");

    menu_graph_function (tree, 0);

    printf ("А на этом все!\n");
    system ("sleep 2");

    main_menu (tree);
}

//---------------------------------------------------------------------------------

int find_variable (struct Tree* tree, char* variable)
{
    for (int i = 0; i < tree->variable_number; i++)
    {
        if (strcmp (variable, tree->array_variable[i].variable_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

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
    convolution_const (tree, KNOT);
    neutralization (tree, KNOT);
}

void convolution_const (struct Tree* tree, struct Knot* current_knot)
{
    if (!type_knot(KNOT))
    {
        if (LKNOT != nullptr) 
        {
            convolution_const (tree, LKNOT);
        }
        if (RKNOT != nullptr) 
        {
            convolution_const (tree, RKNOT);
        }
    }
    else
    {
        double value = calculate_knot (tree, KNOT);

        struct Knot* equal = knot_num_creater (PKNOT, nullptr, nullptr, value);
        
        if (KNOT == tree->root) 
        {
            tree->root = equal;
        }
        else
        {
            if (PKNOT->left == KNOT) 
            {
                PKNOT->left = equal;
            }
            if (PKNOT->right == KNOT) 
            {
                PKNOT->right = equal;
            }
        }
    }
}

void neutralization (struct Tree* tree, struct Knot* current_knot)
{
    if (KNOT != nullptr)
    {
        if (LKNOT != nullptr) 
        {
            neutralization (tree, LKNOT);
        }
        if (RKNOT != nullptr) 
        {
            neutralization (tree, RKNOT);
        }
    }
    else
    {
        return;
    }

    if (TYPE_KNOT == OPERATION)
    {
        switch (OP_KNOT)
        {
            case ADD:
            case SUB:
            {
                if (LKNOT != nullptr)
                {
                    if ((LKNOT->type == NUMBER) && LKNOT->value == 0)
                    {
                        relinking_subtree(tree, KNOT, RKNOT, LKNOT);
                        return;
                    }
                }
                if (RKNOT != nullptr)
                {
                    if ((RKNOT->type == NUMBER) && RKNOT->value == 0)
                    {
                        relinking_subtree(tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }
                break;
            }
            case MUL:
            {
                if (LKNOT != nullptr)
                {
                    if ((LKNOT->type == NUMBER) && LKNOT->value == 0)
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }
                if (RKNOT != nullptr)
                {
                    if ((RKNOT->type == NUMBER) && RKNOT->value == 0)
                    {
                        relinking_subtree (tree, KNOT, RKNOT, LKNOT);
                        return;
                    }
                }
                if (LKNOT != nullptr)
                {
                    if ((LKNOT->type == NUMBER) && LKNOT->value == 1)
                    {
                        relinking_subtree (tree, KNOT, RKNOT, LKNOT);
                        return;
                    }
                }
                if (RKNOT != nullptr)
                {
                    if ((RKNOT->type == NUMBER) && RKNOT->value == 1)
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }
                break;
            }
            
            case DIV:
            {   
                if (LKNOT != nullptr)
                {
                    if ((LKNOT->type == NUMBER) && LKNOT->value == 0)
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }

                if (RKNOT != nullptr)
                {
                    if ((RKNOT->type == NUMBER) && RKNOT->value == 1)
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }

                break;
            }
            case POW:
            {
                if (LKNOT != nullptr)
                {
                    if ((LKNOT->type == NUMBER) && (LKNOT->value == 0))
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                    if ((LKNOT->type == NUMBER) && (LKNOT->value == 1))
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }
                if (RKNOT != nullptr)
                {
                    if ((RKNOT->type == NUMBER) && (RKNOT->value == 0))
                    {
                        RKNOT->value = 1;
                        relinking_subtree (tree, KNOT, RKNOT, LKNOT);
                        return;
                    }
                    if ((RKNOT->type == NUMBER) && (RKNOT->value == 1))
                    {
                        relinking_subtree (tree, KNOT, LKNOT, RKNOT);
                        return;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else if (TYPE_KNOT == FUNCTION)
    {
        switch (FUNC_KNOT)
        {
            case LN:
            case ARCCOS:
            case ARCCH:
            {
                if ((RKNOT->type == NUMBER) && RKNOT->value == 1)
                {
                    TYPE_KNOT = NUMBER;
                    KNOT->value = 0;
                    return;
                }
                break;
            }
            case SIN:
            case TG:
            case ARCSIN:
            case ARCTG:
            case SH:
            case TH:
            case ARCSH:
            case ARCTH:
            case SQRT:
            {
                if ((RKNOT->type == NUMBER) && RKNOT->value == 0)
                {
                    TYPE_KNOT = NUMBER;
                    KNOT->value = 0;
                    return;
                }
                break;
            }
            case COS:
            case CH:
            case EXP:
            {
                if ((RKNOT->type == NUMBER) && RKNOT->value == 0)
                {
                    TYPE_KNOT = NUMBER;
                    KNOT->value = 1;
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
    if (KNOT == tree->root) 
    {
        tree->root = subtree_linking;
        subtree_linking->prev = nullptr;
    }
    else
    {
        subtree_linking->prev = PKNOT;

        if (PKNOT != nullptr && PKNOT->left == KNOT) 
        {
            PKNOT->left = subtree_linking;
        }
        if (PKNOT != nullptr && PKNOT->right == KNOT) 
        {
            PKNOT->right = subtree_linking;
        }
    }
}

int type_knot (struct Knot* KNOT)
{
    if (KNOT == nullptr) 
    {
        return 0;
    }

    if (TYPE_KNOT == VARIABLE)       
    {
        return 0;
    }
    if (TYPE_KNOT == NUMBER) 
    {
        return 1;
    }
    if (TYPE_KNOT == FUNCTION)   
    {
        return 0;
    }
    if (TYPE_KNOT == OPERATION)   
    {
        return type_knot (LKNOT) * type_knot(RKNOT);
    }

    return 0;   
}

double calculate_knot (struct Tree* tree, struct Knot* current_knot)
{   
    assert (KNOT != nullptr);

    switch (KNOT->type)
    {
        case NUMBER: 
            return KNOT->value;
        case OPERATION:
            switch (KNOT->op_type)
            {
                case ADD: return      LCALC + RCALC;
                case SUB: return      LCALC - RCALC;
                case MUL: return      LCALC * RCALC;
                case DIV: return      LCALC / RCALC;     
                case POW: return pow (LCALC , RCALC);
                default:
                    break;
            }
            break;

        case FUNCTION:
            switch (FUNC_KNOT)
            {
                #define DEF_FUNC(FUNC, func, func_tex, ...) case (FUNC): return func (RCALC);

                #include "code_generation.h"

                #undef DEF_FUNC

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

struct Stack* tree_search_variable (struct Knot* KNOT, struct Variable variable)
{
    //ASSERT_OK_TREE (tree);

    struct Stack* path_element = (struct Stack*) calloc (1, sizeof(Stack));

    StackCtor (path_element, 10);

    knot_variable_search(KNOT, path_element, variable);

    //ASSERT_OK_TREE (tree);
    
    return path_element;
}

int knot_variable_search (struct Knot* KNOT, struct Stack* path_element, struct Variable variable)
{
    assert (KNOT);
    assert (path_element);

    StackPush(path_element, KNOT);

    if (KNOT->type == VARIABLE && strcmp (KNOT->variable, variable.variable_name) == 0)
    {
        return 1;
    }
    else
    {
        if (LKNOT != nullptr && RKNOT != nullptr)
        {
            if (knot_variable_search(LKNOT, path_element, variable) || knot_variable_search (RKNOT, path_element, variable))
            {
                return 1;
            }
            else
            {
                StackPop(path_element);
                return 0;
            }
        }
        else if (LKNOT == nullptr && RKNOT == nullptr)
        {
            StackPop(path_element);
            return 0;
        }
        else if (LKNOT == nullptr && RKNOT != nullptr)
        {
            if (knot_variable_search (RKNOT, path_element, variable))
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
                    "\\usepackage{mathtools}\n"
                    "\\DeclareMathOperator{\\arccosh}{arccosh}\n"
                    "\\DeclareMathOperator{\\arcsinh}{arcsinh}\n"
                    "\\DeclareMathOperator{\\arctanh}{arctanh}\n"
                    "\\DeclareMathOperator{\\e}{e}\n"
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

    fprintf (tex, "\\begin{center}");
    fprintf (tex, "\\\\{$");
    tree_print (tex, tree->root);
    fprintf (tex, "$}");
    fprintf (tex, "\\end{center}");
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
            #define DEF_FUNC(FUNC, func, func_tex, ...) case (FUNC): fprintf (tex, "\\" #func_tex); break;

            #include "code_generation.h"

            #undef DEF_FUNC
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