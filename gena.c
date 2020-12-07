/**
 * @file gena.c
 * @author xpimen00
 * @date 7.12.2020
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

/**
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
bool genCreaStartFrame(char *nameFrame)
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
 * Generate MOVE command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreMove(char* name1,char *name2,tDataType type)
{
    ADD_CODE("MOVE ");
    ADD_CODE("LF@");
    ADD_CODE(name1);
    ADD_CODE(" ");
    genCheckTypeValue(type);
    ADD_CODE("@");
    ADD_CODE(name2);
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
		    ADD_CODE("LF");
			return false;
	}
	return true;
}

bool proid(char *name1,char *name2,char *name3)
{
    ADD_CODE("LF@");
    ADD_CODE(name1);
    ADD_CODE(" ");
    ADD_CODE("LF@");
    ADD_CODE(name2);
    ADD_CODE(" ");
    ADD_CODE("LF@");
    ADD_INST(name3);
    return true;
}


 /**
 * Generates command from expression list
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckArithm(tPrecRules rule, char *name1,char *name2,char *name3)
{
            switch (rule)
            {
            case E_PLUS_E:
                ADD_CODE("ADD ");
                proid(name1,name2,name3);
                break;
            case E_MINUS_E:
                ADD_CODE("SUB ");
                proid(name1,name2,name3);
                break;
            case E_MUL_E:
                ADD_CODE("MUL ");
                proid(name1,name2,name3);
                break;
            case E_DIV_E:
                ADD_CODE("DIV ");
                proid(name1,name2,name3);
                 break;
            case E_IDIV_E:
                ADD_CODE("IDIV ");
                proid(name1,name2,name3);
                break;
            case E_HTN_E:
                ADD_CODE("GT ");
                proid(name1,name2,name3);
                break;
            case E_LTN_E:
                ADD_CODE("LT ");
                proid(name1,name2,name3);
                break;
            case E_EQ_E:
                ADD_CODE("EQ ");
                proid(name1,name2,name3);;
                break;
             case E_NEQ_E:
                ADD_CODE("NEQ ");
                proid(name1,name2,name3);
                break;
            case E_LEQ_E:
                ADD_CODE("LEQ ");
                proid(name1,name2,name3);
                break;
            case E_HEQ_E:
                ADD_CODE("HEQ ");
                proid(name1,name2,name3);
                break;
            default:
            return  printf("[ERROR] (genArithm) frame input");
            break;
    }

    return true;
}

 /**
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
bool genConCat(char *name1,char *name2)
{
    ADD_CODE("CONCAT ");
    ADD_CODE("LF@");
    ADD_CODE(name1);
    ADD_CODE("LF@");
    ADD_INST(name2);
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
