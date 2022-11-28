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

#define RED_TEXT(text) "\e[31m" #text "\e[0m"

#define TEX_TEXT_PRINT(tree_tex, tree, ...)     char tex_text[MAX_LEN_CMD] = "";                                    \
                                                snprintf (tex_text, MAX_LEN_CMD, __VA_ARGS__);                      \
                                                tex_tree_print (tree_tex, tree, tex_text);


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

void start_programm (struct Tree* tree);

void main_menu (struct Tree* tree);

void processing_selected_mode (struct Tree* tree);

//---------------------------------------------------------------------------------

void menu_value_function_at_point (struct Tree* tree);

Tree* value_function_at_point_counter (struct Tree* tree, double point_value);

//---------------------------------------------------------------------------------

void menu_derivative (struct Tree* tree);

void processing_selected_derivative_mode (struct Tree* tree);

Knot* derivative (struct Knot* current_knot);

//---------------------------------------------------------------------------------

void menu_maclaurin_decomposition (struct Tree* tree);

void maclaurin_decomposition (struct Tree* tree, int order);

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

void menu_tangent_equation (struct Tree* tree);

void tangent_equation_at_point (struct Tree* tree, double point_value);

//---------------------------------------------------------------------------------

void menu_graph_function (struct Tree* tree);

void processing_selected_graph_function_mode (struct Tree* tree, FILE* graph_gnuplot);

void tree_print_file (struct Knot* knot, FILE* graph_gnuplot);

//---------------------------------------------------------------------------------

int get_command ();

void graph_open (struct Tree* tree);

//---------------------------------------------------------------------------------

void simplification_tree (struct Tree* tree, struct Knot* current_knot);

void convolution_const (struct Tree* tree, struct Knot* current_knot);

void neutralization (struct Tree* tree, struct Knot* current_knot);

void relinking_subtree(struct Tree* tree, struct Knot* current_knot, struct Knot* subtree_linking, struct Knot* subtree_delete);

int type_knot (struct Knot* current_knot);

double calculate_knot (struct Knot* current_knot);

//---------------------------------------------------------------------------------

struct Stack* tree_search_varieble (struct Knot* current_knot);

int knot_varieble_search (struct Knot* current_knot, struct Stack* path_element);

//---------------------------------------------------------------------------------

FILE* tex_open ();

void tex_tree_print (FILE* tex, struct Tree* tree, char* text);

void tree_print (FILE* tex, struct Knot* knot);

void tex_close (FILE* tex);

#endif // DIFFERENTIATOR_H