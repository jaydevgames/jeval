#ifndef _EVAL_EVAL
#define _EVAL_EVAL
#include "types.h"
#include "fhead.h"
#include "list.h"
#include "stack.h"

int amount_of_vars(list in, char **list_of_vars)
{
	eval_log("Getting amount of variables...\n");
	struct _list_node *tmp = (struct _list_node *)malloc(sizeof(struct _list_node));
	struct _list_node *walk = *in;
	while(walk != NULL)
	{
		if(walk->content->is_operator) { walk = walk->next; continue; }
		if(!list_contains_var(&tmp, walk->content->content.var[0]))
		{
			char buf[strlen("Found new variable: 'c'.\n")];
			sprintf(buf, "Found new variable: '%c'.\n", walk->content->content.var[0]);
			eval_log(buf);
			append(&tmp, walk->content);
		}
		walk = walk->next;
	}

	eval_log("Constructed temp list.\n");
	eval_log("Temp list: ");
	print_list(tmp);
	walk = tmp;
	while(walk->content->is_operator)
	{
		walk = walk->next;
		remove_at(&tmp, 0);
	}
	walk = tmp;
	int count = 0;
	while(walk != NULL)
	{
		count++;
		walk = walk->next;
	}

	char *buf = (char *)malloc(count + 1);
	walk = tmp;
	int i = 0;
	eval_log("All variables: ");
	char buff[3] = { 0 };
	buff[1] = ' ';
	while(walk != NULL && i < count)
	{
		buff[0] = walk->content->content.var[0];
		buf[i] = walk->content->content.var[0];
		i++;
		walk = walk->next;
		eval_log(buff);
	}
	eval_log("\n");
	buf[i] = '\0';
	*list_of_vars = buf;
	return count;
}

int bit_status(long mask, int index)
{
	return (mask >> index) & 1;
}

int eval_one(int op1, char oper, int op2)
{
	switch(oper)
	{
		case 'a': return op1 && op2;
		case 'o': return op1 || op2;
		case 'e': return op1 == op2;
		case 'n': return op1 != op2;
		case 'i': return (!op1 || (op1 && op2));
		default: return 1;
	}
}

int eval_ed(list in) 
{
	struct _list_node *walk = *in;
	struct _stack_node *stack = (struct _stack_node *)malloc(sizeof(struct _stack_node));
	struct _entity *x = (struct _entity *)malloc(sizeof(struct _entity));
	struct _entity *y = (struct _entity *)malloc(sizeof(struct _entity));
	while(walk != NULL && walk->content != NULL)
	{
		x->is_operator = 0;
		y->is_operator = 0;
		eval_log("Parsing entity....\n");
		if(walk->content->is_operator)
		{
			eval_log("Is operator.\n");
			if(walk->content->content.op[0] == '!')
			{
				eval_log("Is negation.\n");
				x = pop(&stack);
				x->current = !(x->current);
				push(&stack, x);
			}
			else
			{
				x = pop(&stack);
				y = pop(&stack);
				x->current = eval_one(x->current, walk->content->content.op[0], y->current);
				push(&stack, x);
			}
		}
		else
		{
			eval_log("Is variable.\n");
			push(&stack, walk->content);
		}

		eval_log("Moving on...\n");
		walk = walk->next;
	}
	struct _entity *t = pop(&stack);
	int rt = t->current;
	char buf[strlen("Finished. Value: 1.\n\n")];
	sprintf(buf, "Finished. Value: %01d.\n\n", rt);
	eval_log(buf);
	//free(x);
	//free(y);
	//free(t);
	free(stack);
	return rt;
}

void set_all(list in, char var, int val)
{
	struct _list_node *walk = *in;
	while(walk != NULL)
	{
		if(walk->content->content.var[0] == var)
		{
			walk->content->current = val;
		}
		walk = walk->next;
	}
}

int evaluate(list in)
{
	eval_log("\nEvaluating...\n");
	char *all_vars;
	int vars = amount_of_vars(in, &all_vars);
	print_list(*in);
	char buf[3] = { 0 };
	eval_log("Amount of variables: ");
	sprintf(buf, "%d", vars);
	eval_log(buf);
	eval_log("\n");
	eval_log("All variables: ");
	eval_log(all_vars);
	eval_log("\n");

	long max = 1;
	for(int i = 0; i < vars; i++) { max *= 2; }
	long mask = 0;

	eval_out_c('+');
	for(int i = 0; i < vars; i++) { eval_out("---+"); }
	eval_out("--------+\n|");
	for(int i = 0; i < vars; i++)
	{
		eval_out(" "); eval_out_c(all_vars[i]); eval_out(" |");
	}
	eval_out(" result |\n+");
	for(int i = 0; i < vars; i++) { eval_out("---+"); }
	eval_out("--------+\n");

	for(;mask < max; mask++)
	{
		eval_out("|");
		for(int i = 0; i < vars; i++) 
		{
			eval_out(" "); eval_out_i(bit_status(mask, i), 1); eval_out(" |");
			set_all(in, all_vars[i], bit_status(mask, i));
		}
		int res = eval_ed(in);
                eval_out("   "); eval_out_i(res, 1); eval_out("    |");
		eval_out("\n");
	}
	eval_out("+");
	for(int i = 0; i < vars; i++) { eval_out("---+"); }
        eval_out("--------+\n"); 
	return 1;
}



int infix_to_postfix(list in, list out)
{
	eval_log("Started converting infix to postfix...\n");
	struct _list_node *walk = *in;
	struct _stack_node *stack = (struct _stack_node *)malloc(sizeof(struct _stack_node));
	struct _entity *left_parent = (struct _entity *)malloc(sizeof(struct _entity));
	left_parent->is_operator = 1;
	left_parent->content.op[0] = '(';
	struct _entity *right_parent = (struct _entity *)malloc(sizeof(struct _entity));
        right_parent->is_operator = 1;        
        right_parent->content.op[0] = ')';
	append(in, right_parent);
	push(&stack, left_parent);
	//struct _stack_node walk_node;
	struct _entity *tmp;
	eval_log("Finished preparations.\n");

	eval_log("Started conversion...\n");
	while(!is_null(&stack) && walk != NULL)
	{
		eval_log("\nCurrent postfix: ");
		print_list(*out);
		if(walk->content->is_operator)
		{
			//current is operator or parenthesis
			if(walk->content->content.op[0] == '(')
			{
				eval_log("Pushing left parenthesis.\n");
				//left parenthesis
				push(&stack, walk->content);
			}
			else if(walk->content->content.op[0] == ')')
			{
				eval_log("Popping until encounter of next left parenthesis.\n");
				//right parenthesis
				while(!is_null(&stack) && stack->content->content.op[0] != '(')
				{
					tmp = pop(&stack);
					append(out, tmp);
				}
				pop(&stack);
			}
			else
			{
				eval_log("Popping until encounter of a lesser operator.\n");
				//normal operator
				int prec = precedence(walk->content, stack->content);
				if(prec == 100) { return 1; }
				while(!is_null(&stack) && walk->content->content.op[0] != '(' &&
						precedence(walk->content, stack->content) <= 0)
				{
					if(precedence(walk->content, stack->content) == 100) { return 1; }
					tmp = pop(&stack);
					append(out, tmp);
				}
				push(&stack, walk->content);
			}
			eval_log("Current operator stack: ");
			print_stack(stack);
		}
		else
		{
			eval_log("Appending variable.\n");
			//current is var
			append(out, walk->content);
		}
		walk = walk->next;
	}

	eval_log("Finished succesfully.\n");
	/*struct _entity *rm;
	do
	{
		rm = pop(&stack);
	} while(rm != NULL);*/
	//free(walk); free(stack); free(left_parent); free(right_parent); free(tmp);
	return 0;
}

//100 = error (non operator); 101 = error (parentheses)
int precedence(struct _entity *op1, struct _entity *op2)
{
	if(!op1->is_operator || !op2->is_operator)
	{
		eval_error("Can't compare precedence with non-operator entity.\n");
		return 100;
	}

	int pr1, pr2;
	switch(op1->content.op[0])
	{
		case '(':
		case ')': pr1 = 0; break;
		case '!': pr1 = 4; break;
		case 'a':
		case 'o': pr1 = 3; break;
		case 'i': pr1 = 2; break;
		case 'n':
		case 'e': pr1 = 1; break;
		default: eval_error("Operator 1 is not a recognized operator.\n"); pr1 = 0;
	}

	switch(op2->content.op[0])
        {
		case '(':
		case ')': pr2 = 0; break;
                case '!': pr2 = 4; break;
                case 'a':
                case 'o': pr2 = 3; break;
                case 'i': pr2 = 2; break;
                case 'n':
                case 'e': pr2 = 1; break;
                default: eval_error("Operator 2 is not a recognized operator.\n"); pr2 = 0;
        }

	return (pr1 - pr2 < 0) ? -1 : (pr1 == pr2) ? 0 : 1;
}

#endif







