#include <iostream>
#include "Analizer.h"
#include "Assempler.h"

Analizer::Analizer()
{
	this->transaltor = Translate(this);
	this->error_code = Errors::NO_ERROR;
	this->scan = Scanner();
}

int Analizer::run(const char* file)
{	
	scan.read(file);
	clip.set(LexType::EOF_TYPE);
	clip.set(LexType::CLASS_DEC);

	while (this->error_code == Errors::NO_ERROR)
	{
		LexType clip_lex = clip.get();
		last_waited = clip_lex;
		if (clip_lex == LexType::EOF_TYPE) {
			break;
		}
		if (is_terminal(clip_lex)) {
			terminal_process(clip_lex);
		}
		else {
			non_terminal_process(clip_lex);
		}
	}
	return 0;
}

Cursore Analizer::position()
{
	return scan.position();
}

char* Analizer::last_readed_lexem()
{
	return scan.elxem();
}

LexType Analizer::last_waited_lexem()
{
	return last_waited;
}

void Analizer::draw_semantic_tree()
{
	transaltor.draw_semantic_tree();
}

void Analizer::write_triads()
{
	this->transaltor.write_triads();
}

Assemler* Analizer::get_assembler()
{
	return transaltor.get_assembler();
}

int Analizer::terminal_process(LexType clip_lex)
{
	LexType scan_lex = scan.scan();
	if (scan_lex != clip_lex) {
		error_code = Errors::MISS_TERMINAL;
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
		case(LexType::LEFT_PRNT):
			decloaration_tail_rule3();
			break;
		default:
			error_code = Errors::NO_CORRECT_STRUCT;
			return 1;
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
		case(LexType::STANDART_DATA_TYPE):
			operator_rule1();
			break;
		case(LexType::LEFT_BRK):
			operator_rule2();
			break;
		case(LexType::SEMY):
			operator_rule3();
			break;
		default:
			error_code = Errors::NO_CORRECT_STRUCT;
			return 1;
			break;
		}
		break;
	case(LexType::SIMPLE):
		switch (next_lex)
		{
		case(LexType::FOR_KEYWORD):
			simple_operator_rule1();
			break;
		case(LexType::NAME):
			simple_operator_rule3();
			break;
		case(LexType::STANDART_DATA_TYPE):
			simple_operator_rule4();
			break;
		default:
			error_code = Errors::NO_CORRECT_STRUCT;
			return 1;
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
	case(LexType::FOR_STEP):
		if (next_lex == LexType::NAME) {
			for_step_rule();
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
		break;
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
			multipl_rithg_rule();
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
			value_tail_rule1();
		}
		else {
			value_tail_rule2();
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
		default:
			error_code = Errors::NO_CORRECT_STRUCT;
			return 1;
			break;
		}
		break;
	case(LexType::IDENT_OPERATION_TAIL):
		switch (next_lex)
		{
		case(LexType::ASSIGN_SIGN):
			ident_operation_tail_rule1();
			break;
		case(LexType::LEFT_PRNT):
			ident_operation_tail_rule2();
			break;
		case(LexType::NAME):
			ident_operation_tail_rule3();
			break;
		default:
			error_code = Errors::NO_CORRECT_STRUCT;
			return 1;
			break;
		}
		break;
	case(LexType::VAR_DEC_TAIL):
		if (next_lex == LexType::ASSIGN_SIGN)
			var_dec_tail_rule();
		break;
	case(LexType::PARAMS_LIST):
		params_list_rule();
		break;
	case(LexType::CALL_PARAM_LIST):
		call_param_list_rule();
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
	case(LexType::CLASS_DECLARE_DELTA):
		transaltor.set_class();
		break;
	case(LexType::INSIDE_DELTA):
		transaltor.inside();
		break;
	case (LexType::UPSIDE_DELTA):
		transaltor.upside();
		break;
	case(LexType::SETUP_DEFAULT_TYPE_DELTA):
		transaltor.process_data_type();
		break;
	case(LexType::NAME_DECLARE_DELTA):
		transaltor.process_name();
		break;
	case(LexType::VAR_DECLARE_DELTA):
		transaltor.complite_dec_var();
		break;
	case(LexType::CHECK_NAME_DELTA):
		transaltor.chek_name();
		break;
	case(LexType::FUN_DECLARE_DELTA):
		transaltor.complite_dec_fun();
		break;
	case(LexType::FREE_CONTEXT_DELTA):
		transaltor.free_context();
		break;
	case(LexType::CHECK_VARIABLE_DELTA):
		transaltor.check_is_variable();
		break;
	case(LexType::ADD_PARAM_DELTA):
		transaltor.add_param();
		break;
	case(LexType::INIT_CALL_DELTA):
		transaltor.init_call();
		break;
	case(LexType::ADD_COLL_PARAM_DELTA):
		transaltor.add_call_param();
		break;
	case(LexType::COMPLITE_COLL_DELTA):
		transaltor.set_call();
		break;
	case(LexType::ASSIGN_OPERATION_DELTA):
		transaltor.generate_assing();
		break;
	case(LexType::CONST_OPERATION_DELTA):
		transaltor.push_const();
		break;
	case(LexType::OPERATION_DELTA):
		transaltor.generate_operation();
		break;
	case(LexType::TYPE_OPERATION_DELTA):
		transaltor.push_oper();
		break;
	case(LexType::MARK_OPERATION_DELTA):
		transaltor.add_mark();
		break;
	case(LexType::LOOP_OPERATION_DELTA):
		transaltor.loop();
		break;
	case(LexType::RETURN_OPERATION_DELTA):
		transaltor.return_operation();
		break;
	case(LexType::ASSIGN_INIT_OPERATION_DELTA):
		transaltor.prepare_for_assign_operation();
		break;
	default:
		error_code = Errors::NO_RECOGNIZE_LEXEM;
		return 1;
		break;
	}
	return 0;
}

void Analizer::class_declaration_rule()
{
	clip.set_array(7,
		LexType::CLASS_KEYWORD,
		LexType::CLASS_DECLARE_DELTA, //delta
		LexType::NAME,
		LexType::LEFT_BRK,
		LexType::CLASS_BODY,
		LexType::RIGHT_BRK,
		LexType::UPSIDE_DELTA //delta
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
	clip.set_array(4,
		LexType::DATA_TYPE,
		LexType::NAME_DECLARE_DELTA, //delta
		LexType::NAME,
		LexType::DECLOARATION_TAIL
	);
}

void Analizer::decloaration_tail_rule1()
{
	clip.set_array(3,
		LexType::SEMY,
		LexType::VAR_DECLARE_DELTA,
		LexType::FREE_CONTEXT_DELTA
		);
}

void Analizer::decloaration_tail_rule3()
{
	clip.set_array(12,
		LexType::LEFT_PRNT,
		LexType::PARAM,
		LexType::RIGHT_PRNT,
		LexType::LEFT_BRK,
		LexType::FUN_DECLARE_DELTA,
		LexType::COMAND_LIST,
		LexType::RETURN_KEYWORD,
		LexType::EXPR,
		LexType::RETURN_OPERATION_DELTA,
		LexType::SEMY,
		LexType::RIGHT_BRK,
		LexType::UPSIDE_DELTA
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
	clip.set_array(5,
		LexType::INSIDE_DELTA, // delta
		LexType::LEFT_BRK,
		LexType::COMAND_LIST,
		LexType::RIGHT_BRK,
		LexType::UPSIDE_DELTA // delta
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
	clip.set_array(7,
		LexType::DATA_TYPE,
		LexType::NAME_DECLARE_DELTA,
		LexType::NAME,
		LexType::VAR_DECLARE_DELTA,
		LexType::ASSIGN_INIT_OPERATION_DELTA,
		LexType::VAR_DEC_TAIL,
		LexType::SEMY
	);
}

void Analizer::for_rule()
{
	clip.set_array(14,
		LexType::FOR_KEYWORD,
		LexType::INSIDE_DELTA, //delta
		LexType::LEFT_PRNT,
		LexType::FOR_INIT,
		LexType::SEMY,
		LexType::MARK_OPERATION_DELTA,
		LexType::FOR_EXPR,
		LexType::SEMY,
		LexType::FOR_STEP,
		LexType::RIGHT_PRNT,
		LexType::OPERATOR,
		LexType::MARK_OPERATION_DELTA,
		LexType::LOOP_OPERATION_DELTA,
		LexType::UPSIDE_DELTA //delta
	);
}

void Analizer::for_init_rule()
{
	clip.set_array(8,
		LexType::DATA_TYPE,
		LexType::NAME_DECLARE_DELTA,  //delta         //boopa
		LexType::NAME,
		LexType::VAR_DECLARE_DELTA,   //delta
		LexType::FREE_CONTEXT_DELTA,
		LexType::ASSIGN_SIGN,
		LexType::EXPR,
		LexType::ASSIGN_OPERATION_DELTA
	);
}

void Analizer::for_expr_rule()
{
	clip.set(LexType::EXPR);
}

void Analizer::for_step_rule()
{
	clip.set_array(6,
		LexType::IDENT,
		LexType::CHECK_VARIABLE_DELTA,
		LexType::FREE_CONTEXT_DELTA,
		LexType::ASSIGN_SIGN,
		LexType::EXPR,
		LexType::ASSIGN_OPERATION_DELTA
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
	clip.set_array(4,
		LexType::OR_SIGN,
		LexType::EXPR,
		LexType::EXPR_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::AND_SIGN,
		LexType::AND_LEFT,
		LexType::AND_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::DISUNC_SIGN,
		LexType::DISUNC_LEFT,
		LexType::DISUNC_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::CONUC_SIGN,
		LexType::CONUC_LEFT,
		LexType::CONUC_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::EQUALS_SIGNS,
		LexType::EQUALS_lEFT,
		LexType::EQUALS_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::COMPARE_SIGNS,
		LexType::COMPARE_LEFT,
		LexType::COMPARE_RIGHT,
		LexType::OPERATION_DELTA
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
	clip.set_array(4,
		LexType::ADDITION_SIGNS,
		LexType::ADDITION_LEFT,
		LexType::ADDITION_RIGHT,
		LexType::OPERATION_DELTA
	);
}

void Analizer::multipl_left_rule()
{
	clip.set_array(2,
		LexType::BASIC_EXP,
		LexType::MULTIPLE_RIGHT
	);
}

void Analizer::multipl_rithg_rule()
{
	clip.set_array(4,
		LexType::MULTIPLE_SIGNS,
		LexType::MULTIPLE_LEFT,
		LexType::MULTIPLE_RIGHT,
		LexType::OPERATION_DELTA
	);
}

void Analizer::basic_expr_rule1()
{
	clip.set(LexType::VALUE);
}

void Analizer::basic_expr_rule2()
{
	clip.set_array(2,
		LexType::CONST_OPERATION_DELTA,
		LexType::CONSTANT);
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
	clip.set_array(3,
		LexType::CHECK_NAME_DELTA,
		LexType::NAME,
		LexType::IDENT_TAIL
	);
}

void Analizer::ident_tail_rule()
{
	clip.set_array(2,
		LexType::DOT,
		LexType::IDENT //хз
	);
}

void Analizer::data_type_rule1()
{
	clip.set_array(2,
		LexType::SETUP_DEFAULT_TYPE_DELTA,
		LexType::STANDART_DATA_TYPE);
}

void Analizer::data_type_rule2()
{
	clip.set(LexType::IDENT); 
}

void Analizer::params_rule()
{
	clip.set(LexType::PARAMS_LIST); 
}

void Analizer::params_list_rule() 
{
	clip.set_array(4,
		LexType::DATA_TYPE,
		LexType::ADD_PARAM_DELTA, //delta
		LexType::NAME,
		LexType::PARAM_TAIL
	);
}

void Analizer::params_tail_rule()
{
	clip.set_array(2,
		LexType::COL,
		LexType::PARAMS_LIST
	);
}

void Analizer::value_rule()
{
	clip.set_array(3,
		LexType::IDENT,
		LexType::VALUE_TAIL,
		LexType::FREE_CONTEXT_DELTA //delta
	);
}

void Analizer::value_tail_rule1()
{
	clip.set_array(5,
		LexType::INIT_CALL_DELTA,
		LexType::LEFT_PRNT,
		LexType::CALL_PARAM,
		LexType::RIGHT_PRNT,
		LexType::COMPLITE_COLL_DELTA //delta
	);
}

void Analizer::value_tail_rule2()
{
	clip.set(
		LexType::CHECK_VARIABLE_DELTA // delta
	);
}

void Analizer::call_param_rule()
{
	clip.set(LexType::CALL_PARAM_LIST);
}

void Analizer::call_param_list_rule()
{
	clip.set_array(3,
		LexType::EXPR,
		LexType::ADD_COLL_PARAM_DELTA,
		LexType::CALL_PARAM_TAIL
	);
}

void Analizer::call_param_tail_rule()
{
	clip.set_array(2,
		LexType::COL,
		LexType::CALL_PARAM_LIST
	);
}

void Analizer::ident_operation_tail_rule1()
{
	clip.set_array(5,
		LexType::CHECK_VARIABLE_DELTA,   
		LexType::FREE_CONTEXT_DELTA, 
		LexType::ASSIGN_SIGN,
		LexType::EXPR,
		LexType::ASSIGN_OPERATION_DELTA
	);
}

void Analizer::ident_operation_tail_rule2()
{
	clip.set_array(5,
		LexType::INIT_CALL_DELTA, //delta
		LexType::LEFT_PRNT,
		LexType::CALL_PARAM,
		LexType::RIGHT_PRNT,
		LexType::COMPLITE_COLL_DELTA //delta
	);
}

void Analizer::ident_operation_tail_rule3()
{
	clip.set_array(5,
		LexType::NAME_DECLARE_DELTA, 
		LexType::NAME,
		LexType::VAR_DECLARE_DELTA,
		LexType::ASSIGN_INIT_OPERATION_DELTA,
		LexType::VAR_DEC_TAIL
	);
}

void Analizer::var_dec_tail_rule()
{
	clip.set_array(3,
		LexType::ASSIGN_SIGN,
		LexType::EXPR,
		LexType::ASSIGN_OPERATION_DELTA //operation
	);
}

void Analizer::set_rulle(LexType lexem)
{
	clip.set_array(2,
		LexType::TYPE_OPERATION_DELTA,
		lexem);
}

int main()
{
	Analizer* analize = new Analizer();
	analize->run("C:\\Users\\Nik\\source\\repos\\Analizer\\x64\\Debug\\test.txt");
	if (analize->error_code != Errors::NO_ERROR) {
		error_messange(analize);
	}
	else {
		analize->draw_semantic_tree();
		analize->write_triads();
		Assemler* assembler = analize->get_assembler();
		assembler->count_sizes();
		assembler->count_addr();
		assembler->draw_tree();
		assembler->asemble();
		assembler->print();
		
	}
}