#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "list.h"
#include "_evallog.h"
#include "prepare.h"
#include "eval.h"

int main()
{
	eval_log("Starting.....\n");
	eval_log("Testing log module...\n");
	eval_error("Testing error module... This is not an error!\n");
	eval_log("Finished.\n");

	eval_log("Reading expression: 'p and q or not r'\n");
	eval_log("Correct expression for tool: 'p a q o !r'\n");
	struct _list_node *list = (struct _list_node *)malloc(sizeof(struct _list_node));
	struct _list_node *postfix = (struct _list_node *)malloc(sizeof(struct _list_node));
	if(input_to_list("p a q o !r", &list)) 
	{ 
		eval_log("Success. List: ");
		eval_log("Is a valid expression? ");
		eval_log(is_valid(&list) ? "YES.\n" : "NO.\n");
		if(!is_valid(&list))
		{
			eval_log("List: ");
			print_list(list);
			eval_log("Stopping.\n");
			return 1;
		}
		print_list(list);
		eval_out("Expression: ");
		print_readable(list, 1);
		eval_out("\n");

		eval_log("Trying to convert list to postfix...\n");
		int res = infix_to_postfix(&list, &postfix);
		if(res)
		{
			eval_log("Failed.\n");
			return 1;
		}
		else
		{
			eval_log("Success.\nList: ");
			print_list(postfix);
			eval_log("Trying to evaluate postfix...\n");
			int res = evaluate(&postfix);
		}
	}
	else
	{
		eval_log("Failed.\n");
	}
	

	eval_log("Cleaning up resources...\n");
	/*struct _entity *rm;
	do{
		rm = remove_at(&list, 0);
	}
	while(rm != NULL);
	free(list);
	do{
                rm = remove_at(&postfix, 0);
        }
	while(rm != NULL);
	free(postfix);
	free(rm);*/
	eval_log("Execution finished.\n");
}




