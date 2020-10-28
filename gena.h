/**
 * @file CodeGenerator.h
 * @author xpimen00
 * @date 28.10.2020
 * @brief Header file for CodeGenrator.c
*/

#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamicStr.h"
#include "scanner.h"

DYN_STRING dyncode;


static bool defBuildFunc();

bool code_generator_start();

void codeGenClear();

#endif

