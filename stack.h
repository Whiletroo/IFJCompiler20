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
#include "expressions.h"

#define MAX_STACK 20

//FIXME: Simplify
/* States */
typedef enum
{
    S,    /// 0 < SHIFT
    E,    /// 1 = EQUAL
    R,    /// 2 > REDUCE
    N     /// 3 # ERROR �� �� ����!!!!!!!!
} PrecedenceTableSign;



typedef struct stackItem
{

    PrecedenceTableSymbol symbol;
    tToken token;
    PrecedenceTableSymbol type;
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
void stackPush (Stack* s, tToken token, PrecedenceTableSymbol type);

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