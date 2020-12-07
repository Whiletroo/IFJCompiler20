/**
* @file precstack.h
* @author xkrukh00
* @date 03.12.2020
* @brief The header file for precstack.c
*/

#ifndef _PRECSTACK_H
#define _PRECSTACK_H

#include <stdbool.h>
#include <stdio.h>
#include "expressions.h"



/**
 * @struct Precedence stack item
*/
typedef struct precStackItem {

    tPrecTabItem precItem;   
    tDataType dataType;
    struct precStackItem *next;

} tPSItem;

typedef struct {
    tPSItem *top;
}tPS;

extern tPS *precStack;

/**
 * Initialize a stack for precedence analisys
 * 
 * @return tPS * pointer to stack for precedence analisys. NULL on allocation error
*/
tPS *initPS();


/**
 * Push item to precedence stack
 * 
 * @param tabItem precedence table item
 * @param dataType type of data (for sematic analise)
 * @return exit code
*/
int pushPS(tPrecTabItem precItem, tDataType datatype);


/**
 * Get pointer to top item in stack for precedence analise
 * 
 * @return tPSItem * pointer to precendece stack item
*/
tPSItem *topPS();


/**
 * Get pointer to top terminal in stack for precedence analisys
 * 
 * @return tPSItem * pointer to precendece stack item, NULL when no terminals in stack
*/
tPSItem *topTermPS();

/**
 * Inserts reduce symbol before top Terminal
*/
void insertReducePS();

/**
 * Check if is precedence stack empty (DOLLAR ON THE TOP)
 * 
 * @return true if empty, false if not empty
*/
bool emptyPS();


/**
 * Pop top item. Cant pop DOLLAR.
*/
void popPS();

/**
 * Dispose precedence stack;
*/
void disposePS();


/**
 * Fully frees allocated memory for precedence stack
*/
void freePS();





#endif