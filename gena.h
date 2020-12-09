/**
 * @file gena.h
 * @author xpimen00
 * @date 9.12.2020
 * @brief Header file for gena.c
*/

#ifndef _GENA_H_
#define _GENA_H_

#include <stdbool.h>

#include "dynamicStr.h"
#include "expressions.h"

extern DYN_STRING dyncode;

bool codeGenStart();
bool codeGenOpen();
bool genCreaStartFream(char *nameFrame);
bool genCrePushFr();
bool genCrePopFr();
bool genCreDefVar(char *nameMod);
bool proid(char *name1,char *name2,char *name3);
bool genCreMove(char* name1,char *name2,tDataType type);
bool genCreReturn();
bool genCreateLabel(char *Label);
bool genDestLabelEndJamp(char* Label);
bool genCheckFrameDeep(int FramDeep);
void genCodePrint();
bool genCreDefVarFunk(char *name,char *value,tDataType type);
bool genCheckArithm(tPrecRules rule, char *name1,char *name2,char *name3);
bool genCheckTypeValue(tDataType type);
bool genCreDefVar(char *nameMod);
bool geneCall(char *Label );
bool genFunRead(char *nameValue,int FrameDeep,tDataType typeValue);
bool genFunWrite(char *nameMod,int FrameDeep);
bool genCreJumpEQ(char *Label, char *var1, char *typeOfVar, char *var2, bool stak);
bool genCreJumpNEQ(char *Label, char *var1, char *typeOfVar, char *var2, bool stak);
bool genExit(char *val);
bool genBreak();
bool genCheckArithmStack(tPrecRules rule, char *name1,char *name2,char *name3);
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

