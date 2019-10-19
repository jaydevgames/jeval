# JEVAL
*Simple Truth Table Generator*
## Operators:
  * a for conjunction 
    * Logical symbol: ``∧``
  * o for disjunction
    * Logical symbol: ``∨``
  * i for implication or 
    * Logical symbol: ``⇒``
  * ! for negation
    * Logical symbol: ``¬``
  * e for biconditional (or equivalence)
    * Logical symbol: ``≡``
  * n for exclusive disjunction
    * Logical symbol: ``≢``

## Variables
Variables are case-sensitive and can be any non-operator letter.

## Usual order of execution
Without brackets, the order of execution is as following:
  1. Negation
  1. Conjunction/disjunction
  1. Implication
  1. Biconditional/exclusive disjunction

## Compiling
The repo contains the most recent build for Arch Linux (compiled with GCC).
If you'd need to compile for another OS:
  1. Get any C compiler (GCC recommended)
  1. Open up a terminal and ``cd`` to the directory into which this repo was cloned/downloaded. 
  (Don't forget to unzip if you downloaded)
  1. Compile: ``gcc main.c -o build.out`` or ``gcc main.c -o build.exe`` (for Windows)
  1. (If necessary): add execute permissions on the file: ``chmod u+x build.out``
  1. Run with ``./build.out``

## Command-line arguments
  * ``-h`` or ``--help``: shows the help and returns
  * ``'expression'``: evaluates the expression in four steps:
    1. shows the expression in a more readable way,
    1. converts the expression to postfix notation (under the hood),
    1. evaluates the expression, while the application
    1. shows the truth table.

## Flags
The ``def.h`` file contains some flags, which you might want to modify.
The default settings are:
```c
#ifndef _EVAL_DEF //prevent double include
#define _EVAL_DEF // ^^

#define _EVAL_LOGLEVEL 1
#define _EVAL_ERRLEVEL 0
#define _EVAL_OUTLEVEL 0

#endif
```
  1. The first flag (``_EVAL_LOGLEVEL``) defines the output stream for the debug messages.
    * 0 is ``stdout``
    * 1 is ``stderr`` 
    * 2 is ``/dev/null`` (default)
      * WARNING: setting any flag to ``/dev/null`` might cause compilation errors on non-Linux platforms.
    * Any other value causes an error and defaults to ``stdout``
  2. The second flag (``_EVAL_ERRLEVEL``) defines the output stream for any error messages.
    * 0 is ``stderr`` (default)
    * 1 is ``stdout``
    * 2 is ``/dev/null``
      * See the warning for the ``_EVAL_LOGLEVEL`` flag
    * Any other value causes an error and defaults to ``stderr``
  3. The third flag (``_EVAL_OUTLEVEL``) defines the output stream for the truth table and other 'real' output.
    * 0 is ``stdout`` (default)
    * 1 is ``stderr``
    * 2 is ``/dev/null``
      * See the warning for the ``_EVAL_LOGLEVEL`` flag
    * Any other value causes and error and defaults to ``stdout``
      * Changing this flag is generally not recommended.
Don't forget to recompile the project after modifying the ``def.h`` file or any other files.

## Calling from code
To implement the generator for your own projects, include:
```c
#include "fhead.h"
#include "_evallog.h"
#include "types.h"
#include "entity.h"
#include "list.h"
#include "stack.h"
#include "prepare.h"
#include "eval.h"
```

To start the evaluation process, create a string literal (``char *`` or ``const char *``) containing the expression, and allocate (``malloc``) two ``struct _list_node *``. Then call, in order, the following functions:
  1. ``input_to_list(const char *, list)`` (a ``list`` is a ``struct _list_node **``) ~ to convert the string to a list containing all semi-parsed nodes.
    * This function returns ``true`` (or ``1``) when succesfull, otherwise 0.
  1. ``is_valid(list)`` ~ to check whether any syntactical mistakes have been made (is not 100% foolproof).
    * This function returns ``true`` when the expression is valid, according to the algorithm.
  1. ``infix_to_postfix(list, list)`` ~ this function converts the infix-``list`` in the first argument and modifies the second argument to be the corresponding postfix-``list``.
    * This function returns ``1`` if any errors occured while converting.
  1. ``evaluate(list)`` ~ to evaluate the postfix-``list``.
    * This function returns ``1`` if the evaluation finished succesfully, and prints the truth table.
    * There is no official support (yet) to get the result for a certain configuration.

## Known bugs/limitations
  * Implication: requires brackets because of the semi right-to-left evaluation
  * CURRENT VERSION IS NOT FULLY TESTED











