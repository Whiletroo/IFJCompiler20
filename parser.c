/**
* @file parser.c
* @author xproko40
* @date 25.11.2020
* @brief The parser
*/

#include "parser.h"

#define GET_TOKEN()                               \
    if ((result = getToken(&token)) != OK)        \
        return result

#define CHECK_TOKEN(expected)                                       \
    if (token.token_type != (expected))                             \
    fprintf (stderr, "%s\n", getTokenName(token.token_type));       \
    return SYNTAX_ERR

#define CHECK_RULE(rule)                          \
    if ((result = rule ())) return result

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


static int start();
static int prog();
static int type();
static int types();
static int next_types();
static int func_args();
static int func_next_arg();
static int func_ret_types();
static int st_list();
static int state();
static int var_def();
//static int var_dec();
static int else_state();
static int assign();
static int func_call();
static int func_call_args();
static int func_call_next_arg();




static int start(){
    int result;
    // <start> -> package id <prog>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_PACKAGE) {
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        GET_TOKEN();
        return prog();
    }
    return OK;
}

static int prog()
{
    int result;
    globalTable = symTableInit();

    // <prog> → func id (<func_args>) <func_ret_types> {<st_list>}  <prog>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_FUNC) {

        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        // add function to global symbol table
        if ((symTableSearch(globalTable, token.attribute.value_string->str)) == false)
        {
             symTableInsert(globalTable, token.attribute.value_string->str);
             symTableGetItem(globalTable, token.attribute.value_string->str)->idType = function;
        }

        GET_AND_CHECK_TOKEN(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_args);
        CHECK_TOKEN(TOKEN_RIGHT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_ret_types);
        GET_AND_CHECK_TOKEN(TOKEN_LCURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(st_list);
        CHECK_TOKEN(TOKEN_RCURLY_BRACKET);

        GET_TOKEN();
        return prog();

    }
    // <prog> → EOF
    else if (token.token_type == TOKEN_EOF){
        return OK;
    }
    return SYNTAX_ERR;
}

static int type()
{
    int result;
    // int, double or string
    if (token.token_type == TOKEN_KEYWORD){
        switch (token.attribute.keyword) {
            case KEYWORD_INT:
                symTableGetItem(globalTable, token.attribute.value_string->str)->dataType = INT_TYPE;
                break;
            case KEYWORD_FLOAT64:
                symTableGetItem(globalTable, token.attribute.value_string->str)->dataType = FLOAT_TYPE;
                break;
            case KEYWORD_STRING:
                symTableGetItem(globalTable, token.attribute.value_string->str)->dataType = STRING_TYPE;
                break;
            default:
                return SYNTAX_ERR;
        }
    } else {
        return SYNTAX_ERR;
    }
    GET_TOKEN(); //TODO
    return result;
}

static int types()
{
    int result;
    // <types> → <type> <next_types>
    CHECK_RULE(type);
    GET_TOKEN_AND_CHECK_RULE(next_types);
    // <types> → ε
    return OK;
}

static int next_types()
{
    int result;
    // <next_types> → , <type> <next_types>
    if (token.token_type == TOKEN_COMMA) {
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(next_types);
    }
    // <next_types> → ε
    return OK;
}

static int func_args()
{
    int result;
    // <func_args> → id <type> <func_next_arg>
    if (token.token_type == TOKEN_IDENTIFIER) {
        // add variable to global symbol table
        if ((symTableSearch(globalTable, token.attribute.value_string->str)) == false)
        {
            symTableInsert(globalTable, token.attribute.value_string->str);
            symTableGetItem(globalTable, token.attribute.value_string->str)->idType = variable;
        }
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(func_next_arg);
    }
    // <func_args> → ε
    return OK;
}

static int func_next_arg()
{
    int result;
    // <func_next_arg> → ',' id <type> <func_next_arg>
    if (token.token_type == TOKEN_COMMA) {
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(func_next_arg);
    }
    // <func_next_arg> → ε
    return OK;
}

static int func_ret_types()
{
    int result;
    // <func_ret_types> → (<types>)
    if (token.token_type == TOKEN_LEFT_BRACKET){
        GET_TOKEN_AND_CHECK_RULE(types);
        GET_AND_CHECK_TOKEN(TOKEN_RIGHT_BRACKET);
    }
    GET_TOKEN();
    return prog();
}

static int st_list()
{
    int result;
    // <st_list> → ε
    if (token.token_type == TOKEN_RCURLY_BRACKET) {
        return OK;
    }else{
    // <st_list> → <state> <st_list>
        CHECK_RULE(state);
        GET_TOKEN_AND_CHECK_RULE(st_list);
    }
    return OK;
}

static int state()
{
    int result;
    // <state> → if E <state> <else_state>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_IF){
        GET_TOKEN_AND_CHECK_RULE(expessions);
        GET_TOKEN_AND_CHECK_RULE(state);
        GET_TOKEN_AND_CHECK_RULE(else_state);
    }
    // <state> → for <var_def> ; E ; <assign> <state>
    else if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_FOR){
        GET_TOKEN_AND_CHECK_RULE(var_def);
        GET_AND_CHECK_TOKEN(TOKEN_SEMICOLON);
        GET_TOKEN_AND_CHECK_RULE(expessions);
        GET_AND_CHECK_TOKEN(TOKEN_SEMICOLON);
        GET_TOKEN_AND_CHECK_RULE(assign);
        GET_TOKEN_AND_CHECK_RULE(state);
    }
    // <state> → return E
    else if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_RETURN){
        GET_TOKEN_AND_CHECK_RULE(expessions);
    }
    // <state> → {<st_list>}
    else if (token.token_type == TOKEN_LCURLY_BRACKET){
        GET_TOKEN_AND_CHECK_RULE(st_list);
        GET_AND_CHECK_TOKEN(TOKEN_RCURLY_BRACKET);
    }
    // <state> → <типо id> a потом <var_def> or <func_call> or <var_dec>
    else if (token.token_type == TOKEN_IDENTIFIER){
        GET_TOKEN();
        switch (token.token_type) {
            // var_def
            case TOKEN_DEFINITION:
                GET_TOKEN_AND_CHECK_RULE(var_def);
            // func_call
            case TOKEN_IDENTIFIER:
                GET_TOKEN_AND_CHECK_RULE(func_call_args);
            // var_dec
            default:
                GET_TOKEN_AND_CHECK_RULE(type);
        }
    }
    // <state> → Exp
    else {
        GET_TOKEN_AND_CHECK_RULE(expessions);
    }
    return OK;
}

static int var_def()
{ int result;
    // <var_def> → := Exp
    if (token.token_type == TOKEN_DEFINITION){
        GET_TOKEN_AND_CHECK_RULE(expessions);
    }
    return OK;
}

/* дважды декларирована
static int var_dec()
{
    // <var_dec> → <type> ничего не надо
}
*/

static int func_call()
{
    int result;
    // <func_call> → (<func_call_args>)
    if (token.token_type == TOKEN_LEFT_BRACKET){
        GET_TOKEN_AND_CHECK_RULE(func_call_args);
    }
    return OK;
}


static int else_state()
{
    int result;
    // <else_state> → else <state>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_RETURN){
        GET_TOKEN_AND_CHECK_RULE(state);
    }
    // <else_state> → ε
    return OK;
}

static int assign()
{
    int result;
    // <assign> → id = Exp
    if (token.token_type == TOKEN_IDENTIFIER){
        GET_AND_CHECK_TOKEN(TOKEN_ASSIGN);
        GET_TOKEN_AND_CHECK_RULE(expessions);
    }
    // <assign> → ε
    return OK;
}


static int func_call_args()
{
    int result;
    // <func_call_args> → id <func_call_next_arg>
    if (token.token_type == TOKEN_IDENTIFIER){
        GET_TOKEN_AND_CHECK_RULE(func_call_next_arg);
    }
    return OK;
}

static int func_call_next_arg()
{
    int result;
    // <func_call_next_arg> → , id <func_call>
    if (token.token_type == TOKEN_COMMA){
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        GET_TOKEN_AND_CHECK_RULE(func_call);
    }
    // <func_call_next_arg> → ε
    return OK;
}

int parse() {
    int result;
    DYN_STRING string;

    if ((result = getToken(&token)) == OK)
    {

        if (token.token_type == TOKEN_EOF)
        {
            fprintf(stderr, "#FILE: Input file is empty\n");
            dynamicStrFree(&string);
            return result;
        } else {
            CHECK_RULE(start);
        }
    }
    return OK;
}