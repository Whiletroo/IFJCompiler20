/**
* @file expressions.c
* @author xkrukh00
* @date 03.11.2020
* @brief header file for expresions.c
*/

#ifndef _EXPRESSIONS_H
#define _EXPRESSIONS_H

typedef enum
{
    I_PLUS_MINUS,		/// 0 +-
    I_MUL_DIV_IDIV,	    /// 1 */ //
    I_REL_OP,			/// 2 r (relation operators) < <= > >= == !=
    I_LEFT_BRACKET,		/// 3 (
    I_RIGHT_BRACKET,	/// 4 )
    I_DATA,				/// 5 i (id, int, double, string)
    I_DOLLAR			/// 6 $ �� ���� ��!!!!!!!
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
    NIL,

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
    E_IDIV_E,		/// 4 E -> E // E �� ����
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

#endif