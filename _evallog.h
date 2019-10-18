#ifndef _EVAL_LOG
#define _EVAL_LOG
#include <stdio.h>
#include "def.h"

int __islogged__incorrect__ = 0;
int __iserrd__incorrect__ = 0;
int __isoutd__incorrect__ = 0;

void eval_out(const char *message)
{
#if _EVAL_OUTLEVEL == 0
        fprintf(stdout, message);
#elif _EVAL_OUTLEVEL == 1
        fprintf(stderr, message);
#elif _EVAL_OUTLEVEL == 2
#else
        if(!__islogged__incorrect__) 
        { 
                fprintf(stderr, "Invalid output level defined; logging to stdout.\n"); 
                __isoutd__incorrect__ = 1;
        }
        fprintf(stdout, message);
#endif
}

void eval_out_c(char c) { char buf[2] = { 0 }; buf[0] = c; eval_out(buf); }
void eval_out_i(int i, int len)
{
	char buf[len + 1];
	sprintf(buf, "%d", i);
	eval_out(buf);
}

void eval_log(const char *message)
{
#if _EVAL_LOGLEVEL == 0
	fprintf(stdout, message);
#elif _EVAL_LOGLEVEL == 1
	fprintf(stderr, message);
#elif _EVAL_LOGLEVEL == 2
#else
	if(!__islogged__incorrect__) 
	{ 
		fprintf(stderr, "Invalid log level defined; logging to stdout.\n"); 
		__islogged__incorrect__ = 1;
	}
	fprintf(stdout, message);
#endif
}

void eval_error(const char *errormsg)
{
#if _EVAL_ERRLEVEL == 0
	fprintf(stderr, "%s\n", errormsg);
#elif _EVAL_ERRLEVEL == 1
	fprintf(stdout, "%s\n", errormsg);
#elif _EVAL_ERRLEVEL == 2
#else
	if(!__iserrd__incorrect__)
	{
		fprintf(stderr, "Invalid error level defined; logging to stderr.\n");
		__iserrd__incorrect__ = 1;
	}
	fprintf(stderr, "%s\n", errormsg);
#endif
}

const FILE *normal_stream()
{
#if _EVAL_LOGLEVEL == 0
	return stdout;
#elif _EVAL_LOGLEVEL == 1
	return stderr;
#elif _EVAL_LOGLEVEL == 2
	return open("/dev/null", O_WRONLY);
#else
	eval_error("Incorrect definition for normal output stream.\n");
	return stdout;
#endif
}

const FILE *error_stream()
{
#if _EVAL_ERRLEVEL == 0
	return stderr;
#elif _EVAL_ERRLEVEL == 1
	return stdout;
#elif _EVAL_ERRLEVEL == 2
	return open("/dev/null", O_WRONLY);
#else
	eval_error("Incorrect definition for error output stream.\n");
	return stderr;
#endif
}

#endif
