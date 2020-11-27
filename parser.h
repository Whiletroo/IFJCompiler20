/**
* @file parser.h
* @author xproko40
* @date 25.11.2020
* @brief The parser
*/

#ifndef _PARSER_H
#define _PARSER_H
#include <stdbool.h>
#include "symtable.h"
#include "dynamicStr.h"
#include "error.h"
//#include "gena.h"
#include "scanner.h"
#include "expressions.h"

TSymTable *globalTable;
TData *data;
tToken token;				/// Token
int label_index;
int label_deep;

#endif //_PARSER_H