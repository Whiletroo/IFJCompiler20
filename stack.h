/**
 * @file stack.h
 * @author xpimen00
 * @date 8.10.2020
 * @brief
*/

#ifndef _STACK_H
#define _STACK_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"

#define MAX_STACK 20

//FIXME: Simplify
/* States */
typedef enum
{
    S,    /// 0 < SHIFT
    E,    /// 1 = EQUAL
    R,    /// 2 > REDUCE
    N     /// 3 # ERROR мб не надо!!!!!!!!
} PrecedenceTableSign;

typedef enum
{
    I_PLUS_MINUS,		/// 0 +-
    I_MUL_DIV_IDIV,	    /// 1 */ //
    I_REL_OP,			/// 2 r (relation operators) < <= > >= == !=
    I_LEFT_BRACKET,		/// 3 (
    I_RIGHT_BRACKET,	/// 4 )
    I_DATA,				/// 5 i (id, int, double, string)
    I_DOLLAR			/// 6 $ хз надо ли!!!!!!!
} PrecedenceTableIndex;

typedef enum
{
    /* Mathematical Operators */
    PLUS,			/// 0 +
    MINUS,			/// 1 -
    MUL,			/// 2 *
    DIV,			/// 3 /
    IDIV,			/// 4 //

    /* Relation Operators */
    EQ,				/// 5 ==
    NEQ,			/// 6 !=
    LEQ,			/// 7 <=
    LTN,			/// 8 <
    HEQ,			/// 9 >=
    HTN,			/// 10 >

    /* Braces */
    LEFT_BRACKET,	/// 11 (
    RIGHT_BRACKET,	/// 12 )

    /* Data Types */
    IDENTIFIER,		/// 13 ID
    INT_NUMBER,		/// 14 int
    DOUBLE_NUMBER,	/// 15 double
    STRING,			/// 16 string

    /* Others */
    DOLLAR,			/// 17 $
    STOP,			/// 18 stop symbol used when reducing
    NON_TERM		/// 19 non-terminal
} PrecedenceTableSymbol;

typedef enum
{
    E_OPERAND,		/// 0 E -> i
    LPAR_E_RPAR,    /// 1 E -> (E)
    E_PLUS_E,		/// 2 E -> E + E
    E_MINUS_E,	    /// 3 E -> E - E
    E_IDIV_E,		/// 4 E -> E // E не надо
    E_MUL_E,		/// 5 E -> E * E
    E_DIV_E,		/// 6 E -> E / E
    E_EQ_E,		    /// 7 E -> E == E
    E_NEQ_E,		/// 8 E -> E != E
    E_LEQ_E,		/// 9 E -> E <= E
    E_LTN_E,		/// 10 E -> E < E
    E_MEQ_E,		/// 11 E -> E => E
    E_MTN_E,		/// 12 E -> E > E

    NOT_E_RULE		/// 13 rule doesn't exist
} PrecedenceRules;

typedef struct stackItem
{

    PrecedenceTableSymbol symbol;
    tToken token;
    DATA_TYPE type;
    bool isTerminal; /* <- Whether the terminal is terminal or not */
    bool isStart;    /* <- Start non-terminal */
    struct stackItem *nextItem;
} StackItem;

typedef struct stack
{

    int TopCounter;
    StackItem* topItem;
} Stack;

/**
 * Initialize stack
 *
 * @param s
 */
void stackInit (Stack* s);

/**
 * Check whether stack is empty
 *
 * @param s
 * @return
 */
int stackEmpty (Stack* s);

/**
 * Insert item into the stack
 *
 * @param s
 * @param token
 */
void stackPush (Stack* s, tToken token, DATA_TYPE type);

/**
 * Pop item from the stack
 *
 * @param s
 */
StackItem* stackPop (Stack* s);

/**
 * Get the top item of the stack
 *
 * @param s
 * @return
 */
StackItem* stackTop (Stack* s);

/**
 * Get the top token of the stack
 *
 * @param s
 * @return
 */
tToken* getStackTopToken(Stack* s);

/**
 * Insert stopper after first terminal
 *
 * @param s
 * @return
 */
int insertStopper(Stack* s);

/**
 * Get counter of items before stop item
 *
 * @param s
 * @return
 */
int getStackItemCounterBeforeStopper(Stack *s);

/**
 * Destroy stack
 *
 * @param s
 */
void stackDestroy (Stack* s);

/**
 * Convert token type into data type
 *
 * @param item
 * @return
 */

#endif
