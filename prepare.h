#ifndef _EVAL_PREP
#define _EVAL_PREP
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fhead.h"
#include "types.h"
#include "_evallog.h"
#include "list.h"

char *operators = "aon!()ei";
char *forbidden = "AON!()EI";

int input_to_list(const char *expr, list out)
{
	char buf[2];

	for(int i = 0; i < strlen(expr); i++)
	{
		struct _entity *ent = (struct _entity *)(malloc(sizeof(struct _entity *)));
		eval_log("Encountered character '");
		sprintf(buf, "%c", expr[i]);
		eval_log(buf);
		eval_log("'. Legality status: ");
		sprintf(buf, "%i", is_legal(expr[i]));
		eval_log(buf);
		eval_log(".\n");

		//encounter: a,o,n,!,(,),e,i: operator; other a-zA-Z: var; whitespace: ignore
		switch(is_legal(expr[i]))
		{
			case 1: 
				ent->is_operator = 1;
				ent->content.op[0] = expr[i];
				ent->content.op[1] = '\0';
				append(out, ent);
				break;

			case 2:
				ent->is_operator = 0;
				ent->content.var[0] = expr[i];
				ent->content.var[1] = '\0';
				append(out, ent);
				break;

			case 3:
				break;

			default:
				eval_error("Unparsable entity in expression: '");
				char buf[2];
				buf[0] = expr[i];
				buf[1] = '\0';
				eval_error(buf);
				eval_error("'.\n");
				return 0;
		}
		char bf[strlen("000 of 100\n")];
		sprintf(bf, "%03d of %03d\n", i, strlen(expr) - 1);
		eval_log(bf);
	}
	//free(ent);
	eval_log("Finished.\n");
	return 1;
}

//0 -> illegal; 1 -> operator; 2 -> var; 3 -> whitespace
int is_legal(char in)
{
	if(in == '\0') { return 0; }
	for(int i = 0; i < strlen(operators); i++)
	{
		if(in == operators[i]) { return 1; }
	}

	for(int i = 0; i < strlen(forbidden); i++)
	{
		if(in == forbidden[i]) { return 0; }
	}

	if(isalpha(in)) { return 2; }
	if(isspace(in)) { return 3; }

	return 0;
}

int is_valid(list exp)
{
	if(exp == NULL || *exp == NULL || (*exp)->content == NULL) { eval_error("NULL pointer.\n"); return 0; }
	struct _list_node *walk = *exp;
	int amount_of_open = 0;
	int amount_of_close = 0;
	while(walk != NULL)
	{

		char buf[strlen("Current: operator: 1; value: a.\n")];
		sprintf(buf, "Current: operator %01d; value: %c.\n", 
				walk->content->is_operator, 
				(walk->content->is_operator) ? 
					walk->content->content.op[0] :
					walk->content->content.var[0]);
		eval_log(buf);
		if(walk->content == NULL) { eval_log("Is NULL.\n"); break; }
		eval_log("Testing.\n");
		if(walk->content->is_operator)
		{
			if(walk->content->content.op[0] == '(') { amount_of_open++; }
			else if(walk->content->content.op[0] == ')') { amount_of_close++; }
			else if(walk->next == NULL || ( walk->next->content->is_operator ))
			{
				if(walk->next->content->content.op[0] == '(' || 
						walk->next->content->content.op[0] == '!')
				{
					if(walk->content->content.op[0] == ')')
					{
						//nok: )( or )!
						return 0;
					}
					else
					{
						//ok: operator + ( or operator + !
					}
				}
			}
			else if(walk->next == NULL)
			{
				if(walk->content->content.op[0] == ')')
				{
					//ok: end with )
				}
				else
				{
					//nok: end with non-) operator
					return 0;
				}
			}
			else
			{
				if(walk->content->content.op[0] == ')') 
				{
					//nok: ) + var
					return 0;
				}
				else
				{
					//ok: operator (not )) + var
				}
			}
		}
		else
		{
			if(walk->next == NULL || ( walk->next->content->is_operator && 
						strcmp(walk->next->content->content.op, "(")))
			{
				//ok: var + operator (not ()
				//ok: var + end of input
			}
			else
			{
				//nok: var + (
				return 0;
			}
		}

		eval_log("Stepping. ");
		walk = walk->next;
		eval_log("Moving on.\n");
	}

	return (amount_of_open == amount_of_close);
}
#endif










