/**
* @file parser.h
* @author xproko40
* @date 25.11.2020
* @brief The parser
*/

#ifndef _PARSER_H
#define _PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "symtable.h"


extern TSymTable *globalTable;
extern TSymTable *localTable;
extern TData *currentId;
extern TData *tmparg;
//int label_index;
//int label_deep;



int parse();

#endif //_PARSER_H