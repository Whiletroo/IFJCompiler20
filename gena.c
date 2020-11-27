#include"gena.h"
#include"symtable.h"

///DEINITIONS FOR FUNCTIONS
#define ADD_INST(_inst)														\
	if (!dynamic_string_add_const_str(&code, (_inst "\n"))) return false


#define ADD_CODE(_code)														\
	if (!dynamicStrAddStr(&dyncode, (_code))) return false;

#define MAX_LENGTH 20

bool codeGenStart()
{
	if (!dynamicStrInit(&dyncode)) return false;
	return true;
}

void codeGenClear()
{
	dynamicStrFree(&dyncode);
}

/** Создает Главичку соубору
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



/** //Ф-ция создаст фрейм функии
 * Generate main struction of function
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreaStartFream(char *nameFrame)
{
	ADD_CODE("LABEL "); ADD_CODE(nameFrame);
	ADD_INST("CREATEFRAME");

	return true;
}

/**
 * Generate PUSHFRAME command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCrePushFr() //Ф-ция двинет фрейм
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
bool genCrePopFr()  //Ф-ция попнет фрейм
{
    ADD_INST("POPFRAME");
    return true;
}

/** //Ф-ия вызовет фрейм
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


/** //Ф-ция создаст лабде
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
bool genDestLabelEndJamp(int* Label)
{
    ADD_CODE("JUMP ");ADD_INST(Label);
    return true;
}

 /** //Ф-ция определит глубину переменной
 * Check deep of frame
 *
 *
 * @return True if it was successful, false otherwise.
 */
void genCheckFrameDeep(int *FramDeep)
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
}



 /** //Ф-ция проведет команныу из експрешена
 * Generates command from expression list
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckArithm(tToken token, bool stackVersion,char *result, char *var1,char *var2,TDataType typeValue)
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
                ADD_CODE(result);
                ADD_CODE(" ");
                ADD_CODE(var1);
                ADD_CODE(" ");
                ADD_INST(var2);
                break;
            case TOKEN_MINUS:
                ADD_CODE("SUB ");
                ADD_CODE(result);
                ADD_CODE(" ");
                ADD_CODE(var1);
                ADD_CODE(" ");
                ADD_INST(var2);
                break;
            case TOKEN_MUL:
                ADD_CODE("MUL ");
                AADD_CODE(result);
                ADD_CODE(" ");
                ADD_CODE(var1);
                ADD_CODE(" ");
                ADD_INST(var2);
                break;
            case TOKEN_DIV:
                if (typeValue==FLOAT){
                    ADD_CODE("DIV ");
                    ADD_CODE(result);
                    ADD_CODE(" ");
                    ADD_CODE(var1);
                    ADD_CODE(" ");
                    ADD_INST(var2);
                }else{
                    ADD_CODE("IDIV ");
                    ADD_CODE(result);
                    ADD_CODE(" ");
                    ADD_CODE(var1);
                    ADD_CODE(" ");
                    ADD_INST(var2);
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


/** //Ф-ция определяет тип переменной
 * Generates default value of variable.
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCheckTypeValue(TDataType type)
{
	switch (type)
	{
		case INT:
			ADD_CODE(" int");

			break;

		case FLOAT:
			ADD_CODE(" float64");

			break;

		case STRING:
			ADD_CODE(" string");

			break;

		case NIL:
			 ADD_CODE(" nil");
		default:
			return false;
	}
	return true;
}


 /** //Ф-ция декларирует переменную
 * Generate deklaration of variable
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genCreDefVar(int *FrameDeep,char *nameMod)
{
    ADD_CODE("DEFVAR ");
    genCheckFrameDeep(FrameDeep);
	ADD_INST(nameMod);
    return true;
}

/** //Ф-ция вызовет перемнную
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

/** //Ф-ция вызовет перемнную
 * Generate READ command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genFunRead(char nameValue,int FrameDeep,TDataType typeValu)
{
    ADD_CODE("READ ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameMod);
    ADD_CODE(" ");
    genCheckFrameDeep(typeValue);

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
bool genCreJumpEQ(char *Label, char *var1,int *FrameDeep, char *typeOfVar, char *var2, bool stak)
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
bool genCreJumpNEQ(char *Label, char *var1,int *FrameDeep, char *typeOfVar, char *var2, bool stak)
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
bool genDpronr(char *valOfvar,TDataType type)
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
bool int2Float(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1,bool stak){
    if (stak)
        {
            ADD_CODE("INT2FLOATS ");
            return true;
        }
    else
    {
            ADD_CODE("INT2FLOATS ");
        genCheckFrameDeep(FrameDeep);
            ADD_CODE(nameMod);
        genCheckFrameDeep(FrameDeep1);
            ADD_INST(retval1);
            return true;
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
bool int2Char(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1,bool stak)
{
    if (stak)
        {
            ADD_CODE("INT2CHARS");
            return true;
        }
    else
    {
      ADD_CODE("INT2CHAR ");
       genCheckFrameDeep(FrameDeep);
      ADD_CODE(nameMod);
       genCheckFrameDeep(FrameDeep);
       ADD_INST(retval1);
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
bool float2Int(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1,bool stak)
{
    if (stak)
        {
            ADD_CODE("FLOAT2INTS");
            return true;
        }
    else
    {
        ADD_CODE("FLOAT2INT ");
        genCheckFrameDeep(FrameDeep);
        ADD_CODE(nameMod);
        genCheckFrameDeep(FrameDeep1);
        ADD_INST(retval1);
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
bool string2Int(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1,bool stak)
{
    if (stak)
        {
            ADD_CODE("STRING2INTS");
            return true;
        }
    else
    {
       ADD_CODE("STRING2INT ");
       genCheckFrameDeep(FrameDeep);
       ADD_CODE(nameMod);
       genCheckFrameDeep(FrameDeep1);
       ADD_INST(retval1);
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
bool genConCat(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1,)
{
    ADD_CODE("CONCAT ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(nameMod);
    genCheckFrameDeep(FrameDeep);
    ADD_INST(retval1);
    return true;
}

/**
 * Generate STRLEN command
 *
 *
 * @return True if it was successful, false otherwise.
 */
bool genStrLen(char *nameMod,int *FrameDeep)
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
bool genGetChar(char *nameMod,char *retval1,int *FrameDeep,int *FrameDeep1)
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
bool genSetChar(char *nameMod,char *indFildMod,char *nameChar,int *FrameDeep,int *FrameDeep1,TDataType type)
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
bool genType(char *varToSave,int *FrameDeep,char *typeMod,char *nameOfVar,TDataType type)
{
    ADD_CODE("TYPE ");
    genCheckFrameDeep(FrameDeep);
    ADD_CODE(varToSave);
    ADD_CODE(" ");
    genCheckFrameDeep(type);
    ADD_CODE("@");
    ADD_INST(nameVarMod);
    return true;
}
