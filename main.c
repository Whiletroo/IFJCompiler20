#include"gena.h"


///DEINITIONS FOR FUNCTIONS
#define ADD_INST(_inst)														\
	if ((!dynamicStrAddStr(&dyncode,(_inst))) && (!dynamicStrAddStr(&dyncode,("\n")))) return false;


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

bool codeGenOpen()
{
	ADD_INST(".IFJcode20");
	return true;
}

/***
* How can I do that function
*/
bool genArguments(tToken token){
    switch (token.token_type)
        {
        case TOKEN_INT:
            ADD_CODE("int@");
            break;
        case TOKEN_DOUBLE:
            ADD_CODE(" double@");
            break;
        case TOKEN_STRING:
            ADD_CODE(" string@");
            break;
            default:
            return  printf("[ERROR] (genArguments) frame input");
            break;
        }
     return true;
}

bool genArithm(tToken token, bool stackVersion)
{
    if (stackVersion)
        {
            switch (token.token_type)
            {
            case TOKEN_PLUS:
                ADD_CODE("ADDS ");
                break;
            case TOKEN_MINUS:
                ADD_CODE("SUBS ");
                break;
            case TOKEN_MUL:
                ADD_CODE("MULS ");
                break;
            case TOKEN_DIV:
                ADD_CODE("DIVS ");
                break;
            case TOKEN_HIGHER:
                ADD_CODE("GTS ");
                break;
            case TOKEN_LESS:
                ADD_CODE("LTS ");
                break;
            case TOKEN_EQUALS:
                ADD_CODE("EQS ");
                break;
    /*
            case TOKEN_KEYWORD:
                switch (token.attribute.keyword)
                {
                case KEYWORD_AND:
                    ADD_CODE(" ANDS ");
                    break;
                case KEYWORD_OR:
                    ADD_CODE(" ORS ");
                    break;
                case KEYWORD_NOT:
                    ADD_CODE(" NOTS ");
                    break;
                default:
                    return false;
                    break;
                }*/
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
                ADD_CODE("DIV ");
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
        //Ф-ция создаст основной фрейм функии
bool genCreaStartFream(char *nameFrame)
{
    if ( strcmp(nameFrame,"main"))
        {
        ADD_CODE("LABEL "); ADD_INST("$$main");
        ADD_INST("CREATEFRAME");
        ADD_INST("PUSHFRAME");
        }
     else {
	ADD_CODE("LABEL "); ADD_CODE("$"); ADD_CODE(nameFrame);
	ADD_INST("CREATEFRAME");
	ADD_INST("PUSHFRAME");
     }
	return true;
}
    //Функция закроет фрейм
bool genCreEndFream()
{
	ADD_INST("POPFRAME");
	ADD_INST("CLEARS");
    ADD_INST("RETURN");
	return true;
}

/*           //Нужна ли эта золупа?
bool genCreateFrame()
{
    ADD_INST("CREATEFRAME");
    return true;
    //fprintf(stdout, "CREATEFRAME\n");
}
*/
/**
 * Generates default value of variable.
 *
 * @param type Data type of variable.
 * @return True if it was successful, false otherwise.
 */
bool geneVarValue(tToken type, char *valOfType)
{
	switch (type.token_type)
	{
		case TOKEN_INT:
			ADD_CODE(" int@");
            ADD_INST(valOfType);
			break;

		case TOKEN_DOUBLE:
			ADD_CODE(" float@");
			 ADD_INST(valOfType);
			break;

		case TOKEN_STRING:
			ADD_CODE(" string@");
			ADD_INST(valOfType);
			break;

		case TOKEN_NIL:
			 ADD_INST(" nil@nil");
		default:
			return false;
	}

	return true;
}


bool genCreVal(char *nameMod)
{
    ADD_CODE("DEFVAR "); ADD_CODE("LF@"); ADD_INST(nameMod);
    return true;
}

bool geneAssignVal(char *nameMod)
{
    ADD_CODE("DEFVAR "); ADD_CODE(" LF@"); ADD_INST(nameMod);
    ADD_CODE("MOVE "); ADD_CODE(" LF@");
    return true;
}


bool geneCall(char *nameOfFunc )
{
	ADD_CODE("CALL $"); ADD_INST(nameOfFunc);
	return true;
}

bool int2Float(char *nameMod,char *retval1,bool stak){
    if (stak)
        {
            ADD_CODE("INT2FLOATS ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
            return true;
        }
    else
    {
       ADD_CODE("INT2FLOATS ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
       return true;
    }
    return false;
}

bool int2Char(char *nameMod,char *retval1,bool stak)
{
    if (stak)
        {
            ADD_CODE("INT2CHARS ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
            return true;
        }
    else
    {
      ADD_CODE("INT2CHAR ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
      return true;
    }
    return false;
}

bool float2Int(char *nameMod,char *retval1,bool stak)
{
    if (stak)
        {
            ADD_CODE("FLOAT2INTS ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
            return true;
        }
    else
    {
        ADD_CODE("FLOAT2INT ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
        return true;
    }
    return false;
}

bool string2Int(char *nameMod,char *retval1,bool stak)
{
    if (stak)
        {
            ADD_CODE("STRING2INTS ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
            return true;

        }
    else
    {
       ADD_CODE("STRING2INT ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
       return true;
    }
    return false;
}

bool genFunRead(char *nameMod,char *typeVal)
{
    ADD_CODE("READ ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" ");ADD_INST(typeVal);
    return true;
}

bool genFunWrite(char *nameMod)
{
    ADD_CODE("WRITE ");ADD_CODE("LF@");ADD_INST(nameMod);
    return true;
}

bool genConCat(char *nameMod,char *retval1)
{
    ADD_CODE("CONCAT ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" LF@");ADD_INST(retval1);
    return true;
}

bool genStrLen(char *nameMod)
{
    ADD_CODE("STRLEN ");ADD_CODE("LF@");ADD_INST(nameMod);
    return true;
}

bool genGetChar(char *nameMod,char *retval1)
{
    ADD_CODE("GETCHAR ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE("LF@");ADD_INST(retval1);
    return true;
}

bool genSetChar(char *nameMod,char *indFildMod,char *nameChar)
{
    ADD_CODE("SETCHAR ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE("int@");
    ADD_CODE(indFildMod);ADD_CODE("LF@");ADD_INST(nameChar);
    return true;
}

bool genType(char *nameMod,char *typeMod,char *nameVarMod)
{
    ADD_CODE("TYPE ");ADD_CODE("LF@");ADD_CODE(nameMod);ADD_CODE(" ");
    ADD_CODE(typeMod);ADD_CODE("@");ADD_INST(nameVarMod);
    return true;
}

bool genCreJump(char *nameFunck)
{
    ADD_CODE("JUMP ");ADD_CODE(nameFunck);
    return true;
}

bool genCreJumpEQ(char *nameFunck, char *nameVarMod, char *typeOfVar, char *valOfVar, bool stak)
{
    if (stak)
    {
        ADD_CODE("JUMPIFEQS ");ADD_CODE(nameFunck);ADD_CODE("LF@");ADD_CODE(nameVarMod);
            ADD_CODE(typeOfVar); ADD_INST(valOfVar);
            return true;
    }
    else
    {
        ADD_CODE("JUMPIFEQ ");ADD_CODE(nameFunck);ADD_CODE("LF@");ADD_CODE(nameVarMod);
            ADD_CODE(typeOfVar);ADD_INST(valOfVar);
            return true;
    }
    return false;
}

bool genCreJumpNEQ(char *nameFunck, char *nameVarMod, char *typeOfVar, char *valOfVar, bool stak)
{
    if (stak){
            ADD_CODE("JUMPIFNEQS ");ADD_CODE(nameFunck);ADD_CODE("LF@");ADD_CODE(nameVarMod);
                ADD_CODE(typeOfVar);ADD_INST(valOfVar);
                return true;
        }
    else
    {
        ADD_CODE("JUMPIFNEQ ");ADD_CODE(nameFunck);ADD_CODE("LF@");ADD_CODE(nameVarMod);
                ADD_CODE(typeOfVar);ADD_INST(valOfVar);
                return true;
    }
    return false;
}

bool genExit(char *val)
{
    ADD_CODE("EXIT ");ADD_INST(val);
    return true;
}

bool genBreak(char *val)
{
    ADD_INST("BREAK");
    return true;
}

bool genDpronr(char *valOfvar)
{
    ADD_CODE("DPRINT ");ADD_CODE("int@");ADD_INST(valOfvar);
    return true;
}





