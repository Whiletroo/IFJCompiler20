/**
* @file expressions.c
* @author xkrukh00
* @date 03.11.2020
* @brief header file for expresions.c
*/

#ifndef _EXPRESSIONS_H
#define _EXPRESSIONS_H

#include "scanner.h"
#include "stdbool.h"
#include "error.h"
#include "precstack.h"

/* Precedence TAble Indexes */
typedef enum {  
    I_MUL_DIV,	        // 0 /*
    I_ADD_SUB,		    // 1 +-
    I_REL,			    // 2 rel (relation operators) < <= > >= == !=    
    I_DATA,				// 3 id (id, int, double, string)
    I_LEFT_BRACKET,		// 4 (
    I_RIGHT_BRACKET,	// 5 )
    I_DOLLAR			// 6 $
} tPrecTabIndex;


/* Precedence Items */
typedef enum precTabItems{
    
    /* Data Types */
    IDENTIFIER,		// 0 ID
    INT_NUMBER,		// 1 int
    DOUBLE_NUMBER,	// 2 double
    STRING,			// 3 string
    NIL,            // 4 nil

    /* Mathematical Operators */
    ADD,			// 5 +
    SUB,			// 6 -
    MUL,			// 7 *
    DIV,			// 8 /

    /* Relation Operators */
    EQ,				// 9 ==
    HEQ,			// 13 >=   
    HTN,			// 14 >  
    LEQ,			// 11 <=  
    LTN,			// 12 <         
    NEQ,			// 10 !=

    /* Braces */
    LEFT_BRACKET,	// 15 (
    RIGHT_BRACKET,	// 16 )

    /* Others */
    DOLLAR,			// 17 $
    REDUCE,			// 18 for rule reduce
    NON_TERM,		// 19 non-terminal
    STOP            //

} tPrecTabItem;


typedef enum
{
    NOT_E_RULE,		// 0 rule doesn't exist
    E_OPERAND,		// 1 E -> i
    LPAR_E_RPAR,    // 2 E -> (E)
    E_PLUS_E,		// 3 E -> E + E
    E_MINUS_E,	    // 4 E -> E - E
    E_MUL_E,		// 5 E -> E * E
    E_DIV_E,		// 6 E -> E / E
    E_EQ_E,		    // 7 E -> E == E
    E_NEQ_E,		// 8 E -> E != E
    E_LEQ_E,		// 9 E -> E <= E
    E_LTN_E,		// 10 E -> E < E
    E_MEQ_E,		// 11 E -> E => E
    E_MTN_E         // 12 E -> E > E
} PrecRules;


/**
 * Get precedence table index by precedence table item.
 * 
 * @return tPrecTabIntex index for precedence table.
*/
tPrecTabIndex getPrecTabIndex(tPrecTabItem precItem);


/**
 * Converts token.token_type to precedence item.
 * 
 * @return tPrecTabItem precedence item. NON_TERM if unexpected token.
*/
tPrecTabItem tkn2precItem();


/**
 * Converts type of token to tDataType
 * 
 * @return tDataType equivalent, UNDEFINED_TYPE if equivalent is not found
*/
tDataType tokenType2DataType();


/**
 * get precedence from precedence table
*/
char precedence(tPrecTabIndex topTerm, tPrecTabIndex inTerm);


/**
 * reduce by rule.
 * 
 * @return exit code.
*/
int reduce();


/**
 * <expression> rule.
 * 
 * @return exit code.
*/
int expessions();


#endif