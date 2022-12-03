#include "Recursive_descent.h"
#include "tree.h"

void tree_ctor (struct Tree* tree)
{
    assert (tree);

    tree->tex = tex_open ();
    
    tree->size = 0;
    tree->code_error = 0;
    tree->variable_number = 0;

    for (int i = 0; i < MAX_NUMBER_VARIEBLE; i++)
    {
        tree->array_variable[i].variable_value = POISON_VALUE;
    }

    if (BufferCreater (tree->file_function, tree->buffer_function) != GOOD_WORKING)
    {
        printf ("Error BufferCreater in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_BUFFER_CREATER;
        abort ();
    }

    char* file_buffer = tree->buffer_function->file_buffer;

    tree->root = GetG (&tree->buffer_function->file_buffer, tree);

    tree->buffer_function->file_buffer = file_buffer;

    ASSERT_OK_TREE(tree);
}

void tree_dtor (struct Tree* tree)
{
    assert (tree);

    if (tree->buffer_function != nullptr)
    {
        TextDtor (tree->buffer_function);
    }
    if (tree->file_function != nullptr)
    {
        tree->file_function->file_name = nullptr;
    }

    knot_dtor (tree->root);

    tree->root = nullptr;
    tree->code_error = DTOR_SIZE_T;
    tree->size = DTOR_SIZE_T;

}

void knot_dtor (struct Knot* current_knot)
{
    assert (current_knot);

    if (current_knot->left != nullptr) 
    {
        knot_dtor (current_knot->left);
    }
    if (current_knot->right != nullptr) 
    {
        knot_dtor (current_knot->right);
    }

    current_knot->prev = nullptr;
    current_knot->right = nullptr;
    current_knot->left = nullptr;

    if (current_knot->type == VARIABLE)
    {
        if (current_knot->variable != nullptr)
        {
            free (current_knot->variable);
            current_knot->variable = nullptr;
        }
    }
}
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

Knot* knot_creater (Knot* prev, Knot* left, Knot* right, enum TYPE type)
{
    struct Knot* new_knot = (Knot*) calloc (1, sizeof(Knot));

    if (new_knot == nullptr)
    {
        printf ("calloc error: in tree.cpp on %d\n", __LINE__);
        abort ();
    }

    new_knot->prev  = prev;
    new_knot->left  = left;
    new_knot->right = right;

    new_knot->type = type;

    return new_knot;
}

Knot* knot_op_creater (Knot* prev, Knot* left, Knot* right, enum OPERATION_TYPE OP)
{
    Knot* new_knot = knot_creater (prev, left, right, OPERATION);
    new_knot->op_type = OP;

    return new_knot;
}

Knot* knot_num_creater (Knot* prev, Knot* left, Knot* right, int value)
{
    Knot* new_knot = knot_creater (prev, left, right, NUMBER);
    new_knot->value = value;  

    return new_knot;
}

Knot* knot_func_creater (Knot* prev, Knot* left, Knot* right, enum MATH_FUNC FUNC)
{
    Knot* new_knot = knot_creater (prev, left, right, FUNCTION);
    new_knot->function = FUNC;    

    return new_knot;
}

Knot* knot_var_creater (Knot* prev, Knot* left, Knot* right, char* variable)
{
    Knot* new_knot = knot_creater (prev, left, right, VARIABLE);

    new_knot->variable = (char*) calloc (MAX_LEN_STR, sizeof(char));
    if (new_knot->variable == nullptr)
    {
        printf ("calloc error: in tree.cpp on %d\n", __LINE__);
        abort ();
    }
    strcpy (new_knot->variable, variable);

    return new_knot;
}

Knot* knot_clone (Knot* current_knot, Knot* prev)
{
    assert (current_knot);

    Knot* copy_knot = knot_creater (prev, current_knot->left, current_knot->right, current_knot->type);

    switch (current_knot->type)
    {
        case (OPERATION):
            copy_knot->op_type = current_knot->op_type;
            break;
        case (NUMBER):
            copy_knot->value = current_knot->value;
            break;
        case (VARIABLE):
            copy_knot->variable = (char*) calloc (MAX_LEN_STR, sizeof (char));
            strcpy (copy_knot->variable, current_knot->variable);
            break;
        case (FUNCTION):
            copy_knot->function = current_knot->function;
            break;
        default:
            printf ("syntax error in tree: in tree.cpp on %d\n", __LINE__);
            abort ();
    }

    return copy_knot;
}

Tree* tree_clone (Knot* current_knot)
{
    Tree* tree_clone = (Tree*) calloc (1, sizeof(Tree));
    if (tree_clone == nullptr)
    {
        printf ("calloc error: in tree.cpp on %d\n", __LINE__);
        abort ();
    }

    tree_clone->root = all_knot_clone (current_knot);

    return tree_clone;
}

Tree* all_tree_clone (Tree* tree)
{
    Tree* tree_copy = tree_clone (tree->root);

    tree_copy->tex             = tree->tex;
    tree_copy->code_error      = tree->code_error;
    tree_copy->variable_number = tree->variable_number;
    tree_copy->size            = tree->size;

    for (int i = 0; i < tree->variable_number; i++)
    {   
        if (tree->array_variable[i].variable_value != tree->array_variable[i].variable_value)
        {
            tree_copy->array_variable[i].variable_value = NAN;
        }
        else
        {
            tree_copy->array_variable[i].variable_value = tree->array_variable[i].variable_value;
        }
        strcpy (tree_copy->array_variable[i].variable_name, tree->array_variable[i].variable_name);
    }

    return tree_copy;
}

Knot* all_knot_clone (Knot* current_knot)
{
    assert (current_knot);

    Knot* clone_left = nullptr;
    Knot* clone_right = nullptr;

    Knot* current_clone = knot_clone (current_knot, current_knot->prev);

    if (current_knot->left != nullptr) 
    {
        clone_left = all_knot_clone (current_knot->left);
        clone_left->prev = current_clone;
    }
    if (current_knot->right != nullptr) 
    {
        clone_right = all_knot_clone (current_knot->right);
        clone_right->prev = current_clone;
    }

    current_clone->left = clone_left;
    current_clone->right = clone_right;

    return current_clone;
}

//-----------------------------------------------------------------------------------

int tree_error (struct Tree* tree)
{
#ifndef NDEBUG
    int pointer_list_check_null = ((!tree) ? TREE_ERROR_POINTER_STRUCT_NULL : 0);

    if (pointer_list_check_null == 0)
    {
        tree->code_error |= CHECK_ERROR (!tree->root,                    TREE_ERROR_POINTER_ROOT_NULL);
        //tree->code_error |= CHECK_ERROR (!tree->buffer_function,         TREE_ERROR_POINTER_BUFFER_NULL);
        //tree->code_error |= CHECK_ERROR (!tree->file_function,           TREE_ERROR_POINTER_FILE_INFO_NULL);
        tree->code_error |= CHECK_ERROR (tree->size < 0,                 TREE_ERROR_SIZE_SMALLER_ZERO);
    }

    return tree->code_error;
#else
    return GOOD_WORKING;
#endif
}

//-----------------------------------------------------------------------------------
void knot_graph (struct Tree* tree, struct Knot* current_knot, FILE* tree_log_graph, int* count)
{
    assert (tree);
    assert (tree_log_graph);

    if (current_knot == nullptr)
    {
        return;
    }
    (*count)++;

    if (current_knot->right == nullptr && current_knot->left == nullptr)
    {
        if (current_knot->type == OPERATION)
        {
            char operation[MAX_LEN_CMD] = {0};
            if (current_knot->op_type == ADD)
            {
                *operation = '+';
            }
            else if (current_knot->op_type == SUB)
            {
                *operation = '-';
            }
            else if (current_knot->op_type == MUL)
            {
                *operation = '*';
            }
            else if (current_knot->op_type == DIV)
            {
                *operation = '/';
            }
            
            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#ee9b52\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, operation, current_knot->right);
        }
        else if (current_knot->type == NUMBER)
        {
            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#ee9b52\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %lf | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, current_knot->value, current_knot->right);
        }
        else if (current_knot->type == FUNCTION)
        {
            char function[MAX_LEN_CMD] = {0};

            #define DEF_FUNC(FUNC, func_str, func_tex, ...) if (current_knot->function == FUNC)     \
                                                            {                                       \
                                                                strcpy (function, #FUNC);           \
                                                            }

            #include "code_generation.h"

            #undef DEF_FUNC

            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#ee9b52\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, function, current_knot->right);
        }
        else if (current_knot->type == VARIABLE)
        {
            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#ee9b52\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, current_knot->variable, current_knot->right);
        }
    }
    else 
    {
        if (current_knot->type == OPERATION)
        {
            char operation[MAX_LEN_CMD] = {0};
            if (current_knot->op_type == ADD)
            {
                *operation = '+';
            }
            else if (current_knot->op_type == SUB)
            {
                *operation = '-';
            }
            else if (current_knot->op_type == MUL)
            {
                *operation = '*';
            }
            else if (current_knot->op_type == DIV)
            {
                *operation = '/';
            }
            else if (current_knot->op_type == POW)
            {
                *operation = '^';
            }

            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, operation, current_knot->right);
        }
        else if (current_knot->type == NUMBER)
        {
            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %lf | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, current_knot->value, current_knot->right);
        }
        else if (current_knot->type == FUNCTION)
        {
            char function[MAX_LEN_CMD] = {0};

            #define DEF_FUNC(FUNC, func_str, func_tex, ...) if (current_knot->function == FUNC)     \
                                                            {                                       \
                                                                strcpy (function, #FUNC);           \
                                                            }

            #include "code_generation.h"

            #undef DEF_FUNC       

            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, function, current_knot->right);
        }
        else if (current_knot->type == VARIABLE)
        {
            fprintf(tree_log_graph, "\t\"%p\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                            "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                    current_knot, current_knot->prev, current_knot, current_knot->left, current_knot->variable, current_knot->right);
        }
    }

    if (current_knot->left != nullptr)
    {
        fprintf(tree_log_graph, "\t \"%p\" -> \"%p\" [style = \"bold\"]\n", current_knot, current_knot->left);
    }

    if (current_knot->right != nullptr)
    {
        fprintf(tree_log_graph, "\t \"%p\" -> \"%p\" [style = \"bold\"]\n", current_knot, current_knot->right);
    }

    if (current_knot->left  != nullptr) 
    {
        knot_graph(tree, current_knot->left, tree_log_graph, count);
    }

    if (current_knot->right != nullptr) 
    {
        knot_graph(tree, current_knot->right, tree_log_graph, count); 
    }
}

int tree_graph_dump (struct Tree* tree)
{
    assert (tree);

    static int number_of_function_launches = 0;

    FILE* tree_log_graph = fopen ("graph/graph_log_tree.dot", "w");

    fprintf (tree_log_graph, "digraph G\n{\n");
    fprintf (tree_log_graph, "\tgraph [dpi = 250];\n");
    fprintf (tree_log_graph, "\trankdir = TB;\n");

    int count = 0;
    knot_graph (tree, tree->root, tree_log_graph, &count);

    fprintf (tree_log_graph, "\n}");

    fclose (tree_log_graph);

    char command[MAX_LEN_STR] = "";
    sprintf (command, "dot -Tpng -ograph/graph_log_tree_%d.png graph/graph_log_tree.dot", number_of_function_launches);

    system(command);

    FILE* htm_log_file = fopen ("graph/htm_log_file.htm", "a");

    fprintf (htm_log_file, "<pre>\n");
    fprintf (htm_log_file, "<img src = \"graph_log_tree_%d.png\">", number_of_function_launches);
    fprintf (htm_log_file, "<hr>\n");

    fclose(htm_log_file);

    number_of_function_launches++;

    return number_of_function_launches - 1;
}


