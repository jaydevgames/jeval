#ifndef _EVAL_FHEAD
#define _EVAL_FHEAD
#include "types.h"

int push(stack, struct _entity *);
const char *stack_error(int);
void print_stack(struct _stack_node *);
struct _entity *pop(stack);
int is_null(stack);

int append(list, struct _entity *);
void print_list(struct _list_node *);
void print_readable(struct _list_node *, int);
struct _entity *remove_at(list, int);
int list_contains_var(list, char);

void print_entity(struct _entity *);

void eval_log(const char *);
void eval_error(const char *);
void eval_out(const char *);
void eval_out_c(char);
void eval_out_i(int, int);
const FILE *normal_stream();
const FILE *error_stream();

int input_to_list(const char *, list);
int is_legal(char);
int is_valid(list);
int bit_status(long, int);
int eval_ed(list);
void set_all(list, char, int);
int infix_to_postfix(list, list);
int evaluate(list);
int amount_of_vars(list, char**);
int precedence(struct _entity *, struct _entity *); // -1: 1 > 2; 0: 1 == 2; 1: 1 < 2
#endif
