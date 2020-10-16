/**
* @file scanner.h
* @author xproko40
* @date 16.10.2020
* @brief The header file for scanner.c
*/

#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>

#include "dynamicStr.h"
//#include "stackIndent.h"

//StackIndent IStack;

/* Scaner states */
typedef enum
{
    /* FINAL STATES */
    STATE_START,                 //  IN SCANNER // Starting state every new token processing starts here and initializes other states
    STATE_INTEGER,               //  IN SCANNER // Start of number processing, accepts numbers, e/E and . | Can return integer number
    STATE_DOUBLE,                //  IN SCANNER // The last symbol was number | Can return double number
    STATE_DOUBLE_EXPONENT,       //  IN SCANNER // Returns double number with exponent
    STATE_IDENTIFIER_OR_KEYWORD, //  IN SCANNER // Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID
    STATE_STRING,                //  IN SCANNER // Sequence " was read, ends with " | Returns string
    STATE_LESS,                  //  IN SCANNER // Starts with < | Returns < or <=
    STATE_HIGHER,                //  IN SCANNER // Starts with > | Returns > or >=
    STATE_EOL,                   //  IN SCANNER // End of line
    STATE_ASSIGN,                //  IN SCANNER // Starts with = | Returns = or ==
    STATE_DIV,                   //  IN SCANNER // Starts with / / Returns /


    /* SATES */
    STATE_SCREAMER,                //  IN SCANNER // "!" can return token !=
    STATE_COLON,                   //  IN SCANNER // ":" can return token :=
    STATE_STRING_COMMENTARY,       //  IN SCANNER // Line commentary, ignores every symbol, ends with EOL
    STATE_BLOCK_COMMENTARY,        //  IN SCANNER // Starts with / except * and ignores every symbol except * - this might be the starting end of block comment
    STATE_BLOCK_COMMENTARY_LEAVE,  //  IN SCANNER // Starts with * except / next  -> go to start
    STATE_NUMBER_POINT,            //  IN SCANNER // If symbol was ., the number has type double
    STATE_NUMBER_EXPONENT,         //  IN SCANNER // The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
    STATE_NUMBER_EXPONENT_SIGN,    //  IN SCANNER // Optional symbol was read, continue with numbers only
    STATE_STRING_START,            //  IN SCANNER // String starts with "
    STATE_STRING_ESCAPE,           //  IN SCANNER // If symbol \ was loaded, can replace char with escape sequence symbols
    STATE_STRING_DIGIT_xX,         //  IN SCANNER // first decimal beginning xA
    STATE_STRING_DIGIT_xXX,        //  IN SCANNER // decimal xAF

} STATES;

/* Tokens */
typedef enum
{
    /* Main task */
    KEYWORD_ELSE,   // else
    KEYWORD_FLOAT64,     // float64
    KEYWORD_FOR,   // for
    KEYWORD_FUNC,   // func
    KEYWORD_IF, // if
    KEYWORD_INT,  // int
    KEYWORD_PACKAGE, // package
    KEYWORD_RETURN, // return
    KEYWORD_STRING, // string
/**
    * Additional tasks *
    * Boolop *
    KEYWORD_TRUE,  //true
    KEYWORD_FALSE, // false
    KEYWORD_NOT,   // not
    KEYWORD_AND,   // and
    KEYWORD_OR,    // or

    * Base ??? *

    * Cycles *
    KEYWORD_FOR,      // for
    KEYWORD_BREAK,    // break
    KEYWORD_CONTINUE, // continue

    * Funexp ??? *

    * Ifthen *
    KEYWORD_ELIF, // elif
    KEYWORD_IN,   // in
*/
} KEYWORDS;

typedef enum
{
    TOKEN_EOF,        /// EOL //  IN SCANNER
    TOKEN_EOL,        /// EOF //  IN SCANNER
    TOKEN_IDENTIFIER, /// Identifier
    TOKEN_KEYWORD,    /// Keyword


    TOKEN_INT,    /// Integer number //  IN SCANNER
    TOKEN_DOUBLE, /// Double number //  IN SCANNER
    TOKEN_STRING, /// String


    /* Operations */
    TOKEN_ASSIGN, // = //  IN SCANNER
    TOKEN_PLUS,   // + //  IN SCANNER
    TOKEN_MINUS,  // - //  IN SCANNER
    TOKEN_MUL,    // * //  IN SCANNER
    TOKEN_DIV,    // / //  IN SCANNER

    /* Comparison */
    TOKEN_EQUALS,          // == //  IN SCANNER
    TOKEN_HIGHER_OR_EQUAL, // >= //  IN SCANNER
    TOKEN_HIGHER,          // > //  IN SCANNER
    TOKEN_LESS_OR_EQUAL,   // <= //  IN SCANNER
    TOKEN_LESS,            // < //  IN SCANNER
    TOKEN_NOT_EQUAL,       // != //  IN SCANNER
    TOKEN_DEFINITION,      // := //

    /* */
    TOKEN_LEFT_BRACKET,  // ( //  IN SCANNER
    TOKEN_RIGHT_BRACKET, // ) //  IN SCANNER
    TOKEN_COLON,         // : //  IN SCANNER
    TOKEN_COMMA,         // , //  IN SCANNER
    TOKEN_LCURLY_BRACKET,// { //
    TOKEN_RCURLY_BRACKET,// } //


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

tToken token; // Global variable

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
