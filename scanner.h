/**
* @file scanner.h
* @author xproko40
* @date 16.10.2020
* @brief The header file for scanner.c
*/

#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "dynamicStr.h"


/* Scaner states */
typedef enum
{
    /* FINAL STATES */
    STATE_START,                 //  DONE IN SCANNER // Starting state every new token processing starts here and initializes other states
    STATE_INTEGER,               //  DONE IN SCANNER // Start of number processing, accepts numbers, e/E and . | Can return integer number
    STATE_DOUBLE,                //  DONE IN SCANNER // The last symbol was number | Can return double number
    STATE_DOUBLE_EXPONENT,       //  DONE IN SCANNER // Returns double number with exponent
    STATE_IDENTIFIER_OR_KEYWORD, //  DONE IN SCANNER // Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID
    STATE_STRING,                //  DONE IN SCANNER // Sequence " was read, ends with " | Returns string
    STATE_LESS,                  //  DONE IN SCANNER // Starts with < | Returns < or <=
    STATE_HIGHER,                //  DONE IN SCANNER // Starts with > | Returns > or >=
    STATE_EOL,                   //  DONE IN SCANNER // End of line
    STATE_ASSIGN,                //  DONE IN SCANNER // Starts with = | Returns = or ==
    STATE_DIV,                   //  DONE IN SCANNER // Starts with / / Returns /


    /* SATES */
    STATE_SCREAMER,                //  DONE IN SCANNER // "!" can return token !=
    STATE_COLON,                   //  DONE IN SCANNER // ":" can return token :=
    STATE_STRING_COMMENTARY,       //  DONE IN SCANNER // Line commentary, ignores every symbol, ends with EOL
    STATE_BLOCK_COMMENTARY,        //  DONE IN SCANNER // Starts with / except * and ignores every symbol except * - this might be the starting end of block comment
    STATE_BLOCK_COMMENTARY_LEAVE,  //  DONE IN SCANNER // Starts with * except / next  -> go to start
    STATE_NUMBER_POINT,            //  DONE IN SCANNER // If symbol was ., the number has type double
    STATE_NUMBER_EXPONENT,         //  DONE IN SCANNER // The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
    STATE_NUMBER_EXPONENT_SIGN,    //  DONE IN SCANNER // Optional symbol was read, continue with numbers only
    STATE_STRING_START,            //  DONE IN SCANNER // String starts with "
    STATE_STRING_ESCAPE,           //  DONE IN SCANNER // If symbol \ was loaded, can replace char with escape sequence symbols
    STATE_STRING_DIGIT_xX,         //  DONE IN SCANNER // first decimal beginning xA
    STATE_STRING_DIGIT_xXX,        //  DONE IN SCANNER // decimal xAF

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
    KEYWORD_BOOLEAN,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    KEYWORD_NOT,
    KEYWORD_AND,
    KEYWORD_OR,
} KEYWORDS;

typedef enum
{
    TOKEN_EOF,        /// EOL //   DONE IN SCANNER
    TOKEN_EOL,        /// EOF //   DONE IN SCANNER
    TOKEN_EMPTY,      /// Empty // DONE IN SCANNER
    TOKEN_IDENTIFIER, /// Identifier // DONE IN SCANNER
    TOKEN_KEYWORD,    /// Keyword //    DONE IN SCANNER


    TOKEN_INT,    /// Integer number // DONE IN SCANNER
    TOKEN_FLOAT64, /// Double number //  DONE IN SCANNER
    TOKEN_STRING, /// String //         DONE IN SCANNER
    TOKEN_NIL,


    /* Operations */
    TOKEN_ASSIGN, // = //  DONE IN SCANNER
    TOKEN_PLUS,   // + //  DONE IN SCANNER
    TOKEN_MINUS,  // - //  DONE IN SCANNER
    TOKEN_MUL,    // * //  DONE IN SCANNER
    TOKEN_DIV,    // / //  DONE IN SCANNER

    /* Comparison */
    TOKEN_EQUALS,          // == // DONE IN SCANNER
    TOKEN_HIGHER_OR_EQUAL, // >= // DONE IN SCANNER
    TOKEN_HIGHER,          // > //  DONE IN SCANNER
    TOKEN_LESS_OR_EQUAL,   // <= // DONE IN SCANNER
    TOKEN_LESS,            // < //  DONE IN SCANNER
    TOKEN_NOT_EQUAL,       // != // DONE IN SCANNER
    TOKEN_DEFINITION,      // := // PROBABLY DONE IN SCANNER

    /* */
    TOKEN_LEFT_BRACKET,  // ( //  DONE IN SCANNER
    TOKEN_RIGHT_BRACKET, // ) //  DONE IN SCANNER
    TOKEN_COMMA,         // , //  DONE IN SCANNER
    TOKEN_LCURLY_BRACKET,// { //  DONE IN SCANNER
    TOKEN_RCURLY_BRACKET,// } //  DONE IN SCANNER
    TOKEN_SEMICOLON,     // ; //  DONE IN SCANNER

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

//tToken token; // Global variable

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

extern FILE *source;             // Source file that will be scanned
extern tToken token;


#endif