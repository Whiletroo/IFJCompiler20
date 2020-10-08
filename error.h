/**
 * @file error.h
 * @author xabram00
 * @date 09.11.2019
 * @brief File containing definitions of possible errors that can occur in compiler
*/

//TODO!!!!!!!!!

#ifndef ERRORS
#define ERRORS

#define OK 0

#define LEX_ERROR 1             /// Lex structure error.
#define SYNTAX_ERR 2            /// Syntax error.
#define SEM_ERR_UNDEFINED_VAR 3 /// Semantic error - undefined variable.
#define SEM_ERR_TYPE_COMPAT 4   /// Semantic error - types uncompatible.
#define SEM_ERR_EXP_COMPAT 5
#define SEM_ERR_PAR_COUNT 6     /// Semantic error - wrong count of parameters
#define SEM_ERR_OTHER 7         /// Semantic error - other.
#define ZERO_ERROR 9            /// Division by zero.

//OUR ERRORS
#define ERR_INTERNAL 99 //interní chyba interpretu tj. neovlivněná vstupním programem (např. chyba alokace paměti, chyba při otvírání souboru s řídicím programem atd.).
#define ERR_ARGS 50     //chybně zadané vstupní parametry na příkazovém řádku při spouštění interpretu.
#define ERR_ANAL 51
#define ERR_SEM 52
#define ERR_BAD_OPPER 53
#define ERR_NOVAR 54

#endif