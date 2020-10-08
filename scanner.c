/**
* @file scanner.c
* @author xabram00, xzhuko01
* @date 10.11.2019
* @brief The scanner
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "error.h"

FILE *source;             // Source file that will be scanned
DYNAMIC_STRING *d_string; // Dynamic string that will be written into

/**
* Free resources and returns exit code.
*
* @param exit_code Exit code.
* @param str Dynamic string to be freed.
* @return Given exit code.
*/
static int freeResources(int exit_code, DYNAMIC_STRING *str)
{
    dynamicStrFree(str);
    return exit_code;
}

void setSourceFile(FILE *file)
{
    source = file;
}

void dynamicStrSet(DYNAMIC_STRING *str)
{
    d_string = str;
}

char hexToAscii(char F, char S)
{
    char hex[5], *err;
    hex[0] = '0';
    hex[1] = 'x';
    hex[2] = F;
    hex[3] = S;
    hex[4] = 0;
    return strtol(hex, &err, 16);
}

static int processInteger(DYNAMIC_STRING *str, tToken *token)
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

static int processDecimal(DYNAMIC_STRING *str, tToken *token)
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

static int processIdentifier(DYNAMIC_STRING *str, tToken *token)
{
    /* Main task */
    if (!dynamicStrCompareConstString(str, "def"))
    {
        token->attribute.keyword = KEYWORD_DEF;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "else"))
    {
        token->attribute.keyword = KEYWORD_ELSE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "if"))
    {
        token->attribute.keyword = KEYWORD_IF;
        token->token_type = TOKEN_KEYWORD;
    }

    else if (!dynamicStrCompareConstString(str, "None"))
    {
        token->attribute.keyword = KEYWORD_NONE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "pass"))
    {
        token->attribute.keyword = KEYWORD_PASS;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "return"))
    {
        token->attribute.keyword = KEYWORD_RETURN;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "while"))
    {
        token->attribute.keyword = KEYWORD_WHILE;
        token->token_type = TOKEN_KEYWORD;
    }
    /* Boolop */
    else if (!dynamicStrCompareConstString(str, "true"))
    {
        token->attribute.keyword = KEYWORD_TRUE;
        token->token_type = TOKEN_KEYWORD;
    }
    else if (!dynamicStrCompareConstString(str, "false"))
    {
        token->attribute.keyword = KEYWORD_FALSE;
        token->token_type = TOKEN_KEYWORD;
    }
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
    /* Cycles */
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
    /* Ifthen */
    else if (!dynamicStrCompareConstString(str, "elif"))
    {
        token->attribute.keyword = KEYWORD_ELIF;
        token->token_type = TOKEN_KEYWORD;
    }
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
    case TOKEN_IDIV:
        return "IDivison";
    case TOKEN_EQUALS:
        return "Equals";
    case TOKEN_HIGHER_OR_EQUAL:
        return "Higher or equal";
    case TOKEN_HIGHER:
        return "Higher";
    case TOKEN_LESS_OR_EUQAL:
        return "Less or equal";
    case TOKEN_LESS:
        return "Less";
    case TOKEN_NOT_EQUAL:
        return "Not equal";
    case TOKEN_LEFT_BRACKET:
        return "Left bracket";
    case TOKEN_RIGHT_BRACKET:
        return "Right bracket";
    case TOKEN_COLON:
        return "Colon";
    case TOKEN_COMMA:
        return "Comma";
    }
    return 0;
}

int getToken(tToken *token)
{
    if (source == NULL)
    {
        return ERR_INTERNAL;
    }

    if (d_string == NULL)
    {
        return ERR_INTERNAL;
    }

    token->attribute.value_string = d_string;

    DYNAMIC_STRING string;
    DYNAMIC_STRING *str = &string;

    if (!dynamicStrInit(str))
    {
        return ERR_INTERNAL;
    }

    dynamicStrClear(str);

    char c;
    char F;
    char S;
    int state = STATE_START;
    token->token_type = TOKEN_EMPTY;

    while (1)
    {
        c = (char)getc(source);

        switch (state)
        {
        case (STATE_START):
            if (c == '\n')
            {
                state = STATE_EOL;
            }
            else if (isspace(c))
            {
                state = STATE_START;
            }
            else if (c == '#')
            {
                state = STATE_DASH_COMMENTARY;
            }
            else if (c == '!')
            {
                state = STATE_SCREAMER;
            }
            else if (c == '<')
            {
                state = STATE_LESS;
            }
            else if (c == '>')
            {
                state = STATE_MORE;
            }
            else if (c == '=')
            {
                state = STATE_ASSIGN;
            }
            else if (c == '+')
            {
                token->token_type = TOKEN_PLUS;
                return freeResources(OK, str);
            }
            else if (c == '-')
            {
                token->token_type = TOKEN_MINUS;
                return freeResources(OK, str);
            }
            else if (c == '*')
            {
                token->token_type = TOKEN_MUL;
                return freeResources(OK, str);
            }
            else if (c == '/')
            {
                if (!dynamicStrAddChar(str, (char)(c)))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_DIV;
            }
            else if (c == '(')
            {
                token->token_type = TOKEN_LEFT_BRACKET;
                return freeResources(OK, str);
            }
            else if (c == ')')
            {
                token->token_type = TOKEN_RIGHT_BRACKET;
                return freeResources(OK, str);
            }
            else if (c == ',')
            {
                token->token_type = TOKEN_COMMA;
                return freeResources(OK, str);
            }
            else if (c == ':')
            {
                token->token_type = TOKEN_COLON;
                return freeResources(OK, str);
            }
            else if (c == EOF)
            {
                token->token_type = TOKEN_EOF;
                return freeResources(OK, str);
            }
            else if (c == '"')
            {
                state = STATE_BLOCK_COMMENTARY1;
            }
            else if (isalpha(c) || c == '_')
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }
            else if (isdigit(c))
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_INTEGER;
            }
            else if (c == '\'')
            {
                state = STATE_STRING_START;
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            break;
        case (STATE_DIV):
            if (c == '/')
            {
                token->token_type = TOKEN_IDIV;
            }
            else
            {
                ungetc(c, source);
                token->token_type = TOKEN_DIV;
            }
            return freeResources(OK, str);
        case (STATE_EOL):
            if (isspace(c))
            {
                break;
            }
            ungetc(c, source);
            token->token_type = TOKEN_EOL;
            return freeResources(OK, str);
        case (STATE_BLOCK_COMMENTARY1): // "
            if (c == '"')
            {
                state = STATE_BLOCK_COMMENTARY2;
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            break;
        case (STATE_BLOCK_COMMENTARY2): // ""
            if (c == '"')
            {
                state = STATE_BLOCK_COMMENTARY3;
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            break;
        case (STATE_BLOCK_COMMENTARY3): // """
            if (c == '"')
            {
                state = STATE_BLOCK_COMMENTARY_LEAVE1;
            }
            else if (c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else
            {
                state = STATE_BLOCK_COMMENTARY3;
            }
            break;
        case (STATE_BLOCK_COMMENTARY_LEAVE1): // """(x)*"
            if (c == '"')
            {
                state = STATE_BLOCK_COMMENTARY_LEAVE2;
            }
            else if (c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else
            {
                state = STATE_BLOCK_COMMENTARY3;
            }
            break;
        case (STATE_BLOCK_COMMENTARY_LEAVE2): // """(x)*""
            if (c == '"')
            {
                state = STATE_START;
            }
            else if (c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else
            {
                state = STATE_BLOCK_COMMENTARY3;
            }
            break;
        case (STATE_DASH_COMMENTARY):
            if (c == '\n' || c == EOF)
            {
                state = STATE_START;
                ungetc(c, source);
            }
            break;
        case (STATE_SCREAMER):
            if (c == '=')
            {
                token->token_type = TOKEN_NOT_EQUAL;
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            return freeResources(OK, str);
        case (STATE_LESS):
            if (c == '=')
            {
                token->token_type = TOKEN_LESS_OR_EUQAL;
            }
            else
            {
                ungetc(c, source);
                token->token_type = TOKEN_LESS;
            }
            return freeResources(OK, str);
        case (STATE_MORE):
            if (c == '=')
            {
                token->token_type = TOKEN_HIGHER_OR_EQUAL;
            }
            else
            {
                ungetc(c, source);
                token->token_type = TOKEN_HIGHER;
            }
            return freeResources(OK, str);
        case (STATE_ASSIGN):
            if (c == '=')
            {
                token->token_type = TOKEN_EQUALS;
            }
            else
            {
                ungetc(c, source);
                token->token_type = TOKEN_ASSIGN;
            }
            return freeResources(OK, str);

        case (STATE_INTEGER):
            if (isdigit(c))
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else if (tolower(c) == 'e')
            {
                state = STATE_NUMBER_EXPONENT;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                ungetc(c, source);
                return processInteger(str, token);
            }
            break;

        case (STATE_NUMBER_POINT):
            if (isdigit(c))
            {
                state = STATE_DOUBLE;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                return freeResources(ERR_INTERNAL, str);
            }
            break;

        case (STATE_DOUBLE):
            if (isdigit(c))
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else if (tolower(c) == 'e')
            {
                state = STATE_NUMBER_EXPONENT;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                ungetc(c, source);
                return processDecimal(str, token);
            }
            break;

        case (STATE_NUMBER_EXPONENT):
            if (isdigit(c))
            {
                state = STATE_DOUBLE_EXPONENT;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else if (c == '+' || c == '-')
            {
                state = STATE_NUMBER_EXPONENT_SIGN;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            break;

        case (STATE_NUMBER_EXPONENT_SIGN):
            if (isdigit(c))
            {
                state = STATE_DOUBLE_EXPONENT;
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            break;

        case (STATE_DOUBLE_EXPONENT):
            if (isdigit(c))
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                ungetc(c, source);
                return processDecimal(str, token);
            }
            break;

        case (STATE_STRING_START):
            if (c == '\\')
            {
                state = STATE_STRING_ESCAPE;
            }
            else if (c == '\'')
            {
                state = STATE_STRING;
            }
            else if (c == '\n' || c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            break;

        case (STATE_STRING_ESCAPE):
            if (c == 'n')
            {
                c = '\n';
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == '\'')
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == '"')
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                c = '\t';
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == '\\')
            {
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == 'x')
            {
                state = STATE_STRING_DIGIT_xX;
            }
            else if (c == '\n' || c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else
            {
                if (!dynamicStrAddChar(str, '\\'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            break;

        case (STATE_STRING_DIGIT_xX):
            if ((c >= 65 && c <= 70) || (c >= 97 && c <= 102))
            {
                F = tolower(c);
                state = STATE_STRING_DIGIT_xXX;
            }
            else if (isdigit(c))
            {
                F = c;
                state = STATE_STRING_DIGIT_xXX;
            }
            else if (c == '\n' || c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else if (c == '\'')
            {

                if (!dynamicStrAddChar(str, '\\'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }

                if (!dynamicStrAddChar(str, 'x'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }

                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING;
            }
            else
            {
                if (!dynamicStrAddChar(str, '\\'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, 'x'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            break;

        case (STATE_STRING_DIGIT_xXX):
            if ((c >= 65 && c <= 70) || (c >= 97 && c <= 102))
            {
                S = tolower(c);
                c = hexToAscii(F, S);
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (isdigit(c))
            {
                S = c;
                c = hexToAscii(F, S);
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            else if (c == '\n' || c == EOF)
            {
                return freeResources(ERR_LEXICAL_SYNTAX, str);
            }
            else if (c == '\'')
            {
                if (!dynamicStrAddChar(str, '\\'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, 'x'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, F))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING;
            }
            else
            {
                if (!dynamicStrAddChar(str, '\\'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, 'x'))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, F))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                if (!dynamicStrAddChar(str, c))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
                state = STATE_STRING_START;
            }
            break;

        case (STATE_STRING):
            ungetc(c, source);
            if (!dynamicStrCopyString(token->attribute.value_string, str))
            {
                return freeResources(ERR_INTERNAL, str);
            }
            token->token_type = TOKEN_STRING;
            return freeResources(OK, str);
            break;

        case (STATE_IDENTIFIER_OR_KEYWORD):
            if (isalnum(c) || c == '_')
            {
                if (!dynamicStrAddChar(str, (char)tolower(c)))
                {
                    return freeResources(ERR_INTERNAL, str);
                }
            }
            else
            {
                ungetc(c, source);
                return processIdentifier(str, token);
            }
            break;
        }
    }
}
