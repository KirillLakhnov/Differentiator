#define KNOT            current_knot
#define PKNOT           current_knot->prev
#define LKNOT           current_knot->left
#define RKNOT           current_knot->right
#define TYPE_KNOT       current_knot->type

#define   OP_KNOT   current_knot->op_type
#define  VAL_KNOT   current_knot->value
#define  VAR_KNOT   current_knot->variable
#define FUNC_KNOT   current_knot->function

#define L(knot) knot->left
#define R(knot) knot->right

#define LL(knot) knot->left->left

#define RL(knot) knot->right->left
#define RR(knot) knot->right->right

#define RRL(knot) knot->right->right->left
#define RRR(knot) knot->right->right->right

#define RRRR(knot) knot->right->right->right->right

#define dC derivative (tree, tree_copy->root,       number_variable)
#define dL derivative (tree, tree_copy->root->left, number_variable)
#define dR derivative (tree, tree_copy->root->right, number_variable)

#define d(knot) derivative (tree, knot, number_variable)

#define KNOT_CLONE  all_knot_clone (current_knot)
#define LKNOT_CLONE all_knot_clone (current_knot->left)
#define RKNOT_CLONE all_knot_clone (current_knot->right)

#define LCALC calculate_knot (tree, current_knot->left)
#define RCALC calculate_knot (tree, current_knot->right)
