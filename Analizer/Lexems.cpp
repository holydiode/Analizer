#include "Lexems.h"

const char* lex_type_names[30] = {
	"EOF",
	"class",
	"{",
	"}",
	";",
	"(",
	")",
	"for",
	"||",
	"&&",
	"|",
	"&",
	"==",
	"!=",
	"<",
	">",
	"<=",
	">=",
	"+",
	"-",
	"%",
	"/",
	"*",
	"double or boolean",
	"name",
	"number or logic",
	"return",
	"=",
	".",
	","
};

const char* get_lex_type_name(LexType lexem)
{
    return lex_type_names[(int)lexem];
}

const char* get_construction_name(LexType lexem)
{
	return nullptr;
}

int is_terminal(LexType lexem)
{
    return ((int)lexem) < 100;
}
