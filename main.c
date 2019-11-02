#include <stdio.h>
#include <stdlib.h>
#include "fhead.h"
#include "_evallog.h"
#include "types.h"
#include "entity.h"
#include "list.h"
#include "stack.h"
#include "prepare.h"
#include "eval.h"

int _exit_ = 0;

void help()
{
	eval_out("J_EVAL, the boolean expression evaluator.\n");
	eval_out("-----------------------------------------\n");
	eval_out("GENERAL USAGE:\n");
	eval_out("\tCall jeval with the expression you want to evaluate.\n");
	eval_out("\tThe available operators are: \n\ta (for and),");
	eval_out("\to (for inclusive or), i (for implies), \n");
	eval_out("\te (for is equivalent with), n (for exclusive or)\n");
	eval_out("\tand ! (for negation). You can use brackets to force a\n");
	eval_out("\tcertain order of execution.\n");
	eval_out("\tUse the -m or --multiple flag to exit after evaluation.\n");
	eval_out("\tIn -m mode, use \\q to exit.\n");
	eval_out("\tIt is recommended to redirect stderr to /dev/null.\n\n");

	eval_out("NORMAL ORDER OF EXECUTION:\n");
	eval_out("\tFirst: sub-expressions between brackets.\n");
	eval_out("\tSecond: negations.\n");
	eval_out("\tThird: a (and) and o (inclusive or).\n");
	eval_out("\tFourth: i (implication).\n");
	eval_out("\tFifth: e (equivalence) and n (exclusive or).\n\n");

	eval_out("KNOWN BUGS/LIMITATIONS:\n");
	eval_out("\tJ_EVAL is not yet able to parse the semi right-to-left\n");
	eval_out("\tstructure of the implication. This means that brackets are\n");
	eval_out("\tnecessary and shouldn't be omitted.\n\n");
}

void exec(char *comm)
{
	if(strcmp(comm, "--help") == 0 || strcmp(comm, "-h") == 0)
        {
		eval_out("\n");
                help();
	}
	else if(strcmp(comm, "\\q") == 0)
	{
		eval_out("Exiting...\n");
		_exit_ = 1;
		return;
	}
        else
        {
                eval_log("Expression: ");
                eval_log(comm); 
                eval_log("\n");

                struct _list_node *infix = (struct _list_node *)malloc(sizeof(struct _list_node));
                struct _list_node *postfix = (struct _list_node *)malloc(sizeof(struct _list_node));
                if(input_to_list(comm, &infix))
                {
			eval_log("List created.\n");
                        if(is_valid(&infix))
                        {
				eval_log("Expression valid.\n");
                                eval_out("Human readable expression: ");
                                print_readable(infix, 1);
                                eval_out("\n");

                                if(infix_to_postfix(&infix, &postfix))
                                {
                                        eval_error("Something went wrong while converting to postfix.\n");
                                        eval_out("Error.\n");
                                }
				else
                                {
					eval_log("Expression converted to postfix.\n");
                                        if(evaluate(&postfix))
                                        {
                                                eval_log("Finished.\n");
                                        }
					else
                                        {
                                                eval_error("Couldn't evaluate the expression.\n");
                                                eval_out("Error.\n");
                                        }
                                }
                        }
                        else
                        {
                                eval_error("The given expression is invalid.\n");
                                eval_out("Error.\n");
                        }
                }
                else
                {
                        eval_error("Can't parse the given expression.\n");
                        eval_out("Error.\n");
                }
        }
}

int main(int argc, char **argv)
{
	int multiple = 0;
	if(argc >= 2)
	{
		if(strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--multiple"))
		{
			multiple = 1;
			if(argc == 2) goto nodata;
		}
		exec(argv[1]);
	}
	else
	{
nodata: 
		eval_out("Please enter an expression to evaluate: (use --help for help): ");
		char bufhold[256];
		scanf("%255s", bufhold);
		exec(bufhold);
	}
	if(_exit_) { return 0; }
	if(multiple) { main(2, argv); }
}
