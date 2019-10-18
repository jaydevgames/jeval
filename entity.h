#ifndef _EVAL_ENTITY
#define _EVAL_ENTITY
#include <stdio.h>
#include <string.h>

void print_entity(struct _entity *val)
{
	if(val == NULL) 
	{
		eval_error("Entity is empty. Printing [NULL].\n");
		eval_log("[NULL]");
	}
	else
	{
		char buffer[strlen("Op.: YES; val.: a]")];
		sprintf(buffer, "[Op.: %3s; val.: %1s]",
				val->is_operator ? "YES" : "NO", 
				val->is_operator ? val->content.op : val->content.var);
		eval_log(buffer);
	}
}

#endif
