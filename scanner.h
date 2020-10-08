/**
* @file scanner.h
* @author xabram00, xzhuko01
* @date 10.11.2019
* @brief The header file for scanner.c
*/

#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>

#include "dynamicStr.h"

/* Scaner states */
typedef enum
{
    /* FINAL STATES */
    STATE_START,                 // DONE IN SCANNER // Starting state every new token processing starts here and initializes other states
    STATE_INTEGER,               // DONE IN SCANNER // Start of number processing, accepts numbers, e/E and . | Can return integer number
    STATE_DOUBLE,                // DONE IN SCANNER // The last symbol was number | Can return double number
    STATE_DOUBLE_EXPONENT,       // DONE IN SCANNER // Returns double number with exponent
    STATE_IDENTIFIER_OR_KEYWORD, // DONE IN SCANNER // Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID
    STATE_STRING,                // DONE IN SCANNER // Sequence ' was read, ends with ' | Returns string
    STATE_LESS,                  // DONE IN SCANNER // Starts with < | Returns < or <=
    STATE_MORE,                  // DONE IN SCANNER // Starts with > | Returns > or >=
    STATE_EOL,                   // DONE IN SCANNER // End of line
    STATE_SCREAMER,              // DONE IN SCANNER // "!" can return token !=
    STATE_ASSIGN,                // DONE IN SCANNER // Starts with = | Returns = or ==
    STATE_DIV,                   // DONE IN SCANNER // Starts with \ | Returns / or //

    /* SATES */
    STATE_DASH_COMMENTARY,         // DONE IN SCANNER // Line commentary, ignores every symbol, ends with EOL
    STATE_BLOCK_COMMENTARY1,       // DONE IN SCANNER // Starts with " except second " (")
    STATE_BLOCK_COMMENTARY2,       // DONE IN SCANNER // except third " ("")
    STATE_BLOCK_COMMENTARY3,       // DONE IN SCANNER // except third " (""") and ignores every symbol except " - this might be the starting end of block comment
    STATE_BLOCK_COMMENTARY_LEAVE1, // DONE IN SCANNER // Starts with " except second " (")
    STATE_BLOCK_COMMENTARY_LEAVE2, // DONE IN SCANNER // except third " ("") next " -> go to start
    STATE_NUMBER_POINT,            // DONE IN SCANNER // If symbol was ., the number has type double
    STATE_NUMBER_EXPONENT,         // DONE IN SCANNER // The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
    STATE_NUMBER_EXPONENT_SIGN,    // DONE IN SCANNER // Optional symbol was read, continue with numbers only
    STATE_STRING_START,            // DONE IN SCANNER // String starts with '
    STATE_STRING_ESCAPE,           // DONE IN SCANNER // If symbol \ was loaded, can replace char with escape sequence symbols
    STATE_STRING_DIGIT_xX,         // DONE IN SCANNER // first decimal beginning xA
    STATE_STRING_DIGIT_xXX,        // DONE IN SCANNER // decimal xAF

} STATES;

/* Tokens */
typedef enum
{
    /* Main task */
    KEYWORD_DEF,    // def
    KEYWORD_ELSE,   // else
    KEYWORD_IF,     // if
    KEYWORD_NONE,   // None
    KEYWORD_PASS,   // pass
    KEYWORD_RETURN, // return
    KEYWORD_WHILE,  // while

    /* Additional tasks */

    /* Boolop */
    KEYWORD_TRUE,  //true
    KEYWORD_FALSE, // false
    KEYWORD_NOT,   // not
    KEYWORD_AND,   // and
    KEYWORD_OR,    // or

    /* Base ??? */

    /* Cycles */
    KEYWORD_FOR,      // for
    KEYWORD_BREAK,    // break
    KEYWORD_CONTINUE, // continue

    /* Funexp ??? */

    /* Ifthen */
    KEYWORD_ELIF, // elif

} KEYWORDS;

typedef enum
{
    TOKEN_EOF,        /// EOL // DONE IN SCANNER
    TOKEN_EOL,        /// EOF // DONE IN SCANNER
    TOKEN_EMPTY,      /// Empty // DONE IN SCANNER
    TOKEN_IDENTIFIER, /// Identifier
    TOKEN_KEYWORD,    /// Keyword

    TOKEN_INT,    /// Integer number // DONE IN SCANNER
    TOKEN_DOUBLE, /// Double number // DONE IN SCANNER
    TOKEN_STRING, /// String

    /* Operations */
    TOKEN_ASSIGN, // = // DONE IN SCANNER
    TOKEN_PLUS,   // + // DONE IN SCANNER
    TOKEN_MINUS,  // - // DONE IN SCANNER
    TOKEN_MUL,    // * // DONE IN SCANNER
    TOKEN_DIV,    // \ // DONE IN SCANNER
    TOKEN_IDIV,   // \\ // DONE IN SCANNER

    /* Comparison */
    TOKEN_EQUALS,          // == // DONE IN SCANNER
    TOKEN_HIGHER_OR_EQUAL, // >= // DONE IN SCANNER
    TOKEN_HIGHER,          // > // DONE IN SCANNER
    TOKEN_LESS_OR_EUQAL,   // <= // DONE IN SCANNER
    TOKEN_LESS,            // < // DONE IN SCANNER
    TOKEN_NOT_EQUAL,       // != // DONE IN SCANNER

    /* */
    TOKEN_LEFT_BRACKET,  // ( // DONE IN SCANNER
    TOKEN_RIGHT_BRACKET, // ) // DONE IN SCANNER
    TOKEN_COLON,         // : // DONE IN SCANNER
    TOKEN_COMMA          // , // DONE IN SCANNER

} TOKENS;

typedef union {
    DYNAMIC_STRING *value_string;
    int value_int;
    double value_double;
    KEYWORDS keyword;
} tAttribute;

typedef struct
{
    TOKENS token_type;
    tAttribute attribute;
    char *id;
} tToken;
/**
* Sets source file to be scanned.
*
* @param file Pointer to source file.
*/
void setSourceFile(FILE *file);

/**
* Sets dynamic string to be written into.
*
* @param string Pointer to dynamic string.
*/
void dynamicStrSet(DYNAMIC_STRING *str);

/**
 * Translates the enumeration value into a string
 *
 * @param token
 * @return string Name of the token
 */
const char *getTokenName(TOKENS token);

/**
* This is the main function of scanner, scans token after token and sends it further.
*
* @param token Pointer to output token.
* @return 0 (TOKEN_OK) if token is OK, otherwise in case of lex error one of ERROR_... constant.
*/
int getToken(tToken *token);

#endif
