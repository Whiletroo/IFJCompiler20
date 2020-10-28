/**
 * @file error.h
 * @author xkrukh00
 * @date 08.10.2020
 * @brief
*/

//TODO!!!!!!!!!

#ifndef ERRORS
#define ERRORS

#define OK 0

#define LEX_ERROR 1             // Lex structure error.
#define SYNTAX_ERR 2            // Syntax error.
#define SEM_ERR_UNDEFINED_VAR 3 // Semantic error - undefined variable or function.
#define SEM_ERR_TYPE_COMPAT 4   // Semantic error - TODO
#define SEM_ERR_EXP_COMPAT 5	// Semantic error - incompatible types in expression
#define SEM_ERR_PAR_COUNT 6     // Semantic error - wrong count of parameters
#define SEM_ERR_OTHER 7         // Semantic error - other.
#define ZERO_ERROR 9            // Division by zero.
#define ERR_INTERNAL 99 	// Internal error

#endif
