/**
 * @file gena.h
 * @author xpimen00
 * @date 7.12.2020
 * @brief Header file for gena.c
*/

#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamicStr.h"
#include "expressions.h"
#include "symtable.h"

extern DYN_STRING dyncode;

bool codeGenStart();
void codeGenClear();
bool codeGenOpen();
bool genCreaStartFream(char *nameFrame);
bool genCrePushFr();
bool genCrePopFr();
bool genCreReturn();
bool genCreateLabel(char *Label);
bool genDestLabelEndJamp(char* Label);
bool genCheckFrameDeep(int FramDeep);
bool genCreClear();
bool genCheckArithm(tPrecRules rule, char *name1,char *name2,char *name3);
bool genCheckTypeValue(tDataType type);
bool genCreDefVar(int FrameDeep,char *nameMod);
bool geneCall(char *Label );
bool genFunRead(char *nameValue,int FrameDeep,tDataType typeValue);
bool genFunWrite(char *nameMod,int FrameDeep);
bool genCreJumpEQ(char *Label, char *var1,int FrameDeep, char *typeOfVar, char *var2, bool stak);
bool genCreJumpNEQ(char *Label, char *var1,int FrameDeep, char *typeOfVar, char *var2, bool stak);
bool genExit(char *val);
bool genBreak();
bool genDpronr(char *valOfvar,tDataType type);
bool int2Float(char *retval1,bool stak);
bool int2Char(char *retval1,bool stak);
bool float2Int(char *retval1,bool stak);
bool string2Int(char *retval1,bool stak);
bool genConCat(char *name1,char *name2);
bool genStrLen(char *nameMod,int FrameDeep);
bool genGetChar(char *nameMod,char *retval1,int FrameDeep,int FrameDeep1);
bool genSetChar(char *nameMod,char *indFildMod,char *nameChar,int FrameDeep,int FrameDeep1,tDataType type);
bool genType(char *varToSave,int FrameDeep,char *typeMod,char *nameOfVar,tDataType type);

#endif

