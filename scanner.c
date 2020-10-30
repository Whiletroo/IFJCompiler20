/**
* @file scanner.c
* @author xproko40
* @date 22.10.2020
* @brief The scanner
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "error.h"
#include "dynamicStr.h"

FILE *source;             // Source file that will be scanned
DYN_STRING *d_string; // Dynamic string that will be written into

int DocToString = 0;      // Global variable to contro transformation of Documentary string to ordinary string

char sEOL[] = "\\012";
char sTAB[] = "\\011";
char sSPC[] = "\\032";

/**
* Free resources and returns exit code.
*
* @param exit_code Exit code.
* @param str Dynamic string to be freed.
* @return Given exit code.
*/
static int freeResources(int exit_code, DYN_STRING *str)
{
    dynamicStrFree(str);
    return exit_code;
}

/**
* Sets source file to be scanned.
*
* @param file Pointer to source file.
*/
void setSourceFile(FILE *file)
{
    source = file;
}

/**
* Sets dynamic string to be written into.
*
* @param string Pointer to dynamic string.
*/
void dynamicStrSet(DYN_STRING *str)
{
    d_string = str;
}

/**
* Transform dynamic string to integer and write in to token value
*
* @param str pointer on dynamic string
* @param token pointer on token
* @return Given exit code.
*/
static int processInteger(DYN_STRING *str, tToken *token)
{
    char *endptr;

    int val = (int)strtol(str->str, &endptr, 10);
    if (*endptr)
    {
        return freeResources(ERR_INTERNAL, str);
    }

    token->attribute.value_int = val;
    token->token_type = TOKEN_INT;
    return freeResources(OK, str);
}

/**
* Transform dynamic string to double and write in to token value
*
* @param str pointer on dynamic string
* @param token pointer on token
* @return Given exit code.
*/
static int processDecimal(DYN_STRING *str, tToken *token)
{
    char *endptr;

    double val = strtod(str->str, &endptr);
    if (*endptr)
    {
        return freeResources(ERR_INTERNAL, str);
    }

    token->attribute.value_double = val;
    token->token_type = TOKEN_DOUBLE;

    return freeResources(OK, str);
}

/**
* Transform dynamic string to Identifire or Keyword and write it in Token
*
* @param str pointer on dynamic string
* @param token pointer on token
* @return Given exit code.
*/
int processIdentifier(DYN_STRING *str, tToken *token)
{
    /* main sequence of conditions to verify the dynamic string on keywords in it */
    if (!dynamicStrCompareConstString(str, "else"))
    {
        token->attribute.keyword = KEYWORD_ELSE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "float64"))
    {
        token->attribute.keyword = KEYWORD_FLOAT64;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "for"))
    {
        token->attribute.keyword = KEYWORD_FOR;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "func"))
    {
        token->attribute.keyword = KEYWORD_FUNC;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "if"))
    {
        token->attribute.keyword = KEYWORD_IF;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "int"))
    {
        token->attribute.keyword = KEYWORD_INT;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "package"))
    {
        token->attribute.keyword = KEYWORD_PACKAGE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "return"))
    {
        token->attribute.keyword = KEYWORD_RETURN;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "string"))
    {
        token->attribute.keyword = KEYWORD_STRING;
        token->token_type = TOKEN_KEYWORD;
    }

/**
    else if (!dynamicStrCompareConstString(str, "not"))
    {
        token->attribute.keyword = KEYWORD_NOT;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "and"))
    {
        token->attribute.keyword = KEYWORD_AND;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "or"))
    {
        token->attribute.keyword = KEYWORD_OR;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "for"))
    {
        token->attribute.keyword = KEYWORD_FOR;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "break"))
    {
        token->attribute.keyword = KEYWORD_BREAK;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "continue"))
    {
        token->attribute.keyword = KEYWORD_CONTINUE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "elif"))
    {
        token->attribute.keyword = KEYWORD_ELIF;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "in"))
    {
        token->attribute.keyword = KEYWORD_IN;
        token->token_type = TOKEN_KEYWORD;
    }
*/
        /* If the dynamic line does not contain keywords, then the word located in it is an identifier */
    else
    {
        token->token_type = TOKEN_IDENTIFIER;
        if (!dynamicStrCopyString(token->attribute.value_string, str))
        {
            return freeResources(ERR_INTERNAL, str);
        }
    }
    return freeResources(OK, str);
}

/**
* Additional function to decode the numerical form of the token to the token name
*
* @param token actual token
* @return Token_name or 0
*/
const char *getTokenName(TOKENS token)
{
    switch (token)
    {
        case TOKEN_EOF:
            return "EOF";
        case TOKEN_EOL:
            return "EOL";
        case TOKEN_EMPTY:
            return "EMPTY";
        case TOKEN_IDENTIFIER:
            return "Identifier";
        case TOKEN_KEYWORD:
            return "Keyword";
        case TOKEN_INT:
            return "Int";
        case TOKEN_DOUBLE:
            return "Double";
        case TOKEN_STRING:
            return "String";
        case TOKEN_ASSIGN:
            return "Assign";
        case TOKEN_PLUS:
            return "Plus";
        case TOKEN_MINUS:
            return "Minus";
        case TOKEN_MUL:
            return "Multiplication";
        case TOKEN_DIV:
            return "Divison";
        case TOKEN_EQUALS:
            return "Equals";
        case TOKEN_HIGHER_OR_EQUAL:
            return "Higher or equal";
        case TOKEN_HIGHER:
            return "Higher";
        case TOKEN_LESS_OR_EQUAL:
            return "Less or equal";
        case TOKEN_LESS:
            return "Less";
        case TOKEN_NOT_EQUAL:
            return "Not equal";
        case TOKEN_DEFINITION:
            return "Definition";
        case TOKEN_LEFT_BRACKET:
            return "Left bracket";
        case TOKEN_RIGHT_BRACKET:
            return "Right bracket";
        case TOKEN_COMMA:
            return "Comma";
        case TOKEN_LCURLY_BRACKET:
            return "Left curly bracket";
        case TOKEN_RCURLY_BRACKET:
            return "Right curly bracket";
         case TOKEN_NIL:
            return "Nil";
        case TOKEN_NONE:
            return "None";
    }
    return 0;
}

/**
* This is the main function of scanner, scans token after token and sends it further.
*
* @param token Pointer to output token.
* @return 0 (TOKEN_OK) if token is OK, otherwise in case of lex error one of ERROR_... constant.
*/
int getToken(tToken *token)
{
    /* Check source file existence */
    if (source == NULL)
    {
        return ERR_INTERNAL;
    }

    /* Check dynamic string existence */
    if (d_string == NULL)
    {
        return ERR_INTERNAL;
    }

    token->attribute.value_string = d_string;

    DYN_STRING string;
    DYN_STRING *str = &string;

    /* DynamicString inicialization */
    if (!dynamicStrInit(str))
    {
        return ERR_INTERNAL;
    }

    dynamicStrClear(str);

    char c;
    char hexValue[3] = {0};
    int state = STATE_START;
    token->token_type = TOKEN_EMPTY;

    while (1)
    {
        c = (char)getc(source);
        switch (state)
        {
            /*          STATE_START            */
            /* Initial position of the machine */
            case (STATE_START):
                /* If first symbol is EOL:                   */
                /*   1) send TOKEN - EOL                     */
                /*   3) return with function freeResources   */
                if (c == '\n')
                {
                    token->token_type = TOKEN_EOL;
                    return freeResources(OK, str);
                }

                else if (c == '!')
                {
                    state = STATE_SCREAMER;
                }
                    /* If first symbol is LESS:                                   */
                    /*    1) change state to STATE_LESS and analyze less sequence */
                else if (c == '<')
                {
                    state = STATE_LESS;
                }
                    /* If first symbol is MORE:                                   */
                    /*    1) change state to STATE_MORE and analyze more sequence */
                else if (c == '>')
                {
                    state = STATE_HIGHER;
                }
                    /* If first symbol is ASSIGN:                                     */
                    /*    1) change state to STATE_ASSIGN and analyze assign sequence */
                else if (c == '=')
                {
                    state = STATE_ASSIGN;
                }
                    /* If first symbol is COLON */
                    /*  1) change state to STATE_COLON and analyze colon sequence*/
                else if (c == ':')
                {
                    state = STATE_COLON;
                }
                    /* If first symbol is PLUS:                */
                    /*   1) send TOKEN - PLUS                  */
                    /*   2) return with function freeResources */
                else if (c == '+')
                {
                    token->token_type = TOKEN_PLUS;
                    return freeResources(OK, str);
                }
                    /* If first symbol is MINUS:               */
                    /*   1) send TOKEN - MINUS                 */
                    /*   2) return with function freeResources */
                else if (c == '-')
                {
                    token->token_type = TOKEN_MINUS;
                    return freeResources(OK, str);
                }
                    /* If first symbol is MUL:                 */
                    /*   1) send TOKEN - MUL                   */
                    /*   2) return with function freeResources */
                else if (c == '*')
                {
                    token->token_type = TOKEN_MUL;
                    return freeResources(OK, str);
                }
                    /* If first symbol is DIV:                                 */
                    /*   1) add symbol to dynamic string                       */
                    /*   2) change state to STATE_DIV and analize div sequence */
                else if (c == '/')
                {
                    if (!dynamicStrAddChar(str, (char)(c)))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_DIV;
                }
                else if (c == '{')
                {
                    token->token_type = TOKEN_LCURLY_BRACKET;
                    return freeResources(OK, str);
                }

                else if (c == '}')
                {
                    token->token_type = TOKEN_RCURLY_BRACKET;
                    return freeResources(OK, str);
                }

                    /* If first symbol is LEFT_BRACKET:        */
                    /*   1) send TOKEN - LEFT_BRACKET          */
                    /*   2) check next symbol in case of "     */
                    /*   3) return with function freeResources */
                else if (c == '(')
                {
                    c = (char)getc(source);
                    if (c == '\"')
                    {
                        DocToString = 1;
                    }
                    ungetc(c, source);
                    token->token_type = TOKEN_LEFT_BRACKET;
                    return freeResources(OK, str);
                }
                    /* If first symbol is RIGHT_BRACKET:       */
                    /*   1) send TOKEN - RIGHT_BRACKET         */
                    /*   2) return with function freeResources */
                else if (c == ')')
                {
                    token->token_type = TOKEN_RIGHT_BRACKET;
                    return freeResources(OK, str);
                }
                    /* If first symbol is COMMA:               */
                    /*   1) send TOKEN - COMMA                 */
                    /*   2) return with function freeResources */
                else if (c == ',')
                {
                    token->token_type = TOKEN_COMMA;
                    return freeResources(OK, str);
                }
                    /* If first symbol is EOF:                 */
                    /*   1) send TOKEN - EOF                   */
                    /*   2) return with function freeResources */
                else if (c == EOF)
                {
                    token->token_type = TOKEN_EOF;
                    return freeResources(OK, str);
                }
                break;

                /*              STATE_DIV               */
                /* '/' sequence, div and commentary analysis  */
            case (STATE_DIV):
                /* If next symbol is /                                                   */
                /*   1) change state to STATE_STRING_COMMENTARY and analize the sequence */
                if (c == '/')
                {
                    state = STATE_STRING_COMMENTARY;
                }
                /* If next symbol is */
                /*  1) change state to STATE_BLOCK_COMMENTARY and analize the sequence */
                if (c == '*')
                {
                    state = STATE_BLOCK_COMMENTARY;
                }
                    /* All other transition lead to div and next iteration */
                    /*   1) send TOKEN - TOKEN_DIV                         */
                    /*   2) unget symbol                                   */
                    /*   3) return with function freeResources             */
                else
                {
                    ungetc(c, source);
                    token->token_type = TOKEN_DIV;
                }
                return freeResources(OK, str);

                /*  STATE_STRING_COMMENTARY    */
                /* analyse sequence           */
            case (STATE_STRING_COMMENTARY):
                /* If next symbol is EOL:                 */
                /*    If DocToString == 0:                */
                /*      1) set state on start             */
                /*      2) set variable First symbol on 1 */
                if (c == '\n' || c == EOF)
                {
                    state = STATE_START;
                    if (c == EOF)
                        ungetc(c, source);
                }
                break;

            case (STATE_BLOCK_COMMENTARY):
                if (c == '*')
                {
                    state = STATE_BLOCK_COMMENTARY_LEAVE;
                    if (c == EOF)
                        ungetc(c, source);
                }
                break;

            case (STATE_BLOCK_COMMENTARY_LEAVE):
                if (c == '/')
                {
                    state = STATE_START;
                    if (c == EOF)
                        ungetc(c, source);
                }
                break;

                /*  STATE_SCREAMER      */
                /* analyse sequence !=  */
            case (STATE_SCREAMER):
                /* If next symbol is =:           */
                /*   1) send token NOT_EQUAL      */
                /*   2) return with freeResources */
                if (c == '=')
                {
                    token->token_type = TOKEN_NOT_EQUAL;
                    return freeResources(OK, str);
                }
                    /* All other transition lead to error */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }

                /*         STATE_LESS        */
                /* analyse sequence < or <=  */
            case (STATE_LESS):
                /* If next symbol is =:           */
                /*   1) send token LESS_OR_EUQAL  */
                /*   2) return with freeResources */
                if (c == '=')
                {
                    token->token_type = TOKEN_LESS_OR_EQUAL;
                }
                    /* If next symbol is OTHERS:      */
                    /*   1) send token TOKEN_LESS     */
                    /*   2) unget symbol              */
                    /*   2) return with freeResources */
                else
                {
                    ungetc(c, source);
                    token->token_type = TOKEN_LESS;
                }
                return freeResources(OK, str);

                /*         STATE_HIGHER        */
                /* analyse sequence > or >=  */
            case (STATE_HIGHER):
                /* If next symbol is =:            */
                /*   1) send token HIGHER_OR_EQUAL */
                /*   2) return with freeResources  */
                if (c == '=')
                {
                    token->token_type = TOKEN_HIGHER_OR_EQUAL;
                }
                    /* If next symbol is OTHERS:      */
                    /*   1) send token TOKEN_HIGHER   */
                    /*   2) unget symbol              */
                    /*   2) return with freeResources */
                else
                {
                    ungetc(c, source);
                    token->token_type = TOKEN_HIGHER;
                }
                return freeResources(OK, str);

                /*        STATE_ASSIGN       */
                /* analyse sequence = or ==  */
            case (STATE_ASSIGN):
                /* If next symbol is =:            */
                /*   1) send token EQUALS          */
                /*   2) return with freeResources  */
                if (c == '=')
                {
                    token->token_type = TOKEN_EQUALS;
                }
                    /* If next symbol is OTHERS:      */
                    /*   1) send token ASSIGN         */
                    /*   2) unget symbol              */
                    /*   2) return with freeResources */
                else
                {
                    ungetc(c, source);
                    token->token_type = TOKEN_ASSIGN;
                }
                return freeResources(OK, str);

            case (STATE_COLON):
                if (c == '=')
                {
                    token->token_type = TOKEN_DEFINITION;
                }
                else
                {
                    ungetc(c, source);
                    state = STATE_START;
                }
                return freeResources(OK, str);

                /*     STATE_INTEGER  1       */
                /* analyse sequence of digits */
            case (STATE_INTEGER):
                /* If next symbol digit:            */
                /*   1) add digit to dynamic string */
                /*   2) repeat cycle                */
                if (isdigit(c))
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* If next symbol dot:               */
                    /*   1) add dot to dynamic string    */
                    /*   2) Change state to NUMBER_POINT */
                else if (c == '.')
                {
                    state = STATE_NUMBER_POINT;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* If next symbol is e or E:            */
                    /*   1) add e/E to dynamic string       */
                    /*   2) Change state to NUMBER_EXPONENT */
                else if (tolower(c) == 'e')
                {
                    state = STATE_NUMBER_EXPONENT;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to:                 */
                    /*    1) Unget symbol                            */
                    /*    2) Send token with function processInteger */
                else
                {
                    ungetc(c, source);
                    return processInteger(str, token);
                }
                break;

                /*    STATE_NUMBER_POINT 1.   */
                /* analyse sequence of digits */
            case (STATE_NUMBER_POINT):
                /* If next symbol is digit:          */
                /*   1) change state on STATE_DOUBLE */
                /*   2) add digit to dynamic string  */
                if (isdigit(c))
                {
                    state = STATE_DOUBLE;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to error */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }
                break;

                /*    STATE_NUMBER_POINT 1.1  */
                /* analyse sequence of digits */
            case (STATE_DOUBLE):
                /* If next symbol is digit:          */
                /*   1) add digit to dynamic string  */
                /*   2) repeat cycle                 */
                if (isdigit(c))
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* If next symbol is e or E:            */
                    /*   1) add e/E to dynamic string       */
                    /*   2) Change state to NUMBER_EXPONENT */
                else if (tolower(c) == 'e')
                {
                    state = STATE_NUMBER_EXPONENT;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to:                 */
                    /*    1) Unget symbol                            */
                    /*    2) Send token with function processDecimal */
                else
                {
                    ungetc(c, source);
                    return processDecimal(str, token);
                }
                break;

                /*  STATE_NUMBER_EXPONENT 1e  */
                /* analyse sequence of digits */
            case (STATE_NUMBER_EXPONENT):
                /* If next symbol is digit:          */
                /*   1) add digit to dynamic string  */
                /*   2) set state DOUBLE_EXPONENT    */
                if (isdigit(c))
                {
                    state = STATE_DOUBLE_EXPONENT;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* If next symbol is + or -:           */
                    /*   1) add symbol to dynamic string   */
                    /*   2) set state NUMBER_EXPONENT_SIGN */
                else if (c == '+' || c == '-')
                {
                    state = STATE_NUMBER_EXPONENT_SIGN;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to error */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }
                break;

                /*  NUMBER_EXPONENT_SIGN 1e+  */
                /* analyse sequence of digits */
            case (STATE_NUMBER_EXPONENT_SIGN):
                /* If next symbol is digit:          */
                /*   1) add digit to dynamic string  */
                /*   2) set state DOUBLE_EXPONENT    */
                if (isdigit(c))
                {
                    state = STATE_DOUBLE_EXPONENT;
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to error */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }
                break;

                /*  NUMBER_EXPONENT_SIGN 1e+2 */
                /* analyse sequence of digits */
            case (STATE_DOUBLE_EXPONENT):
                /* If next symbol is digit:          */
                /*   1) add digit to dynamic string  */
                /*   2) repeat cycle                 */
                if (isdigit(c))
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to:                 */
                    /*    1) Unget symbol                            */
                    /*    2) Send token with function processDecimal */
                else
                {
                    ungetc(c, source);
                    return processDecimal(str, token);
                }
                break;


                /*   STATE_STRING_START "...  */
                /* analyse sequence of chars  */
            case (STATE_STRING_START):
                if (c < 32)
                {
                    return freeResources(LEX_ERROR, str);
                }
                else if (c == ' ')
                {
                    if (!dynamicStrAddStr(str, sSPC))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                else if (c == '\\')
                {
                    state = STATE_STRING_ESCAPE;
                }
                    /* If next symbol is '             */
                    /*   1) change state STATE_STRING  */
                else if (c == '\"')
                {
                    state = STATE_STRING;
                }
                    /* If next symbol is EOL or EOF  */
                    /*   1) return error             */
                else if (c == '\n' || c == EOF)
                {
                    return freeResources(LEX_ERROR, str);
                }
                    /* All other transition lead to:    */
                    /*    1) Add char to dynamic string */
                    /*    2) Repeat cycle               */
                else
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                break;

                /*   STATE_STRING_START "...\ */
                /* analyse sequence of chars  */
            case (STATE_STRING_ESCAPE):
                /* If next symbol is n                            */
                /*   1) copy to dynamic string escape sequence \n */
                /*   2) change state STRING_START                 */
                if (c < 32)
                {
                    return freeResources(LEX_ERROR, str);
                }
                else if (c == 'n')
                {
                    if (!dynamicStrAddStr(str, sEOL))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_STRING_START;
                }
                    /* If next symbol is "                            */
                    /*   1) copy to dynamic string escape sequence  \ */
                    /*   2) change state STRING_START                 */
                else if (c == '"')
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_STRING_START;
                }
                    /* If next symbol is t                            */
                    /*   1) copy to dynamic string escape sequence \t */
                    /*   2) change state STRING_START                 */
                else if (c == 't')
                {
                    if (!dynamicStrAddStr(str, sTAB))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_STRING_START;
                }
                    /* If next symbol is \                            */
                    /*   1) copy to dynamic string escape sequence \\ */
                    /*   2) change state STRING_START                 */
                else if (c == '\\')
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_STRING_START;
                }
                    /* If next symbol is x               */
                    /*   1) change state STRING_DIGIT_xX */
                else if (c == 'x')
                {
                    state = STATE_STRING_DIGIT_xX;
                }
                    /* If next symbol is EOL or EOF */
                    /*   1) return error            */
                else if (c == '\n' || c == EOF)
                {
                    return freeResources(LEX_ERROR, str);
                }
                    /* All other transition lead to:             */
                    /*    1) Add symbol \ to dynamic string      */
                    /*    2) Add actual symbol to dynamic string */
                else
                {
                    if (!dynamicStrAddChar(str, '\\'))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    ungetc(c, source);
                    state = STATE_STRING_START;
                }
                break;

                /* STATE_STRING_DIGIT_xX '...\x */
                /* analyse sequence of chars    */
            case (STATE_STRING_DIGIT_xX):
                hexValue[0] = 0;
                hexValue[1] = 0;
                hexValue[2] = 0;
                /* If next symbol is a-f or A-F or 0-9 */
                /*   1) save char to F                 */
                /*   2) change state STRING_DIGIT_xXX; */
                if ((c >= 65 && c <= 70) || (c >= 97 && c <= 102) || isdigit(c))
                {
                    hexValue[0] = c;
                    state = STATE_STRING_DIGIT_xXX;
                }
                    /* All other transition lead to an error state */
                    /*   1) return error            */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }
                break;

                /* STATE_STRING_DIGIT_xXX "...\xA */
                /* analyse sequence of chars      */
            case (STATE_STRING_DIGIT_xXX):
                /* If next symbol is a-f, A-F . or 0-9                             */
                /*   1) with function ToAscci convert F,S to ASCII and save in str */
                /*   2) change state STRING_START                                  */
                if ((c >= 65 && c <= 70) || (c >= 97 && c <= 102) || isdigit(c))
                {
                    hexValue[1] = c;
                    hexValue[2] = '\0';
                    c = strtol(hexValue, NULL, 16);
                    if (c == '\t')
                    {
                        if (!dynamicStrAddStr(str, sTAB))
                        {
                            return freeResources(ERR_INTERNAL, str);
                        }
                    }
                    else if (c == '\n')
                    {
                        if (!dynamicStrAddStr(str, sEOL))
                        {
                            return freeResources(ERR_INTERNAL, str);
                        }
                    }
                    else if (c == ' ')
                    {
                        if (!dynamicStrAddStr(str, sSPC))
                        {
                            return freeResources(ERR_INTERNAL, str);
                        }
                    }
                    else if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                    state = STATE_STRING_START;
                }
                    /* All other transition lead to an error state */
                    /*   1) return error            */
                else
                {
                    return freeResources(LEX_ERROR, str);
                }
                break;

                /* STATE_STRING '...'        */
                /* analyse sequence of chars */
            case (STATE_STRING):
                /*      1) unget actual symbol                                 */
                /*      2) Save dynamic string to token attribute string value */
                /*      3) Send TOKEN_STRING                                   */
                /*      4) Return with function freeResources                  */
                ungetc(c, source);
                if (!dynamicStrCopyString(token->attribute.value_string, str))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                token->token_type = TOKEN_STRING;
                return freeResources(OK, str);
                break;

                /* STATE_IDENTIFIER_OR_KEYWORD _A9 */
                /*     analyse sequence of chars   */
            case (STATE_IDENTIFIER_OR_KEYWORD):
                /* If next symbol is alnum or _       */
                /*    1) Add symbol to dynamic string */
                /*    2) Repeat cycle                 */
                if (isalnum(c) || c == '_')
                {
                    if (!dynamicStrAddChar(str, c))
                    {
                        return freeResources(ERR_INTERNAL, str);
                    }
                }
                    /* All other transition lead to:                 */
                    /*    1) unget actual symbol                     */
                    /*    2) return with function processIdentifier  */
                else
                {
                    ungetc(c, source);
                    return processIdentifier(str, token);
                }
                break;
        }
    }
}
int main(){
    return 0;
}