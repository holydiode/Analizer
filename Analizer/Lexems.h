#pragma once


enum class LexType {
	EOF_TYPE = 0,
	CLASS_KEYWORD = 1,
	LEFT_BRK = 2,
	RIGHT_BRK = 3,
	SEMY = 4,
	LEFT_PRNT = 5,
	RIGHT_PRNT = 6,
	FOR_KEYWORD = 7,
	OR_SIGN = 8,
	AND_SIGN = 9,
	DISUNC_SIGN = 10,
	CONUC_SIGN = 11,
	EQUAL_SIGN = 12,
	NOT_EQUAL_SING = 13,
	LESS_SIGN = 14,
	MORE_SIGN = 15,
	LESS_OR_EQUAL_SIGN = 16,
	MORE_OR_EQUAL_SIGN = 17,
	PLUS_SIGN = 18,
	MINUS_SIGN = 19,
	MOD_SIGN = 20,
	DIV_SIGN = 21,
	MULTIPLE_SING = 22,
	STANDART_DATA_TYPE = 23,
	NAME = 24,
	CONSTANT = 25,
	RETURN_KEYWORD = 26,
	ASSIGN_SIGN = 27,
	DOT = 28,
	COL = 29,


	IDENT = 101,
	CLASS_BODY = 102,
	IN_CLASS_DEC = 103,
	CLASS_DEC = 104,
	DATA_TYPE = 105,
	DECLOARATION_TAIL = 106,
	EXPR = 108,
	PARAM = 109,
	COMAND_LIST = 110,
	OPERATOR = 111,
	SIMPLE = 112,
	FOR = 113,
	FOR_INIT = 114,
	FOR_EXPR = 115,
	EXPR_RIGHT = 117,

	AND_LEFT = 118,
	AND_RIGHT = 119,

	DISUNC_LEFT = 120,
	DISUNC_RIGHT = 121,

	CONUC_LEFT = 122,
	CONUC_RIGHT = 123,

	EQUALS_lEFT = 124,
	EQUALS_RIGHT = 125,
	EQUALS_SIGNS = 126,


	COMPARE_LEFT = 127,
	COMPARE_RIGHT = 128,
	COMPARE_SIGNS = 129,


	ADDITION_LEFT = 130,
	ADDITION_RIGHT = 131,
	ADDITION_SIGNS = 132,


	MULTIPLE_LEFT = 133,
	MULTIPLE_RIGHT = 134,
	MULTIPLE_SIGNS = 135,

	BASIC_EXP = 136,
	IDENT_TAIL = 137,
	PARAM_TAIL = 138,
	VALUE = 139,
	CALL_PARAM = 140,
	CALL_PARAM_TAIL = 141,
	VALUE_TAIL = 142,
	IDENT_OPERATION_TAIL = 143,
	VAR_DEC_TAIL = 144,
	PARAMS_LIST = 145,
	CALL_PARAM_LIST = 146,


	CLASS_DECLARE_DELTA = 500,
	INSIDE_DELTA = 501,
	UPSIDE_DELTA = 502,
	SETUP_DEFAULT_TYPE_DELTA = 504,
	NAME_DECLARE_DELTA = 505,
	VAR_DECLARE_DELTA = 506,
	FUN_DECLARE_DELTA = 507,
	INIT_DELTA = 508,
	CHECK_NAME_DELTA = 509,
	FREE_CONTEXT_DELTA = 510,
	CHECK_VARIABLE_DELTA = 511,
	ADD_PARAM_DELTA = 512,
	INIT_CALL_DELTA = 513,
	ADD_COLL_PARAM_DELTA = 514,
	COMPLITE_COLL_DELTA = 515
};

const char* get_lex_type_name(LexType lexem);
const char* get_construction_name(LexType lexem);

int is_terminal(LexType lexem);