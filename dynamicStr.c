/**
 * @file dynamicStr.c
 * @author xpimen00
 * @date 8.10.2020
 * @brief The dynamic string
*/

#include "dynamicStr.h"

/* The initial size of string */
#define DYN_STRING_LEN_INC 8

#define DYN_STRING_ERROR 0
#define DYN_STRING_OK 1

DYN_STRING *dynamicStrInit()
{
    DYN_STRING *ds;
    ds = (DYN_STRING *) malloc(sizeof(DYN_STRING));
    if (ds == NULL) {
        return NULL;
    }

    ds->str = (char *) malloc(DYN_STRING_LEN_INC);
    if (ds->str == NULL)
    {
        return NULL;
    }

    ds->length = 0;
    ds->allocSize = DYN_STRING_LEN_INC;
    ds->str[0] = '\0';

    return ds;
}

void dynamicStrFree(DYN_STRING *ds)
{

    free(ds->str);
}

void dynamicStrClear(DYN_STRING *ds)
{

    ds->str[0] = '\0';
    ds->length = 0;
}

int dynamicStrAddChar(DYN_STRING *ds, char c)
{
    /* In case if memory is not enough the size of a string will be reallocated */
    if (ds->length + 1 >= ds->allocSize)
    {
        if ((ds->str = (char *)realloc(ds->str, ds->length + DYN_STRING_LEN_INC)) == NULL)
            /* The reallocation has been carried out unsuccessfully */
            return DYN_STRING_ERROR;

        ds->allocSize = ds->length + DYN_STRING_LEN_INC;
    }

    ds->str[ds->length] = c;
    ds->length++;
    ds->str[ds->length] = '\0';
    return DYN_STRING_OK;
}

int dynamicStrAddStr(DYN_STRING *ds, char *str)
{
    unsigned strSize = strlen(str);
    if (ds->length + strSize + 1 >= ds->allocSize)
    {
        unsigned int newSize = ds->length + strSize + 1;
        if (!(ds->str = (char *)realloc(ds->str, newSize)))
        {
            return DYN_STRING_ERROR;
        }
        ds->allocSize = newSize;
    }

    ds->length += strSize;
    strcat(ds->str, str);
    ds->str[ds->length] = '\0';

    return DYN_STRING_OK;
}

int dynamicStrCopyString(DYN_STRING *ds1, DYN_STRING *ds2)
{

    unsigned int newLength = ds2->length;

    /* In case if memory is not enough to copy ds2 to ds1 */
    if (newLength >= ds1->allocSize)
    {
        /* We will reallocate the current size to a larger one */
        if ((ds1->str = (char *)realloc(ds1->str, newLength + 1)) == NULL)
            /* The reallocation has been carried out unsuccessfully */
            return DYN_STRING_ERROR;

        ds1->allocSize = newLength + 1;
    }

    /* Let's copy */
    strcpy(ds1->str, ds2->str);
    ds1->length = newLength;

    return DYN_STRING_OK;
}

int dynamicStrCopyStringChar(DYN_STRING *ds1, char *string)
{
    unsigned int newLength = strlen(string);

    if (newLength >= ds1->allocSize)
    {
        if ((ds1->str = (char *)realloc(ds1->str, newLength + 1)) == NULL)
            return DYN_STRING_ERROR;

        ds1->allocSize = newLength + 1;
    }

    strcpy(ds1->str, string);
    ds1->length = newLength;

    return DYN_STRING_OK;
}

int dynamicStrCompareString(DYN_STRING *ds1, DYN_STRING *ds2)
{

    return strcmp(ds1->str, ds2->str);
}

int dynamicStrCompareConstString(DYN_STRING *ds1, char *ds2)
{
    return strcmp(ds1->str, ds2);
}

char *dynamicStrGet(DYN_STRING *ds)
{

    return ds->str;
}

int dynamicStrGetLength(DYN_STRING *ds)
{

    return ds->length;
}
