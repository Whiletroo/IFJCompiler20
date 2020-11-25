/**
* @file parser.c
* @author xproko40
* @date 11.11.2020
* @brief The parser
*/

#include "symtable.h"
#include "dynamicStr.h"
#include "parser.h"
#include "error.h"
#include "gena.h"
#include "symtable.h"
#include "scanner.h"

/**
#define next() do { \
	if (getToken(&token) == LEX_ERROR) \
		return LEX_ERROR; \
	PRINTTOKEN(); \
} while (0)

#define check(T) do { \
	if (tToken.token_type != T) { \
		fprintf(stderr, "Syntax error\n"); \
		return SYNTAX_ERR; \
	} \
} while (0)

#define term(T) do { \
	check(T); \
	next(); \
} while (0)

#define call(R) do { \
	PRINTNTERM(R); \
	int err = R(); \
	if (err != OK) \
		return err; \
} while (0)

#define try(C) do { \
	int err = C; \
	if (err != OK) \
		return err; \
} while (0)

#define return(R) do { \
	PRINTNTERM(R); \
	return R(); \
} while (0)


 void GET_TOKEN(int result)
{
     if ((result = getToken(&token)) != OK)
     {
        stopProgram(result, NULL);
     }
}


void CHECK_TOKEN(unsigned int expected)
{
    if (token.token_type != expected)
    {
        fprintf(stderr, "%s\n", getTokenName(token.token_type));
        stopProgram(SYNTAX_ERR, "Not expected token");
    }
}
*/

#define GET_TOKEN()                               \
    if ((result = getToken(&token)) != OK)

#define CHECK_TOKEN(expected)                                       \
    if (token.token_type != (expected))                             \
    fprintf (stderr, "%s\n", getTokenName(token.token_type));       \
    return SYNTAX_ERR

#define CHECK_RULE(rule)                          \
    if ((result = rule (data))) return result

#define CHECK_KEYWORD(_keyword)                   \
    if (token.token_type != TOKEN_KEYWORD         \
        || token.attribute.keyword != (_keyword)) \
        return SYNTAX_ERR

#define GET_AND_CHECK_TOKEN(expected)                               \
    	do {												    	\
		GET_TOKEN();											    \
		CHECK_TOKEN(expected);										\
	} while(0)

#define GET_TOKEN_AND_CHECK_RULE(rule)								\
	do {															\
		GET_TOKEN();												\
		CHECK_RULE(rule);											\
	} while(0)

#define GET_TOKEN_AND_CHECK_KEYWORD(_keyword)						\
	do {															\
		GET_TOKEN();												\
		CHECK_KEYWORD(_keyword);									\
	} while(0)


static int prog()
{
    int result;
    // <prog> → func id (<func_args>) <func_ret_types> {<st_list>}  <prog>
    switch (token.token_type) {
        case TOKEN_INT:
        case TOKEN_DOUBLE:
        case TOKEN_STRING:

            break;
            // <prog> → EOF
        case TOKEN_EOF:
            return OK;
        default:
            return SYNTAX_ERR;
    }
}

static int type()
{
    // int, double or string
}

static int types()
{
    // <types> → <type> <next_types>
    // <types> → ε
}

static int next_types()
{
    // <next_types> → , <type next_types>
    // <next_types> → ε
}

static int func_args()
{
    // <func_args> → id <type> <func_next_arg>
    // <func_args> → ε
}

static int func_next_arg()
{
    // <func_next_arg> → ',' id <type> <func_next_arg>
    // <func_next_arg> → ε
}

static int func_ret_types()
{
    // <func_ret_types> → (<types>)
}

static int st_list()
{
    // <st_list> → <state> <st_list>
    // <st_list> → ε
}

static int state()
{
    // <state> → E
    // <state> → <var_def>
    // <state> → <var_dec>
    // <state> → {<st_list>}
    // <state> → if E <state> <else_state>
    // <state> → for <var_def> ; E ; <assign> <state>
    // <state> → return E
    // <state> → <func_call>
}

static int var_def()
{
    // <var_def> → id := E
}

static int var_dec()
{
    // <var_dec> → id <type>
}

static int else_state()
{
    // <else_state> → else <state>
    // <else_state> → ε
}

static int assign()
{
    // <assign> → id = E
    // <assign> → ε
}

static int func_call()
{
    // <func_call> → id (<func_call_args>)
}

static int func_call_args()
{
    // <func_call_args> → id <func_call_next_arg>
}

static int func_call_next_arg()
{
    // <func_call_next_arg> → , id <func_call>
    // <func_call_next_arg> → ε
}