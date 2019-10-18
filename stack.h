#ifndef _EVAL_STACK
#define _EVAL_STACK
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "fhead.h"
#include "_evallog.h"
#include "entity.h"

int is_null(stack target)
{
	return (target == NULL || (*target) == NULL);
}

struct _entity *pop(stack target)
{
//	eval_log("Started popping process...\n");
	if(target == NULL || (*target) == NULL)
	{
//		eval_error("Stack is empty.\n");
		return NULL;
	}

	struct _stack_node *tmpn = *target;
	struct _entity *tmp = (*target)->content;
	(*target) = (*target)->lower;
	free(tmpn);
//	eval_log("Finished succesfully.\n");
	return tmp;
}

int push(stack target, struct _entity *value)
{
//	eval_log("Started pushing proces...\n");
	if(target == NULL || (*target) == NULL || (*target)->content == NULL)
	{
//		eval_log("Target is NULL...\n");
		struct _stack_node *new_node = (struct _stack_node *)(malloc(sizeof(struct _stack_node)));
		if(new_node == NULL) { return 1; }
		new_node->content = value;
		new_node->lower = NULL;
		(*target) = new_node;
//		eval_log("Finished succesfully.\n");
		return 0;
	}
	else
	{
//		eval_log("Target is not NULL...\n");
		struct _stack_node *new_node = (struct _stack_node *)(malloc(sizeof(struct _stack_node)));
		if(new_node == NULL) { return 1; }
		if((*target)->lower == NULL) 
		{
//			eval_log("No next node currently...\n");
			new_node->content = value;
			struct _stack_node *old = *target;
			new_node->lower = old;
			*target = new_node;
//			eval_log("Finished succesfully.\n");
			return 0;
		}
		new_node->content = value;
		new_node->lower = *target;
		*target = new_node;
//		eval_log("Finished succesfully.\n");
		return 0;
	}
}

const char *stack_error(int errorcode)
{
	switch(errorcode)
	{
		case 0: return "No error.";
		case 1: return "Cannot allocate memory for new node.";
	}
}

void print_stack(struct _stack_node *top)
{
	if(top == NULL)
	{
		eval_error("Received empty stack.\n");
		eval_log("Stack is empty.\n");
	}
	else
	{
		struct _stack_node *walk = top;
		if(walk->lower == NULL)
		{
			print_entity(walk->content);
			eval_log(" --> NULL\n");
			return;
		}

		while(walk != NULL)
		{
			print_entity(walk->content);
			eval_log(" --> ");
			walk = walk->lower;
		}
		eval_log("NULL\n");
	}
}

#endif
