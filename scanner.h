/**
* @file scanner.h
* @author xproko40
* @date 09.12.2020
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
    STATE_START,                 // Starting state every new token processing starts here and initializes other states
    STATE_INTEGER,               // Start of number processing, accepts numbers, e/E and . | Can return integer number
    STATE_DOUBLE,                // The last symbol was number | Can return double number
    STATE_DOUBLE_EXPONENT,       // Returns double number with exponent
    STATE_IDENTIFIER_OR_KEYWORD, // Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID
    STATE_STRING,                // Sequence " was read, ends with " | Returns string
    STATE_LESS,                  // Starts with < | Returns < or <=
    STATE_HIGHER,                // Starts with > | Returns > or >=
    STATE_EOL,                   // End of line
    STATE_ASSIGN,                // Starts with = | Returns = or ==
    STATE_DIV,                   // Starts with / / Returns /


    /* SATES */
    STATE_SCREAMER,                // "!" can return token !=
    STATE_COLON,                   // ":" can return token :=
    STATE_STRING_COMMENTARY,       // Line commentary, ignores every symbol, ends with EOL
    STATE_BLOCK_COMMENTARY,        // Starts with / except * and ignores every symbol except * - this might be the starting end of block comment
    STATE_BLOCK_COMMENTARY_LEAVE,  // Starts with * except / next  -> go to start
    STATE_NUMBER_POINT,            // If symbol was ., the number has type double
    STATE_NUMBER_EXPONENT,         // The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
    STATE_NUMBER_EXPONENT_SIGN,    // Optional symbol was read, continue with numbers only
    STATE_STRING_START,            // String starts with "
    STATE_STRING_ESCAPE,           // If symbol \ was loaded, can replace char with escape sequence symbols
    STATE_STRING_DIGIT_xX,         // first decimal beginning xA
    STATE_STRING_DIGIT_xXX,        // decimal xAF

} STATES;

/* Tokens */
typedef enum
{
    KEYWORD_ELSE,   // else
    KEYWORD_FLOAT64,     // float64
    KEYWORD_FOR,   // for
    KEYWORD_FUNC,   // func
    KEYWORD_IF, // if
    KEYWORD_INT,  // int
    KEYWORD_PACKAGE, // package
    KEYWORD_RETURN, // return
    KEYWORD_STRING, // string
    KEYWORD_BOOLEAN,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    KEYWORD_NOT,
    KEYWORD_AND,
    KEYWORD_OR,
    KEYWORD_PRINT,
    KEYWORD_INPUTS,
    KEYWORD_INPUTI,
    KEYWORD_INPUTF,
    KEYWORD_INT2FLOAT,
    KEYWORD_FLOAT2INT,
    KEYWORD_LEN,
    KEYWORD_SUBSTR,
    KEYWORD_ORD,
    KEYWORD_CHR,
} KEYWORDS;

typedef enum
{
    TOKEN_EOF,        // EOL
    TOKEN_EOL,        // EOF
    TOKEN_EMPTY,      // Empty
    TOKEN_KEYWORD,    // Keyword
    TOKEN_IDENTIFIER, // Identifier

    TOKEN_INT,    // Integer number
    TOKEN_FLOAT64, // Double number
    TOKEN_STRING, // String
    TOKEN_BOOL,
    TOKEN_NIL,


    /* Operations */
    TOKEN_ASSIGN, // = //
    TOKEN_PLUS,   // + //
    TOKEN_MINUS,  // - //
    TOKEN_MUL,    // * //
    TOKEN_DIV,    // / //

    /* Comparison */
    TOKEN_EQUALS,          // == //
    TOKEN_HIGHER_OR_EQUAL, // >= //
    TOKEN_HIGHER,          // > //
    TOKEN_LESS_OR_EQUAL,   // <= //
    TOKEN_LESS,            // < //
    TOKEN_NOT_EQUAL,       // != //
    TOKEN_DEFINITION,      // := //

    /* */
    TOKEN_LEFT_BRACKET,  // ( //
    TOKEN_RIGHT_BRACKET, // ) //
    TOKEN_COMMA,         // , //
    TOKEN_LCURLY_BRACKET,// { //
    TOKEN_RCURLY_BRACKET,// } //
    TOKEN_SEMICOLON,     // ; //

} TOKENS;

typedef union {
    DYN_STRING *value_string;
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

extern FILE *source;             // Source file that will be scanned
extern tToken token;
extern DYN_STRING *d_string;


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
void dynamicStrSet(DYN_STRING *str);

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
