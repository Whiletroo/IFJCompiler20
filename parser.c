/**
* @file parser.c
* @author xproko40
* @date 25.11.2020
* @brief The parser
*/

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include "expressions.h"
#include "gena.h"


TSymTable *globalTable;
TSymTable *localTable;
tToken token;
TData *tmparg;
TData *currentId;

#define GET_TOKEN()                               \
    if ((result = getToken(&token)) != OK)        \
        return result                              \


#define CHECK_TOKEN(expected)                                       \
    if (token.token_type != (expected)){                            \
                                                                  \
        return SYNTAX_ERR;                                           \
    }                                                                \
    fprintf (stderr, "%s\n", getTokenName(token.token_type));

#define CHECK_RULE(rule)                          \
    if (( result = rule() )) return result

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
static int func_args();
static int func_next_arg();
static int func_ret_types();
static int ret_type();
static int ret_types();
static int next_ret_types();
static int st_list();
static int state();
static int var_def();
static int else_state();
static int assign();
static int func_call();
static int func_call_args();
static int func_call_next_arg();



static int start(){
    int result;
    // <start> -> package id EOL <prog>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_PACKAGE) {
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        GET_AND_CHECK_TOKEN(TOKEN_EOL);
        GET_TOKEN();
        return prog();
    } else if (token.token_type == TOKEN_EOL) {
        GET_TOKEN_AND_CHECK_RULE(start);
    }
    return SYNTAX_ERR;
}

static int prog()
{
    int result;


    // <prog> → func id (<func_args>) <func_ret_types> {<st_list>}  <prog>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_FUNC) {

        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        // add function to global symbol table
        if ((symTableSearch(globalTable, token.attribute.value_string->str)) == false)
        {
            symTableInsert(globalTable, token.attribute.value_string->str);
            currentId = symTableGetItem(globalTable, token.attribute.value_string->str);
            currentId->idType = function;
            currentId->defined = true;
        }

        GET_AND_CHECK_TOKEN(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_args);
        CHECK_TOKEN(TOKEN_RIGHT_BRACKET);
        GET_TOKEN();
        if (token.token_type == TOKEN_LEFT_BRACKET) {
           CHECK_RULE(func_ret_types);
        }
        CHECK_TOKEN(TOKEN_LCURLY_BRACKET);
        GET_AND_CHECK_TOKEN(TOKEN_EOL);  //<<<TODO
        GET_TOKEN_AND_CHECK_RULE(st_list);
        CHECK_TOKEN(TOKEN_RCURLY_BRACKET);

        GET_TOKEN();
        return prog();

    }
    // <prog> → EOF
    else if (token.token_type == TOKEN_EOF){
        return OK;
    }
    //<prog> → EOL
    else if (token.token_type == TOKEN_EOL){
        GET_TOKEN();
        return prog();
    }
    return SYNTAX_ERR;
}

static int type()
{
    // int, double or string
    if (token.token_type == TOKEN_KEYWORD) {
        switch (token.attribute.keyword) {
            case KEYWORD_INT:
                tmparg->dataType[0] = INT_TYPE;
                break;
            case KEYWORD_FLOAT64:
                tmparg->dataType[0] = FLOAT_TYPE;
                break;
            case KEYWORD_STRING:
                tmparg->dataType[0] = STRING_TYPE;
                break;
            default:
                return SYNTAX_ERR;
        }
    }
    return OK;
}

static int func_args()
{
    int result;
    // <func_args> → id <type> <func_next_arg>
    if (token.token_type == TOKEN_IDENTIFIER) {
        // add variable to local symbol table
        if ((symTableSearch(localTable, token.attribute.value_string->str)) == false)
        {
                symTableInsert(localTable, token.attribute.value_string->str);
                tmparg = symTableGetItem(localTable, token.attribute.value_string->str);
                tmparg ->idType = variable;
        }
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(func_next_arg);
    } else if ( token.token_type != TOKEN_RIGHT_BRACKET) {
        return SYNTAX_ERR;
    }
    // <func_args> → ε
    return OK;
}

static int func_next_arg()
{
    int result;
    // <func_next_arg> → ',' id <type> <func_next_arg>
    if (token.token_type == TOKEN_COMMA) {
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        // add variable to local symbol table
        if ((symTableSearch(localTable, token.attribute.value_string->str)) == false)
        {
            symTableInsert(localTable, token.attribute.value_string->str);
            tmparg = symTableGetItem(localTable, token.attribute.value_string->str);
            tmparg ->idType = variable;
        }
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(func_next_arg);
    }
    // <func_next_arg> → ε
    return OK;
}

static int func_ret_types()
{
    int result;
    // <func_ret_types> → (<ret_types>)
    if (token.token_type == TOKEN_LEFT_BRACKET){
        GET_TOKEN_AND_CHECK_RULE(ret_types);
        CHECK_TOKEN(TOKEN_RIGHT_BRACKET);
    }
    // <func_ret_types> → ε
    GET_TOKEN();
    return OK;
}
static int ret_type()
{
    // int, double or string
    if (token.token_type == TOKEN_KEYWORD) {
        switch (token.attribute.keyword) {
            case KEYWORD_INT:
                currentId->dataType[0] = INT_TYPE;
                break;
            case KEYWORD_FLOAT64:
                currentId->dataType[0] = FLOAT_TYPE;
                break;
            case KEYWORD_STRING:
                currentId->dataType[0] = STRING_TYPE;
                break;
            default:
                return SYNTAX_ERR;
        }
    }
    return OK;
}

static int ret_types()
{
    int result;
    // <types> → <ret_type> <next_ret_types>
    if (token.token_type != TOKEN_RIGHT_BRACKET) {
        CHECK_RULE(ret_type);
        GET_TOKEN_AND_CHECK_RULE(next_ret_types);
    }
    // <types> → ε
    return OK;
}

static int next_ret_types()
{
    int result;
    // <next_types> → , <ret_type> <next_ret_types>
    if (token.token_type == TOKEN_COMMA) {
        GET_TOKEN_AND_CHECK_RULE(ret_type);
        GET_TOKEN_AND_CHECK_RULE(next_ret_types);
    }
    // <next_types> → ε
    return OK;
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
    return result;
}

static int state()
{
    int result;
    // <state> → if E <state> <else_state>
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_IF){
        GET_TOKEN_AND_CHECK_RULE(expessions);
        GET_AND_CHECK_TOKEN(TOKEN_EOL);
        GET_TOKEN_AND_CHECK_RULE(state);
        GET_AND_CHECK_TOKEN(TOKEN_RCURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(else_state);
        return OK;
    }
    // <state> → for id <var_def> ; E ; id <assign> <state>
    else if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_FOR){
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        GET_TOKEN_AND_CHECK_RULE(var_def);
        GET_AND_CHECK_TOKEN(TOKEN_SEMICOLON);
        GET_TOKEN_AND_CHECK_RULE(expessions);
        GET_AND_CHECK_TOKEN(TOKEN_SEMICOLON);
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
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

    // <state> → <типо id> a потом <var_def> or <func_call> or <var_dec> or <assign>
    else if (token.token_type == TOKEN_IDENTIFIER) {
        GET_TOKEN();
        switch (token.token_type) {
            // var_def
            case TOKEN_DEFINITION:
                CHECK_RULE(var_def);
                return OK;
                // func_call
            case TOKEN_LEFT_BRACKET:
                CHECK_RULE(func_call);
                return OK;
                // assign
            case TOKEN_ASSIGN:
                CHECK_RULE(assign);
                // var_dec OLD VERSION
            default:
                //CHECK_RULE(type);
                return SYNTAX_ERR;
        }
    }
    else {
        return SYNTAX_ERR;
    }
    return result;
}

static int var_def()
{ int result;
    // <var_def> → := Exp
    if (token.token_type == TOKEN_DEFINITION){
        GET_TOKEN_AND_CHECK_RULE(expessions);
        CHECK_TOKEN(TOKEN_EOL); // expressions takes token EOL, which mean end of expression
    }
    return OK;
}

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
    if (token.token_type == TOKEN_KEYWORD && token.attribute.keyword == KEYWORD_ELSE){
        GET_TOKEN_AND_CHECK_RULE(state);
    }
    // <else_state> → ε
    return OK;
}

static int assign()
{
    int result;
    // <assign> → = Exp
    if (token.token_type == TOKEN_ASSIGN){
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
    // <func_call_next_arg> → , id <func_call_next_arg>
    if (token.token_type == TOKEN_COMMA){
        GET_AND_CHECK_TOKEN(TOKEN_IDENTIFIER);
        GET_TOKEN_AND_CHECK_RULE(func_call_next_arg);
    }
    // <func_call_next_arg> → ε
    return OK;
}

int parse() {
    int result;
    globalTable = symTableInit();
    localTable = symTableInit();
    d_string = dynamicStrInit();
    if ((result = getToken(&token)) == OK)
    {
        if (token.token_type == TOKEN_EOF)
        {
            fprintf(stderr, "#FILE: Input file is empty\n");
            dynamicStrFree(d_string);
            return result;
        } else {
            CHECK_RULE(start);
            symTableDestroy(globalTable);
            symTableDestroy(localTable);
            dynamicStrFree(d_string);
        }
        return OK;
    } else 
    {
        return ERR_INTERNAL;
    }
}