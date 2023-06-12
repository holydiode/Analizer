#pragma once
enum class Errors
{
	NO_ERROR,
	MISS_TERMINAL,
	NO_RECOGNIZE_LEXEM,
	NO_CORRECT_STRUCT,
	NAME_REZERV,
	NAME_NOT_EXIST,
	NO_DATA_TYPE,
	NO_VARIABLE,
	NO_FUN,
	NO_PARAM_MATCH,
	NO_DATA_TYPE_MATCH
};
class Analizer;

void error_messange(Analizer* analizer);