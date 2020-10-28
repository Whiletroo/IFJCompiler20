#include"gena.h"


///DEINITIONS FOR FUNCTIONS
#define ADD_INST(_inst)														\
	if (!dynamicStrAddStr(&dyncode, (_inst"\n"))) return false

#define ADD_CODE(_code)														\
	if (!dynamicStrAddStr(&dyncode, (_code))) return false

#define MAX_LENGTH 20



/// built-in functions
// Length(s As String) As Integer
#define FUNCTION_LENGTH														\
	"\n # Built-in function Length"											\
	"\n LABEL $length"														\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%retval"													\
	"\n STRLEN LF@%retval LF@%0"											\
	"\n POPFRAME"															\
	"\n RETURN"

// SubStr(s As String, i As Integer, n As Integer) As String
#define FUNCTION_SUBSTR														\
	"\n # Built-in function SubStr"											\
	"\n LABEL $substr"														\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%retval"													\
	"\n MOVE LF@%retval string@"											\
	"\n DEFVAR LF@length_str"												\
	"\n CREATEFRAME"														\
	"\n DEFVAR TF@%0"														\
	"\n MOVE TF@%0 LF@%0"													\
	"\n CALL $length"														\
	"\n MOVE LF@length_str TF@%retval"										\
	"\n DEFVAR LF@ret_cond"													\
	"\n LT LF@ret_cond LF@length_str int@0"									\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n EQ LF@ret_cond LF@length_str int@0"									\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n LT LF@ret_cond LF@%1 int@0"											\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n EQ LF@ret_cond LF@%1 int@0"											\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n GT LF@ret_cond LF@%1 LF@length_str"									\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n EQ LF@ret_cond LF@%2 int@0"											\
	"\n JUMPIFEQ $substr$return LF@ret_cond bool@true"						\
	"\n DEFVAR LF@max_n"													\
	"\n MOVE LF@max_n LF@length_str"										\
	"\n SUB LF@max_n LF@max_n LF@%1"										\
	"\n ADD LF@max_n LF@max_n int@1"										\
	"\n DEFVAR LF@edit_n_cond"												\
	"\n LT LF@edit_n_cond LF@%2 int@0"										\
	"\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true"					\
	"\n GT LF@edit_n_cond LF@%2 LF@max_n"									\
	"\n JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true"					\
	"\n JUMP $substr$process"												\
	"\n LABEL $substr$edit_n"												\
	"\n MOVE LF@%2 LF@max_n"												\
	"\n LABEL $substr$process"												\
	"\n DEFVAR LF@index"													\
	"\n MOVE LF@index LF@%1"												\
	"\n SUB LF@index LF@index int@1"										\
	"\n DEFVAR LF@char"														\
	"\n DEFVAR LF@process_loop_cond"										\
	"\n LABEL $substr$process_loop"											\
	"\n GETCHAR LF@char LF@%0 LF@index"										\
	"\n CONCAT LF@%retval LF@%retval LF@char"								\
	"\n ADD LF@index LF@index int@1"										\
	"\n SUB LF@%2 LF@%2 int@1"												\
	"\n GT LF@process_loop_cond LF@%2 int@0"								\
	"\n JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true"		\
	"\n LABEL $substr$return"												\
	"\n POPFRAME"															\
	"\n RETURN"

// Asc(s As String, i As Integer) As Integer
#define FUNCTION_ASC														\
	"\n # Built-in function Asc"											\
	"\n LABEL $asc"															\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%retval"													\
	"\n MOVE LF@%retval int@0"												\
	"\n DEFVAR LF@cond_length"												\
	"\n LT LF@cond_length LF@%1 int@1"										\
	"\n JUMPIFEQ $asc$return LF@cond_length bool@true"						\
	"\n DEFVAR LF@length_str"												\
	"\n CREATEFRAME"														\
	"\n DEFVAR TF@%0"														\
	"\n MOVE TF@%0 LF@%0"													\
	"\n CALL $length"														\
	"\n MOVE LF@length_str TF@%retval"										\
	"\n GT LF@cond_length LF@%1 LF@length_str"								\
	"\n JUMPIFEQ $asc$return LF@cond_length bool@true"						\
	"\n SUB LF@%1 LF@%1 int@1"												\
	"\n STRI2INT LF@%retval LF@%0 LF@%1"									\
	"\n LABEL $asc$return"													\
	"\n POPFRAME"															\
	"\n RETURN"

// Chr(i As Integer) As String
#define FUNCTION_CHR														\
	"\n # Built-in function Chr"											\
	"\n LABEL $chr"															\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%retval"													\
	"\n MOVE LF@%retval string@"											\
	"\n DEFVAR LF@cond_range"												\
	"\n LT LF@cond_range LF@%0 int@0"										\
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true"						\
	"\n GT LF@cond_range LF@%0 int@255"										\
	"\n JUMPIFEQ $chr$return LF@cond_range bool@true"						\
	"\n INT2CHAR LF@%retval LF@%0"											\
	"\n LABEL $chr$return"													\
	"\n POPFRAME"															\
	"\n RETURN"


/**
 * Defines built-in functions.
 *
 * @return True if it was successful, false otherwise.
 */
static bool defBuildFunc()
{
	ADD_INST(FUNCTION_LENGTH);
	ADD_INST(FUNCTION_SUBSTR);
	ADD_INST(FUNCTION_ASC);
	ADD_INST(FUNCTION_CHR);

	return true;
}

bool codeGenStart()
{
	if (!dynamicStrInit(&dyncode)) return false;

	if (!defBuildFunc()) return false;

	return true;
}

void codeGenClear()
{
	dynamicStrFree(&dyncode);
}
/***
* How can I do that function
*/
bool genArguments(tToken token){
    switch (token.token_type)
        {
        case TOKEN_INT:
            ADD_CODE("int@");
            //fprintf(stdout, "int@%i\n", token.attribute.value_int);
            break;
        case TOKEN_DOUBLE:
            ADD_CODE(" double@");
            int2SAVE_CODE_main_double(token.attribute.value_double);
            SAVE_CODE_main("\n");
            //fprintf(stdout, "float@%lf\n", token.attribute.value_double);
            break;
        case TOKEN_STRING:
            ADD_CODE(" string@");
            SAVE_CODE_main(genWriteString(token));
            SAVE_CODE_main("\n");
            //fprintf(stdout, "string@");
            //genWriteString(token);
            //fprintf(stdout, "\n");
            break;
            default:
            return  printf("[ERROR] (genArguments) frame input");
            break;
        }
}

bool genArithm(tToken token, bool stackVersion)
{
    if (stackVersion)
        {
            switch (token.token_type)
            {
            case TOKEN_PLUS:
                ADD_INST("ADDS ");
                //fprintf(stdout, "ADD");
                break;
            case TOKEN_MINUS:
                ADD_INST("SUBS ");
                //fprintf(stdout, "SUB");
                break;
            case TOKEN_MUL:
                ADD_INST("MULS ");
                //fprintf(stdout, "MUL");
                break;
            case TOKEN_DIV:
                ADD_INST("DIVS ");
                //fprintf(stdout, "DIV");
                break;
            case TOKEN_HIGHER:
                ADD_INST("GT ");
                //fprintf(stdout, "GT");
                break;
            case TOKEN_LESS:
                ADD_INST("LT ");
                //fprintf(stdout, "LT");
                break;
            case TOKEN_EQUALS:
                ADD_INST("EQS ");
                //fprintf(stdout, "EQS");
                break;
                /*  We need that part?
            case TOKEN_KEYWORD:
                switch (token.attribute.keyword)
                {
                case KEYWORD_AND:
                    SAVE_CODE_main(" ANDS ");
                    //fprintf(stdout, " ANDS");
                    break;
                case KEYWORD_OR:
                    SAVE_CODE_main(" ORS ");
                    //fprintf(stdout, " ORS");
                    break;
                case KEYWORD_NOT:
                    SAVE_CODE_main(" NOTS ");
                    //fprintf(stdout, " NOTS");
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
                ADD_INST("ADD ");
                //fprintf(stdout, "ADD ");
                break;
            case TOKEN_MINUS:
                ADD_INST("SUB ");
                //fprintf(stdout, "SUB ");
                break;
            case TOKEN_MUL:
                ADD_INST("MUL ");
                //fprintf(stdout, "MUL ");
                break;
            case TOKEN_DIV:
                ADD_INST("DIV ");
                //fprintf(stdout, "DIV ");
                break;
            case TOKEN_HIGHER:
                ADD_INST("GT ");
                //fprintf(stdout, "GT");
                break;
            case TOKEN_LESS:
                ADD_INST("LT ");
                //fprintf(stdout, "LT");
                break;
            case TOKEN_EQUALS:
                ADD_INST("EQ ");
                //fprintf(stdout, "EQ");
                break;
            default:
            return  printf("[ERROR] (genArithm) frame input");
            break;

        }
    }
}




