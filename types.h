#ifndef EVAL_BOOL_TYPES
#define EVAL_BOOL_TYPES

struct _entity {
	int is_operator;
	union {
		char op[2]; // and -> a; or -> o; equiv -> e; implic -> i; logic not eq -> n; neg -> !
		char var[2]; //var: max 1 char; not from list above
	} content;
	int current;
};

struct _stack_node {
	struct _entity *content;
	struct _stack_node *lower;
};

struct _list_node {
	struct _entity *content;
	struct _list_node *next;
};

typedef struct _stack_node stack_node;
typedef struct _stack_node **stack;

typedef struct _list_node list_node;
typedef struct _list_node **list;

#endif
