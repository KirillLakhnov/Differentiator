#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>

#include "libraries/common.h"
#include "tree.h"
#include "Recursive_descent.h"
#include "libraries/stack.h"
#include "DSL.h"

#define RED_TEXT(text) "\e[31m" #text "\e[0m"

#define TEX_TEXT_PRINT(tree_tex, which_var, num_var, tree, ...) fprintf (tree_tex, __VA_ARGS__);                    \
                                                                simplification_tree (tree, tree->root);             \
                                                                tex_tree_print (tree_tex, tree, which_var, num_var);                   
                                                


const size_t MAX_STR_SIZE = 250;

enum COMMAND {
    COMMAND_1 = 49,
    COMMAND_2 = 50,
    COMMAND_3 = 51,
    COMMAND_4 = 52,
    COMMAND_5 = 53,
    COMMAND_6 = 54,
    COMMAND_7 = 55,
    COMMAND_8 = 56,
    COMMAND_9 = 57,
};

enum PRINT_VARIABLE {
    ALL_NAME      = 0,
    ONLY_WITH_VAL = 1,
    ONE_VAR       = 2,
};

enum OPEARTING_MODE {
    FROM_FULL_STUDY_OR_DIFF = 0,
    FROM_MAIN_MENU          = 1,
};

struct Error_variable
{
    char*  name_var;
    double value_errors_var;
};

void start_programm (struct Tree* tree);

void main_menu (struct Tree* tree);

void processing_selected_mode (struct Tree* tree);

//---------------------------------------------------------------------------------

void menu_value_function_at_point (struct Tree* tree, int operating_mode, int operating_mode_point_value);

void processing_selected_value_at_point_mode (struct Tree* tree, int counter);

Tree* value_function_at_point_counter (struct Tree* tree, struct Variable variable);

//---------------------------------------------------------------------------------

void menu_derivative (struct Tree* tree, int operating_mode);

void sub_menu_derivative (struct Tree* tree, int number_variable, int operating_mode);

void processing_selected_derivative_mode (struct Tree* tree, struct Tree* tree_copy, int number, int number_variable, int operating_mode);

Knot* derivative (struct Tree* tree, struct Knot* current_knot, int number_variable);

//---------------------------------------------------------------------------------

void menu_taylor_decomposition (struct Tree* tree, int operating_mode);

void sub_menu_taylor_decomposition (struct Tree* tree, int number_variable, int operating_mode);

void taylor_decomposition (struct Tree* tree, int order, double point, int number_variable);

//---------------------------------------------------------------------------------

void menu_tangent_equation (struct Tree* tree, int operating_mode);

void sub_menu_tangent_equation (struct Tree* tree, int number_variable, int operating_mode);

void tangent_equation_at_point (struct Tree* tree, double point_value, int number_vatiable);

//---------------------------------------------------------------------------------

void menu_graph_function (struct Tree* tree, int operating_mode);

void processing_selected_graph_function_mode (struct Tree* tree, FILE* graph_gnuplot);

void tree_print_file (struct Knot* knot, FILE* graph_gnuplot);

//---------------------------------------------------------------------------------

void menu_error_function (struct Tree* tree, int operating_mode);

void error_function (struct Tree* tree, struct Error_variable* errors_variable);

//---------------------------------------------------------------------------------

void menu_full_function_study (struct Tree* tree);

//---------------------------------------------------------------------------------

int find_variable (struct Tree* tree, char* variable);

int get_command ();

void graph_open (struct Tree* tree);

//---------------------------------------------------------------------------------

void simplification_tree (struct Tree* tree, struct Knot* current_knot);

void convolution_const (struct Tree* tree, struct Knot* current_knot);

void neutralization (struct Tree* tree, struct Knot* current_knot);

void relinking_subtree(struct Tree* tree, struct Knot* current_knot, struct Knot* subtree_linking, struct Knot* subtree_delete);

int type_knot (struct Knot* current_knot);

double calculate_knot (struct Tree* tree, struct Knot* current_knot);

//---------------------------------------------------------------------------------

struct Stack* tree_search_variable (struct Knot* current_knot, struct Variable variable);

int knot_variable_search (struct Knot* current_knot, struct Stack* path_element, struct Variable variable);

//---------------------------------------------------------------------------------

FILE* tex_open ();

void tex_tree_print (FILE* tex, struct Tree* tree, enum PRINT_VARIABLE which_var, int number_variable);

void tree_print (FILE* tex, struct Knot* knot);

void tex_close (FILE* tex);

#endif // DIFFERENTIATOR_H