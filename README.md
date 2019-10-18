# JEVAL
*Simple Truth Table Generator*
## Operators:
  * a for conjunction 
    * Logical symbol: "∧"
  * o for disjunction
    * Logical symbol: "∨"
  * i for implication or 
    * Logical symbol: "⇒"
  * ! for negation
    * Logical symbol: "¬"
  * e for biconditional (or equivalence)
    * Logical symbol: "≡"
  * n for exclusive disjunction
    * Logical symbol: "≢"

## Variables
Variables are case-sensitive and can be any non-operator letter.

## Usual order of execution
Without brackets, the order of execution is as following:
  1. Negation
  1. Conjunction/disjunction
  1. Implication
  1. Biconditional/exclusive disjunction

## Known bugs/limitations
  * Implication: requires brackets because of the semi right-to-left evaluation
  * CURRENT VERSION IS UNSTABLE