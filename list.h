#ifndef _EVAL_LIST
#define _EVAL_LIST
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "fhead.h"
#include "_evallog.h"
#include "entity.h"

int list_contains_var(list target, char var)
{
	if(target == NULL || *target == NULL || (*target)->content == NULL) { return 0; }
	eval_log("List is not empty.\n");
	struct _list_node *walk = *target;
	while(walk != NULL)
	{
		if(walk->content->content.var[0] == var) { return 1; }
		walk = walk->next;
	}
	return 0;
}

void print_readable(struct _list_node *target, int to_out)
{
	struct _list_node *walk = target;
	while(walk != NULL)
	{
		if(walk->content->is_operator)
		{
			if(walk->content->content.op[0] == ')' || walk->content->content.op[0] == '(')
			{
				char buf[2];
				sprintf(buf, "%c", walk->content->content.op[0]);
				((to_out) ? eval_out : eval_log)(buf);
			}
			else
			{
				switch(walk->content->content.op[0])
				{
					case 'a': ((to_out) ? eval_out : eval_log)("and "); break;
					case 'o': ((to_out) ? eval_out : eval_log)("or "); break;
					case 'i': ((to_out) ? eval_out : eval_log)("implies "); break;
					case 'e': ((to_out) ? eval_out : eval_log)("is equivalent with "); break;
					case 'n': ((to_out) ? eval_out : eval_log)("is not the same as "); break;
					case '!': ((to_out) ? eval_out : eval_log)("not "); break;
				}
			}
		}
		else
		{
			char buf[3];
			sprintf(buf, "%c ", walk->content->content.var[0]);
			((to_out) ? eval_out : eval_log)(buf);
		}
		walk = walk->next;
	}
}

int append(list target, struct _entity *value)
{
	if(target == NULL || (*target) == NULL || (*target)->content == NULL)
	{
//		eval_log("List is NULL.\n");
		struct _list_node *new_node = (struct _list_node *)(malloc(sizeof(struct _list_node)));
		if(new_node == NULL) { return 1; }
		new_node->content = value;
		new_node->next = NULL;
		(*target) = new_node;
//		eval_log("Success.\n");
		return 0;
	}
	else
	{
		struct _list_node *new_node = (struct _list_node *)(malloc(sizeof(struct _list_node)));
		new_node->content = value;
		struct _list_node *walk = (*target);
		while(walk->next != NULL)
		{
			walk = walk->next;
		}
		walk->next = new_node;
//		eval_log("Success.\n");
		return 0;
	}
}

struct _entity *remove_at(list target, int index)
{
	if(target == NULL || *target == NULL)
	{
//		eval_error("List is empty; returning NULL.\n");
		return NULL;
	}
	else
	{
		struct _list_node *walk = *target;
		struct _list_node *prev = NULL;
		int tmpind = 0;
		while(walk != NULL && tmpind < index)
		{
			prev = walk;
			walk = walk->next;
			tmpind++;
		}

		if(tmpind < index) { /*eval_error("Index out of range.\n");*/ return NULL; }
		if(prev == NULL)
		{
			struct _entity *val = (*target)->content;
			*target = walk->next;
			free(walk);
			return val;
		}
		else
		{
			struct _entity *val = walk->content;
			prev->next = walk->next;
			free(walk);
			return val;
		}
	}
}

void print_list(struct _list_node *top)
{
	if(top == NULL)
	{
		eval_error("Received empty list.\n");
		eval_log("List is empty.\n");
	}
	else
	{
		struct _list_node *walk = top;
		if(walk->next == NULL)
		{
			print_entity(walk->content);
			eval_log(" --> NULL\n");
			return;
		}

		while(walk != NULL)
		{
			print_entity(walk->content);
			eval_log(" --> ");
			walk = walk->next;
		}
		eval_log("NULL\n");
	}
}

#endif
