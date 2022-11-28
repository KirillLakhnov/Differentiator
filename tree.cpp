#include "Recursive_descent.h"
#include "tree.h"

void tree_ctor (struct Tree* tree)
{
    tree->tex = tex_open ();
    
    tree->size = 0;
    tree->code_error = 0;

    if (BufferCreater (tree->file_function, tree->buffer_function) != GOOD_WORKING)
    {
        printf ("Error BufferCreater in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_BUFFER_CREATER;
        abort ();
    }

    char* file_buffer = tree->buffer_function->file_buffer;

    tree->root = GetG (&tree->buffer_function->file_buffer);

    tree->buffer_function->file_buffer = file_buffer;

    ASSERT_OK_TREE(tree);
}

void tree_dtor (struct Tree* tree)
{
    ASSERT_OK_TREE(tree);

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

    new_knot->prev  = prev;
    new_knot->left  = left;
    new_knot->right = right;

    new_knot->type = type;

    return new_knot;
}

Knot* knot_clone (Knot* current_knot, Knot* prev)
{
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
        {

        }
    }

    return copy_knot;
}

Tree* tree_clone (Knot* current_knot)
{
    Tree* tree_clone = (Tree*) calloc (1, sizeof(Tree));

    tree_clone->root = all_knot_clone (current_knot);

    return tree_clone;
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

    }

    return tree->code_error;
#else
    return GOOD_WORKING;
#endif
}

//-----------------------------------------------------------------------------------
void knot_graph (struct Tree* tree, struct Knot* current_knot, FILE* tree_log_graph, int* count)
{
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

            KNOT_FUCK_STR(current_knot->function, SIN);
            KNOT_FUCK_STR(current_knot->function, COS);
            KNOT_FUCK_STR(current_knot->function, TG);
            KNOT_FUCK_STR(current_knot->function, CTG);
            KNOT_FUCK_STR(current_knot->function, ARCSIN);
            KNOT_FUCK_STR(current_knot->function, ARCCOS);
            KNOT_FUCK_STR(current_knot->function, ARCTG);
            KNOT_FUCK_STR(current_knot->function, ARCCTG);
            KNOT_FUCK_STR(current_knot->function, LN);
            KNOT_FUCK_STR(current_knot->function, SH);
            KNOT_FUCK_STR(current_knot->function, CH);
            KNOT_FUCK_STR(current_knot->function, TH);
            KNOT_FUCK_STR(current_knot->function, CTH);
            KNOT_FUCK_STR(current_knot->function, ARCSH);
            KNOT_FUCK_STR(current_knot->function, ARCCH);
            KNOT_FUCK_STR(current_knot->function, ARCTH);
            KNOT_FUCK_STR(current_knot->function, EXP);

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
            KNOT_FUCK_STR(current_knot->function, SIN);
            KNOT_FUCK_STR(current_knot->function, COS);
            KNOT_FUCK_STR(current_knot->function, TG);
            KNOT_FUCK_STR(current_knot->function, CTG);
            KNOT_FUCK_STR(current_knot->function, ARCSIN);
            KNOT_FUCK_STR(current_knot->function, ARCCOS);
            KNOT_FUCK_STR(current_knot->function, ARCTG);
            KNOT_FUCK_STR(current_knot->function, ARCCTG);
            KNOT_FUCK_STR(current_knot->function, LN);
            KNOT_FUCK_STR(current_knot->function, SH);
            KNOT_FUCK_STR(current_knot->function, CH);
            KNOT_FUCK_STR(current_knot->function, TH);
            KNOT_FUCK_STR(current_knot->function, CTH);
            KNOT_FUCK_STR(current_knot->function, ARCSH);
            KNOT_FUCK_STR(current_knot->function, ARCCH);
            KNOT_FUCK_STR(current_knot->function, ARCTH);
            KNOT_FUCK_STR(current_knot->function, EXP);         

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


