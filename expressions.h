/**
* @file expressions.c
* @author xkrukh00
* @date 03.11.2020
* @brief header file for expresions.c
*/

#ifndef _EXPRESSIONS_H
#define _EXPRESSIONS_H

#include "scanner.h"


/* Precedence TAble Indexes */
typedef enum {  
    I_MUL_DIV,	        // 0 /*
    I_ADD_SUB,		    // 1 +-
    I_REL,			    // 2 rel (relation operators) < <= > >= == !=    
    I_DATA,				// 3 id (id, int, double, string)
    I_LEFT_BRACKET,		// 4 (
    I_RIGHT_BRACKET,	// 5 )
    I_DOLLAR			// 6 $
} tPrecTableIndex;


/* Precedence Items */
enum PrecTabItem {
    
    /* Data Types */
    IDENTIFIER,		// 0 ID
    INT_NUMBER,		// 1 int
    DOUBLE_NUMBER,	// 2 double
    STRING,			// 3 string
    NIL,

    /* Mathematical Operators */
    ADD,			// 4 +
    SUB,			// 5 -
    MUL,			// 6 *
    DIV,			// 7 /

    /* Relation Operators */
    EQ,				// 8 ==
    NEQ,			// 9 !=
    LEQ,			// 10 <=
    LTN,			// 11 <
    HEQ,			// 12 >=
    HTN,			// 113 >

    /* Braces */
    LEFT_BRACKET,	// 14 (
    RIGHT_BRACKET,	// 15 )

    /* Others */
    DOLLAR,			// 16 $
    REDUCE,			// 17 stop symbol used when reducing
    NON_TERM,		// 18 non-terminal
};


typedef enum
{
    E_OPERAND,		// 0 E -> i
    LPAR_E_RPAR,    // 1 E -> (E)
    E_PLUS_E,		// 2 E -> E + E
    E_MINUS_E,	    // 3 E -> E - E
    E_MUL_E,		// 4 E -> E * E
    E_DIV_E,		// 5 E -> E / E
    E_EQ_E,		    // 6 E -> E == E
    E_NEQ_E,		// 7 E -> E != E
    E_LEQ_E,		// 8 E -> E <= E
    E_LTN_E,		// 9 E -> E < E
    E_MEQ_E,		// 10 E -> E => E
    E_MTN_E,		// 11 E -> E > E

    NOT_E_RULE		// 12 rule doesn't exist
} PrecRules;


const char precedence_table[7][7] = {
/*         /*   +- rel  id  (   )   $       */
/* /*  */ {'>', '>', '>', '<', '<', '>', '>'},
/* + - */ {'<', '>', '>', '<', '<', '>', '>'},
/* rel */ {'<', '<', ' ', '<', '<', '>', '>'},
/* id  */ {'>', '>', '>', ' ', ' ', '>', '>'},
/*  )  */ {'<', '<', '<', '<', '<', '=', '>'},
/*  )  */ {'>', '>', '>', ' ', ' ', '>', '>'},
/*  $  */ {'<', '<', '<', '<', '<', '<', ' '}
};

/*
1. ADT стак для PrecTableItem
2. функция для ковертирования токена на PrecTabItem
3. функция для поучения precedence ( < > = )
*/


tPrecTableIndex token2precTableIndex();

char get_precedence();

/****** ADT stack for Precedence analysis    *******/
/* Stack with linked items. Here is access to items
* under top of stack, every item have right and left
* pointers to neightbo. This is needed to be able to 
* get to the first non-terminal in the Stack. A stack 
* is essentially a Doubly linked list              */

// Init Stack
// Push PrecS Stack
// 
// Pop Stack
// Dispose Stack
// Find First non-terminal in Stack



#endif