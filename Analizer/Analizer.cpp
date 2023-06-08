#include <iostream>
#include "Analizer.h"

Analizer::Analizer()
{

}

int Analizer::run(const char* file)
{	
	scan.read(file);
	clip.set(LexType::EOF_TYPE);
	clip.set(LexType::CLASS_DEC);

	while (true)
	{
		LexType clip_lex = clip.get();
		int result = 0;
		if (clip_lex == LexType::EOF_TYPE) {
			break;
		}
		if (is_terminal(clip_lex)) {
			result = terminal_process(clip_lex);
		}
		else {
			result = non_terminal_process(clip_lex);
		}
		if (result != 0) {
			break;
		}
	}
	return 0;
}

int Analizer::terminal_process(LexType clip_lex)
{
	LexType scan_lex = scan.scan();
	if (scan_lex != clip_lex) {
		error_code = 1;
		return 1;
	}
	return 0;
}

int Analizer::non_terminal_process(LexType clip_lex)
{
	LexType next_lex = scan.watch();
	switch (clip_lex)
	{
	case(LexType::CLASS_DEC):
		class_declaration_rule();
		break;
	case(LexType::CLASS_BODY):
		if (next_lex == LexType::CLASS_KEYWORD || next_lex == LexType::STANDART_DATA_TYPE || next_lex == LexType::NAME)
			class_body_rule();
		break;
	case(LexType::IN_CLASS_DEC):
		if (next_lex == LexType::CLASS_KEYWORD) {
			in_class_decloration_rule1();
		}
		else if(next_lex == LexType::STANDART_DATA_TYPE || next_lex == LexType::NAME){
			in_class_decloration_rule2();
		}
		break;
	case(LexType::DECLOARATION_TAIL):
		switch (next_lex)
		{
		case(LexType::SEMY):
			decloaration_tail_rule1();
			break;
		case(LexType::ASSIGN_SIGN):
			decloaration_tail_rule2();
			break;
		case(LexType::LEFT_PRNT):
			decloaration_tail_rule3();
			break;
		}
		break;
	case(LexType::COMAND_LIST):
		if (next_lex == LexType::LEFT_BRK ||
			next_lex == LexType::SEMY ||
			next_lex == LexType::NAME ||
			next_lex == LexType::STANDART_DATA_TYPE ||
			next_lex == LexType::FOR_KEYWORD
			)
			comand_list_rule();
		break;
	case(LexType::OPERATOR):
		switch (next_lex)
		{
		case(LexType::FOR_KEYWORD):
			operator_rule1();
			break;
		case(LexType::NAME):
			operator_rule1();
			break;
		case(LexType::RIGHT_BRK):
			operator_rule2();
			break;
		case(LexType::SEMY):
			operator_rule3();
			break;
		}
		break;
	case(LexType::SIMPLE):
		switch (next_lex)
		{
		case(LexType::FOR_KEYWORD):
			simple_operator_rule1();
			break;
		case(LexType::RETURN_KEYWORD):
			simple_operator_rule2();
			break;
		case(LexType::IDENT):
			simple_operator_rule3();
			break;
		case(LexType::STANDART_DATA_TYPE):
			simple_operator_rule4();
			break;
		}
		break;
	case(LexType::FOR):
		for_rule();
		break;
	case(LexType::FOR_INIT):
		if (next_lex == LexType::NAME || next_lex == LexType::STANDART_DATA_TYPE) {
			for_init_rule();
		}
		break;
	case(LexType::FOR_EXPR):
		if (next_lex == LexType::NAME || next_lex == LexType::CONSTANT || next_lex == LexType::LEFT_BRK) {
			for_expr_rule();
		}
		break;
	case(LexType::EXPR):
		expr_rule();
		break;
	case(LexType::EXPR_RIGHT):
		if (next_lex == LexType::OR_SIGN)
			expr_right_rule();
		break;
	case(LexType::AND_LEFT):
		and_left_rule();
		break;
	case(LexType::AND_RIGHT):
		if (next_lex == LexType::AND_SIGN)
			and_rithg_rule();
		break;
	case(LexType::DISUNC_LEFT):
		disunc_left_rule();
		break;
	case(LexType::DISUNC_RIGHT):
		if (next_lex == LexType::DISUNC_SIGN)
			disunc_rithg_rule();
		break;
	case(LexType::CONUC_LEFT):
		conuc_left_rule();
		break;
	case(LexType::CONUC_RIGHT):
		if (next_lex == LexType::CONUC_SIGN)
			conuc_rithg_rule();
		break;
	case(LexType::EQUALS_lEFT):
		equals_left_rule();
		break;
	case(LexType::EQUALS_RIGHT):
		if (next_lex == LexType::EQUAL_SIGN || next_lex == LexType::NOT_EQUAL_SING)
			equals_rithg_rule();
		break;
	case(LexType::COMPARE_LEFT):
		compare_left_rule();
		break;
	case(LexType::COMPARE_RIGHT):
		if (next_lex == LexType::MORE_SIGN ||
			next_lex == LexType::LESS_SIGN ||
			next_lex == LexType::MORE_OR_EQUAL_SIGN ||
			next_lex == LexType::LESS_OR_EQUAL_SIGN)
			compare_rithg_rule();
		break;
	case(LexType::ADDITION_LEFT):
		adition_left_rule();
	case(LexType::ADDITION_RIGHT):
		if (next_lex == LexType::PLUS_SIGN || next_lex == LexType::MINUS_SIGN)
			adition_rithg_rule();
		break;
	case(LexType::MULTIPLE_LEFT):
		multipl_left_rule();
		break;
	case(LexType::MULTIPLE_RIGHT):
		if (next_lex == LexType::MULTIPLE_SING ||
			next_lex == LexType::MOD_SIGN ||
			next_lex == LexType::DIV_SIGN)
			compare_rithg_rule();
		break;
	case(LexType::IDENT):
		ident_rule();
		break;
	case(LexType::IDENT_TAIL):
		if (next_lex == LexType::DOT)
			ident_tail_rule();
		break;
	case(LexType::DATA_TYPE):
		if (next_lex == LexType::STANDART_DATA_TYPE)
			data_type_rule1();
		if (next_lex == LexType::NAME)
			data_type_rule2();
		break;
	case(LexType::PARAM):
		if (next_lex == LexType::NAME ||
			next_lex == LexType::STANDART_DATA_TYPE)
			params_rule();
		break;
	case(LexType::PARAM_TAIL):
		if (next_lex == LexType::COL)
			params_tail_rule();
		break;
	case(LexType::VALUE):
		value_rule();
		break;
	case(LexType::VALUE_TAIL):
		if (next_lex == LexType::LEFT_PRNT) {
			value_tail_rule();
		}
		break;
	case(LexType::CALL_PARAM): {
		if (next_lex == LexType::NAME ||
			next_lex == LexType::CONSTANT ||
			next_lex == LexType::LEFT_PRNT)
			call_param_rule();
		break;
	case(LexType::CALL_PARAM_TAIL):
		if (next_lex == LexType::COL)
			call_param_tail_rule();
		break;
	}
	case(LexType::BASIC_EXP):
		switch (next_lex)
		{
		case(LexType::NAME):
			basic_expr_rule1();
			break;
		case(LexType::CONSTANT):
			basic_expr_rule2();
			break;
		case(LexType::LEFT_PRNT):
			basic_expr_rule3();
			break;
		}
		break;
	case(LexType::IDENT_OPERATION_TAIL):
		switch (next_lex)
		{
		case(LexType::ASSIGN_SIGN):
			ident_operation_tail_rule1();
		case(LexType::LEFT_BRK):
			ident_operation_tail_rule2();
		case(LexType::NAME):
			ident_operation_tail_rule3();
		}
		break;
	case(LexType::VAR_DEC_TAIL):
		if (next_lex == LexType::NAME ||
			next_lex == LexType::LEFT_PRNT ||
			next_lex == LexType::CONSTANT)
			break;
	case(LexType::ADDITION_SIGNS):
		set_rulle(next_lex);
		break;
	case(LexType::MULTIPLE_SIGNS):
		set_rulle(next_lex);
		break;
	case(LexType::DIV_SIGN):
		set_rulle(next_lex);
		break;
	case(LexType::EQUALS_SIGNS):
		set_rulle(next_lex);
		break;
	case(LexType::COMPARE_SIGNS):
		set_rulle(next_lex);
		break;
	default:
		error_code = 2;
		return 1;
		break;
	}
	return 0;
}

void Analizer::class_declaration_rule()
{
	clip.set_array(5,
		LexType::CLASS_KEYWORD,
		LexType::NAME,
		LexType::LEFT_BRK,
		LexType::CLASS_BODY,
		LexType::RIGHT_BRK
	);
}

void Analizer::class_body_rule()
{
	clip.set_array(2, LexType::IN_CLASS_DEC, LexType::CLASS_BODY);
}

void Analizer::in_class_decloration_rule1()
{
	clip.set(LexType::CLASS_DEC);
}

void Analizer::in_class_decloration_rule2()
{
	clip.set_array(3,
		LexType::DATA_TYPE,
		LexType::NAME,
		LexType::DECLOARATION_TAIL
	);
}

void Analizer::decloaration_tail_rule1()
{
	clip.set(LexType::SEMY);
}

void Analizer::decloaration_tail_rule2()
{
	clip.set_array(3,
		LexType::ASSIGN_SIGN,
		LexType::EXPR,
		LexType::SEMY
	);
}

void Analizer::decloaration_tail_rule3()
{
	clip.set_array(6,
		LexType::LEFT_PRNT,
		LexType::PARAM,
		LexType::RIGHT_PRNT,
		LexType::LEFT_BRK,
		LexType::COMAND_LIST,
		LexType::RIGHT_BRK
	);
}

void Analizer::comand_list_rule()
{
	clip.set_array(2,
		LexType::OPERATOR,
		LexType::COMAND_LIST
	);
}

void Analizer::operator_rule1()
{
	clip.set(LexType::SIMPLE);
}

void Analizer::operator_rule2()
{
	clip.set_array(3,
		LexType::LEFT_BRK,
		LexType::COMAND_LIST,
		LexType::RIGHT_BRK
	);
}

void Analizer::operator_rule3()
{
	clip.set(LexType::SEMY);
}

void Analizer::simple_operator_rule1()
{
	clip.set(LexType::FOR);
}

void Analizer::simple_operator_rule2()
{
	clip.set_array(3,
		LexType::RETURN_KEYWORD,
		LexType::EXPR,
		LexType::SEMY
	);
}

void Analizer::simple_operator_rule3()
{
	clip.set_array(3,
		LexType::IDENT,
		LexType::IDENT_OPERATION_TAIL,
		LexType::SEMY
	);
}

void Analizer::simple_operator_rule4()
{
	clip.set_array(4,
		LexType::STANDART_DATA_TYPE,
		LexType::NAME,
		LexType::VAR_DEC_TAIL,
		LexType::SEMY,
	);
}

void Analizer::for_rule()
{
	clip.set_array(9,
		LexType::FOR_KEYWORD,
		LexType::LEFT_PRNT,
		LexType::FOR_INIT,
		LexType::SEMY,
		LexType::FOR_EXPR,
		LexType::SEMY,
		LexType::FOR_EXPR,
		LexType::RIGHT_PRNT,
		LexType::OPERATOR
	);
}

void Analizer::for_init_rule()
{
	clip.set_array(4,
		LexType::DATA_TYPE,
		LexType::IDENT,
		LexType::ASSIGN_SIGN,
		LexType::EXPR
	);
}

void Analizer::for_expr_rule()
{
	clip.set(LexType::EXPR);
}

void Analizer::assign_rule()
{
	clip.set_array(3,
		LexType::IDENT,
		LexType::ASSIGN_SIGN,
		LexType::EXPR
	);
}

void Analizer::expr_rule()
{
	clip.set_array(2,
		LexType::AND_LEFT,
		LexType::EXPR_RIGHT
	);
}

void Analizer::expr_right_rule()
{
	clip.set_array(3,
		LexType::OR_SIGN,
		LexType::EXPR,
		LexType::EXPR_RIGHT
	);
}

void Analizer::and_left_rule()
{
	clip.set_array(2,
		LexType::DISUNC_LEFT,
		LexType::AND_RIGHT
	);
}

void Analizer::and_rithg_rule()
{
	clip.set_array(3,
		LexType::AND_SIGN,
		LexType::AND_LEFT,
		LexType::AND_RIGHT
	);
}

void Analizer::disunc_left_rule()
{
	clip.set_array(2,
		LexType::CONUC_LEFT,
		LexType::DISUNC_RIGHT
	);
}

void Analizer::disunc_rithg_rule()
{
	clip.set_array(3,
		LexType::DISUNC_SIGN,
		LexType::DISUNC_LEFT,
		LexType::DISUNC_RIGHT
	);
}

void Analizer::conuc_left_rule()
{
	clip.set_array(2,
		LexType::EQUALS_lEFT,
		LexType::CONUC_RIGHT
	);
}

void Analizer::conuc_rithg_rule()
{
	clip.set_array(3,
		LexType::CONUC_SIGN,
		LexType::CONUC_LEFT,
		LexType::CONUC_RIGHT
	);
}

void Analizer::equals_left_rule()
{
	clip.set_array(2,
		LexType::COMPARE_LEFT,
		LexType::EQUALS_RIGHT
	);
}

void Analizer::equals_rithg_rule()
{
	clip.set_array(3,
		LexType::EQUALS_SIGNS,
		LexType::EQUALS_lEFT,
		LexType::EQUALS_RIGHT
	);
}

void Analizer::compare_left_rule()
{
	clip.set_array(2,
		LexType::ADDITION_LEFT,
		LexType::COMPARE_RIGHT
	);
}

void Analizer::compare_rithg_rule()
{
	clip.set_array(3,
		LexType::COMPARE_SIGNS,
		LexType::COMPARE_LEFT,
		LexType::COMPARE_RIGHT
	);
}

void Analizer::adition_left_rule()
{
	clip.set_array(2,
		LexType::MULTIPLE_LEFT,
		LexType::ADDITION_RIGHT
	);
}

void Analizer::adition_rithg_rule()
{
	clip.set_array(3,
		LexType::ADDITION_SIGNS,
		LexType::ADDITION_LEFT,
		LexType::ADDITION_RIGHT
	);
}

void Analizer::multipl_left_rule()
{
	clip.set_array(2,
		LexType::BASIC_EXP,
		LexType::CONUC_RIGHT
	);
}

void Analizer::multipl_rithg_rule()
{
	clip.set_array(3,
		LexType::MULTIPLE_SIGNS,
		LexType::MULTIPLE_LEFT,
		LexType::MULTIPLE_RIGHT
	);
}

void Analizer::basic_expr_rule1()
{
	clip.set(LexType::VALUE);
}

void Analizer::basic_expr_rule2()
{
	clip.set(LexType::CONSTANT);
}

void Analizer::basic_expr_rule3()
{
	clip.set_array(3,
		LexType::LEFT_PRNT,
		LexType::EXPR,
		LexType::RIGHT_PRNT
	);
}

void Analizer::ident_rule()
{
	clip.set_array(2,
		LexType::NAME,
		LexType::IDENT_TAIL
	);
}

void Analizer::ident_tail_rule()
{
	clip.set_array(2,
		LexType::DOT,
		LexType::IDENT
	);
}

void Analizer::data_type_rule1()
{
	clip.set(LexType::STANDART_DATA_TYPE);
}

void Analizer::data_type_rule2()
{
	clip.set(LexType::IDENT);
}

void Analizer::params_rule()
{
	clip.set_array(3,
		LexType::DATA_TYPE,
		LexType::NAME,
		LexType::PARAM_TAIL
	);
}

void Analizer::params_tail_rule()
{
	clip.set_array(2,
		LexType::COL,
		LexType::PARAM
	);
}

void Analizer::value_rule()
{
	clip.set_array(2,
		LexType::IDENT,
		LexType::VALUE_TAIL
	);
}

void Analizer::value_tail_rule()
{
	clip.set_array(3,
		LexType::LEFT_PRNT,
		LexType::CALL_PARAM,
		LexType::RIGHT_PRNT
	);
}

void Analizer::call_param_rule()
{
	clip.set_array(2,
		LexType::EXPR,
		LexType::CALL_PARAM_TAIL
	);
}

void Analizer::call_param_tail_rule()
{
	clip.set_array(2,
		LexType::COL,
		LexType::CALL_PARAM
	);
}

void Analizer::ident_operation_tail_rule1()
{
	clip.set_array(2,
		LexType::ASSIGN_SIGN,
		LexType::EXPR
	);
}

void Analizer::ident_operation_tail_rule2()
{
	clip.set_array(3,
		LexType::LEFT_PRNT,
		LexType::CALL_PARAM,
		LexType::RIGHT_PRNT
	);
}

void Analizer::ident_operation_tail_rule3()
{
	clip.set_array(2,
		LexType::NAME,
		LexType::VAR_DEC_TAIL
	);
}

void Analizer::var_dec_tail_rule()
{
	clip.set(LexType::EXPR);
}


void Analizer::set_rulle(LexType lexem)
{
	clip.set(lexem);
}




int main()
{
	Analizer anal;
	anal.run("C:\\Users\\Nik\\source\\repos\\Analizer\\x64\\Debug\\test.txt");
	printf("%d");
}