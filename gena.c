/**
 * @file gena.c
 * @author xpimen00
 * @date 4.12.2020
 * @brief The generator
*/

#include "gena.h"


///DEINITIONS FOR FUNCTIONS
#define ADD_INST(_inst) if (!dynamicStrAddStr(&dyncode, (_inst))) return false;\
    if(!dynamicStrAddChar(&dyncode,('\n'))) return false;


#define ADD_CODE(_inst) if(!dynamicStrAddStr(&dyncode, (_inst))) return false; \

DYN_STRING dyncode;

bool codeGenStart()
{
	if (!dynamicStrInit(&dyncode)) return false;
	return true;
}

void codeGenClear()
{
	dynamicStrFree(&dyncode);
}

/** ������� �������� �������
 * Generate compulsory sting for correct work
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool codeGenOpen()
{
	ADD_INST(".IFJcode20");
    ADD_INST("JUMP main");
    ADD_INST(" ");
	return true;
}



/**
 * Generate main struction of function
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreaStartFream(char *nameFrame)
{
	ADD_CODE("LABEL "); ADD_INST(nameFrame);
	ADD_INST("CREATEFRAME");

	return true;
}


/**
 * Generate PUSHFRAME command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCrePushFr()
{
    ADD_INST("PUSHFRAME");
    return true;
}

/**
 * Generate POPFRAME command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCrePopFr()
{
    ADD_INST("POPFRAME");
    return true;
}

/**
 * Generate RETURN command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreReturn()
{
    ADD_INST("RETURN");
	return true;
}


/**
 * Generate LABEL command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreateLabel(char *Label)
{
    ADD_CODE("LABEL ");ADD_INST(Label);
    return true;
}

/**
 * Generate JUMP command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genDestLabelEndJamp(char* Label)
{
    ADD_CODE("JUMP ");ADD_INST(Label);
    return true;
}

/**
 * Generates CLEARS command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreClear(){
    ADD_INST("CLEARS");
    return true;
}

 /**
 * Check deep of frame
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckFrameDeep(int FrameDeep)
{
      switch (FrameDeep)
	{
		case 1:
			ADD_CODE(" GF@");
			break;
		case 2:
			ADD_CODE(" LF@");
			break;
		case 3:
			ADD_CODE(" TF@");
			break;
		default:
			return false;
	}
    return true;
}

/**
 * Generates default value of variable.
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckTypeValue(tDataType type)
{
	switch (type)
	{
		case INT_TYPE:
			ADD_CODE(" int");

			break;

		case FLOAT_TYPE:
			ADD_CODE(" float64");

			break;

		case STRING_TYPE:
			ADD_CODE(" string");

			break;

		case NIL_TYPE:
			 ADD_CODE(" nil");
		default:
			return false;
	}
	return true;
}

/*
bool genCreStradaiSuka(int DeepFrame,char *result, char *var1,char *var2,tDataType typeValue)
{
    genCheckFrameDeep(DeepFrame);
    ADD_CODE(result);
    ADD_CODE(" ");
    ADD_CODE(var1);
    ADD_CODE(" ");
    ADD_INST(var2);
}
*/
 /**
 * Generates command from expression list
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckArithm(tToken token, bool stackVersion,tDataType typeValue)
{
    if (stackVersion)
        {
            switch (token.token_type)
            {
            case TOKEN_PLUS:
                ADD_INST("ADDS ");
                break;
            case TOKEN_MINUS:
                ADD_INST("SUBS ");
                break;
            case TOKEN_MUL:
                ADD_INST("MULS ");
                break;
            case TOKEN_DIV:
                ADD_INST("DIVS ");
                break;
            case TOKEN_HIGHER:
                ADD_INST("GTS ");
                break;
            case TOKEN_LESS:
                ADD_INST("LTS ");
                break;
            case TOKEN_EQUALS:
                ADD_INST("EQS ");
                break;
                default:
            return  printf("[ERROR] (genArithm) frame input");
            break;
                }
        }
        else
        {
            switch (token.token_type)
            {
            case TOKEN_PLUS:
                ADD_CODE("ADD ");
                break;
            case TOKEN_MINUS:
                ADD_CODE("SUB ");

                break;
            case TOKEN_MUL:
                ADD_CODE("MUL ");

                break;
            case TOKEN_DIV:
                if (typeValue==FLOAT_TYPE){
                    ADD_CODE("DIV ");
                }else{
                    ADD_CODE("IDIV ");
                }
                break;
            case TOKEN_HIGHER:
                ADD_CODE("GT ");
                break;
            case TOKEN_LESS:
                ADD_CODE("LT ");
                break;
            case TOKEN_EQUALS:
                ADD_CODE("EQ ");
                break;
             case TOKEN_NOT_EQUAL:
                ADD_CODE("NEQ ");
                break;
            default:
            return  printf("[ERROR] (genArithm) frame input");
            break;

        }
    }
    return true;
}


 /** //�-��� ����������� ����������
 * Generate deklaration of variable
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreDefVar(int FrameDeep,char *nameMod)
{
    ADD_CODE("DEFVAR ");
    genCheckFrameDeep(FrameDeep);
	ADD_INST(nameMod);
    return true;
}

/**
 * Generate CALL command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool geneCall(char *Label )
{
	ADD_CODE("CALL "); ADD_INST(Label);
	return true;
}

/**
 * Generate READ command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genFunRead(char *nameValue,int FrameDeep,tDataType typeValue)
{
    ADD_CODE("READ ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameValue);
    ADD_CODE(" ");
    genCheckTypeValue(typeValue);

    return true;
}

/**
 * Generate WRITE command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genFunWrite(char *nameMod,int FrameDeep)
{
    ADD_CODE("WRITE ");
    genCheckFrameDeep(FrameDeep);
    ADD_INST(nameMod);
    return true;
}

/**
 * Generate JUNPIFSQ command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreJumpEQ(char *Label, char *var1,int FrameDeep, char *typeOfVar, char *var2, bool stak)
{
    if (stak)
    {
        ADD_CODE("JUMPIFEQS ");
            ADD_CODE(Label);
            genCheckFrameDeep(FrameDeep);
            ADD_CODE(var1);
            ADD_CODE(typeOfVar);
            ADD_INST(var2);
            return true;
    }
    else
    {
        ADD_CODE("JUMPIFEQ ");
        ADD_CODE(Label);
        genCheckFrameDeep(FrameDeep);
        ADD_CODE(var1);
        ADD_CODE(typeOfVar);
        ADD_INST(var2);
        return true;
    }
    return false;
}
/**
 * Generate JUNPIFNEQ command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreJumpNEQ(char *Label, char *var1,int FrameDeep, char *typeOfVar, char *var2, bool stak)
{
    if (stak){
            ADD_CODE("JUMPIFEQS ");
            ADD_CODE(Label);
            genCheckFrameDeep(FrameDeep);
            ADD_CODE(var1);
            ADD_CODE(typeOfVar);
            ADD_INST(var2);
            return true;
        }
    else
    {
        ADD_CODE("JUMPIFEQ ");
        ADD_CODE(Label);
        genCheckFrameDeep(FrameDeep);
        ADD_CODE(var1);
        ADD_CODE(typeOfVar);
        ADD_INST(var2);
        return true;
    }
    return false;
}

/**
 * Generate EXIT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genExit(char *val)
{
    ADD_CODE("EXIT ");ADD_INST(val);
    return true;
}

/**
 * Generate BREAK command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genBreak()
{
    ADD_INST("BREAK");
    return true;
}

/**
 * Generate DPRINT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genDpronr(char *valOfvar,tDataType type)
{
    ADD_CODE("DPRINT ");
    genCheckTypeValue(type);
    ADD_CODE("@");
    ADD_INST(valOfvar);
    return true;
}

/**
 * Generate INT2FLOAT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool int2Float(char *retval1,bool stak){
    if (stak)
        {
            ADD_CODE("INT2FLOATS ");
            return true;
        }
    else
    {
            ADD_CODE("INT2FLOATS ");
            ADD_CODE("LF@");
            ADD_CODE(retval1);
            ADD_CODE(" int@");
            ADD_INST(retval1);
            return true;
    }
    return false;
}

/**
 * Generate INT2CHAR command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool int2Char(char *retval,bool stak)
{
    if (stak)
        {
            ADD_CODE("INT2CHARS");
            return true;
        }
    else
    {
     ADD_CODE("INT2CHAR ");
    ADD_CODE("LF@");
    ADD_CODE(retval);
    ADD_CODE(" int@");
    ADD_INST(retval);
    return true;
    }
    return false;
}

/**
 * Generate FLOAT2INT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool float2Int(char *retval, bool stak)
{
    if (stak)
        {
            ADD_CODE("FLOAT2INTS");
            return true;
        }
    else
    {
        ADD_CODE("FLOAT2INT ");
        ADD_CODE("LF@");
        ADD_CODE(retval);
        ADD_CODE("float64@");
        ADD_INST(retval);
        return true;
    }
    return false;
}

/**
 * Generate STRING2INT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool string2Int(char *retval,bool stak)
{
    if (stak)
        {
            ADD_CODE("STRING2INTS");
            return true;
        }
    else
    {
       ADD_CODE("STRING2INT ");
        ADD_CODE("FLOAT2INT ");
        ADD_CODE("LF@");
        ADD_CODE(retval);
        ADD_CODE(" string@");
        ADD_INST(retval);
        return true;
       return true;
    }
    return false;
}

/**
 * Generate CONCAT command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genConCat(char *nameMod,char *retval1,int FrameDeep,int FrameDeep1)
{
    ADD_CODE("CONCAT ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameMod);
    genCheckFrameDeep(FrameDeep1);
    ADD_INST(retval1);
    return true;
}

/**
 * Generate STRLEN command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genStrLen(char *nameMod,int FrameDeep)
{
    ADD_CODE("STRLEN ");
    genCheckFrameDeep(FrameDeep);
    ADD_INST(nameMod);
    return true;
}

/**
 * Generate STRLEGETCHARN command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genGetChar(char *nameMod,char *retval1,int FrameDeep,int FrameDeep1)
{
    ADD_CODE("GETCHAR ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameMod);
    genCheckFrameDeep(FrameDeep);
    ADD_INST(retval1);
    return true;
}

/**
 * Generate SETCHAR command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genSetChar(char *nameMod,char *indFildMod,char *nameChar,int FrameDeep,int FrameDeep1,tDataType type)
{
    ADD_CODE("SETCHAR ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameMod);
    genCheckTypeValue(type);
    ADD_CODE("@");
    ADD_CODE(indFildMod);
    genCheckFrameDeep(type);
    ADD_INST(nameChar);
    return true;
}

/**
 * Generate SETCHAR command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genType(char *varToSave,int FrameDeep,char *typeMod,char *nameOfVar,tDataType type)
{
    ADD_CODE("TYPE ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(varToSave);
    ADD_CODE(" ");
    genCheckFrameDeep(type);
    ADD_CODE("@");
    ADD_INST(nameOfVar);
    return true;
}
