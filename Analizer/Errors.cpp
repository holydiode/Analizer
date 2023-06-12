#include "Errors.h"
#include "Analizer.h"

void error_messange(Analizer* analizer)
{
	switch (analizer->error_code)
	{
	case(Errors::MISS_TERMINAL):
		printf("ERROR 1: mismatch termanl on row:%d column:%d\n wait:%s\n get:%s\n",
			analizer->position().row,
			analizer->position().column,
			get_lex_type_name(analizer->last_waited_lexem()),
			analizer->last_readed_lexem());
		break;
	case(Errors::NO_RECOGNIZE_LEXEM):
		printf("ERROR 2: Unricognize Lexem");
		break;
	case(Errors::NO_CORRECT_STRUCT):
		printf("ERROR 3: incorrect construction on row:%d column:%d\n get:%s\n",
			analizer->position().row,
			analizer->position().column,
			analizer->last_readed_lexem());
		break;
	case(Errors::NAME_REZERV):
		printf("ERROR 4: name alrady used row:%d column:%d\n name:%s\n",
			analizer->position().row,
			analizer->position().column,
			analizer->last_readed_lexem());
		break;
	case(Errors::NAME_NOT_EXIST):
		printf("ERROR 5: name is not exist row:%d column:%d\n name:%s\n",
			analizer->position().row,
			analizer->position().column,
			analizer->last_readed_lexem());
		break;
	case(Errors::NO_DATA_TYPE):
		printf("ERROR 6: incorrect data type row:%d column:%d\n variable:%s\n",
			analizer->position().row,
			analizer->position().column,
			analizer->last_readed_lexem());
		break;
	case(Errors::NO_VARIABLE):
		printf("ERROR 7: is not variable row:%d column:%d\n",
			analizer->position().row,
			analizer->position().column);
		break;
	case(Errors::NO_PARAM_MATCH):
		printf("ERROR 8: param count is not mach row:%d column:%d\n",
			analizer->position().row,
			analizer->position().column);
		break;
	case(Errors::NO_DATA_TYPE_MATCH):
		printf("ERROR 9: data type not mach row:%d column:%d\n",
			analizer->position().row,
			analizer->position().column);
		break;
	default:
		break;
	}
}
