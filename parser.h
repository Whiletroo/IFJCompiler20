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

extern TSymTable *globalTable;
extern TSymTable *localTable;
extern TData *currentId;
//int label_index;
//int label_deep;


int parse();

#endif //_PARSER_H