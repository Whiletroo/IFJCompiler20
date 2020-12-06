/**
* @file expressions.h
* @author xkrukh00
* @date 03.11.2020
* @brief header file for expresions.c
*/

#ifndef _EXPRESSIONS_H
#define _EXPRESSIONS_H

#include "symtable.h"
#include "parser.h"
#include "gena.h"

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
typedef enum {
    
    /* Data Types */
    IDENTIFIER,		//  ID
    INT_NUMBER,		//  int
    FLOAT64_NUMBER,	//  double
    STRING,			//  string
    BOOLEAN,         //
    NIL,            //  nil

    /* Mathematical Operators */
    ADD,			//  +
    SUB,			//  -
    MUL,			//  *
    DIV,			//  /

    /* Relation Operators */
    EQ,				//  ==
    HEQ,			//  >=   
    HTN,			//  >  
    LEQ,			//  <=  
    LTN,			//  <         
    NEQ,			//  !=

    /* Braces */
    LEFT_BRACKET,	//  (
    RIGHT_BRACKET,	//  )

    /* Others */
    DOLLAR,			//  $
    REDUCE,			//  for rule reduce
    NON_TERM,		//  non-terminal
    STOP            //

}tPrecTabItem;


typedef enum {
    NOT_E_RULE,		// 0 rule doesn't exist
    E_OPERAND,		// 1 E -> i
    LBRCT_E_RBRCT,  // 2 E -> (E)
    E_PLUS_E,		// 3 E -> E + E
    E_MINUS_E,	    // 4 E -> E - E
    E_MUL_E,		// 5 E -> E * E
    E_DIV_E,		// 6 E -> E / E
    E_EQ_E,		    // 7 E -> E == E
    E_HEQ_E,		// 8 E -> E => E 
    E_HTN_E,        // 9 E -> E > E
    E_LEQ_E,		// 10 E -> E <= E   
    E_LTN_E,		// 11 E -> E < E     
    E_NEQ_E,	    // 12 E -> E != E
    E_IDIV_E        // 13 E -> E idiv E
} tPrecRules;


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
 * Get rule by items in stack
*/
tPrecRules getRule();


/**
 * Sematic analyse TODO
 * 
 * @return exit code
*/
int sematic(tPrecRules rule, tDataType *expType);


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