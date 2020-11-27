/**
 * @file dynamicStr.h
 * @author xpimen00
 * @date 8.10.2020
 * @brief The dynamic string
*/

#ifndef _DYNAMIC_STR_H
#define _DYNAMIC_STR_H

#include <stdio.h>
#include <ctype.h>

#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *str;              /* <- value */
    unsigned int length;    /* <- length of a string */
    unsigned int allocSize; /* <- size of an allocated memory for that string */
} DYN_STRING;

/**
 * Dynamic String Initialization
 * @param ds
 * @return Initialized Dynamic String
 */
int dynamicStrInit(DYN_STRING *ds);

/**
 * Dynamic String Free
 * @param ds
 */
void dynamicStrFree(DYN_STRING *ds);

/**
 * Clear Dynamic String
 * @param ds
 */
void dynamicStrClear(DYN_STRING *ds);

/**
 * Add char to Dynamic String
 * @param ds
 * @param c
 * @return DYNAMIC_STRING_OK or DYNAMIC_STRING_ERROR (it went correctly or not)
 */
int dynamicStrAddChar(DYN_STRING *ds, char c);

/**
 * Add str to Dynamic String
 * @param ds
 * @param str
 * @return DYNAMIC_STRING_OK or DYNAMIC_STRING_ERROR (it went correctly or not)
 */
int dynamicStrAddStr(DYN_STRING *ds, char  *str);
/**
 * Copy from Dynamic String to another one
 * @param ds1
 * @param ds2
 * @return DYNAMIC_STRING_OK or DYNAMIC_STRING_ERROR (it went correctly or not)
 */
int dynamicStrCopyString(DYN_STRING *ds1, DYN_STRING *ds2);

/**
 * Copy from normal char string to Dynamic String
 *
 * @param ds1
 * @param string
 * @return DYNAMIC_STRING_OK or DYNAMIC_STRING_ERROR (it went correctly or not)
 */
int dynamicStrCopyStringChar(DYN_STRING *ds1, char *string);

/**
 * Compare two Dynamic Strings
 * @param ds1
 * @param ds2
 * @return 0 => equal
 */
int dynamicStrCompareString(DYN_STRING *ds1, DYN_STRING *ds2);

/**
 * Compare Dynamic String with some string
 * @param ds1
 * @param ds2
 * @return 0 => equal
 */
int dynamicStrCompareConstString(DYN_STRING *ds1, char *ds2);

/**
 * Get Dynamic String value
 * @param ds
 * @return Value of Dynamic String
 */
char *dynamicStrGet(DYN_STRING *ds);

/**
 * Get Dynamic String length
 * @param s
 * @return Length of Dynamic String
 */
int dynamicStrGetLength(DYN_STRING *ds);

#endif
