#pragma once
#include "Clip.h"
#include "Scanner.h"
#include "Lexems.h"
#include "Translate.h"
#include "Errors.h"

class Analizer {
public:
	Analizer();
	int run(const char* file);
	Errors  error_code;
	Cursore position();
	char* last_readed_lexem();
	LexType last_waited_lexem();
	void draw_semantic_tree();
private:
	Clip clip;
	Scanner scan;
	LexType last_waited;
	Translate transaltor;

	int terminal_process(LexType);
	int non_terminal_process(LexType);

	void class_declaration_rule();
	void class_body_rule();
	
	void in_class_decloration_rule1();
	void in_class_decloration_rule2();

	void decloaration_tail_rule1();
	void decloaration_tail_rule2();
	void decloaration_tail_rule3();

	void comand_list_rule();

	void operator_rule1();
	void operator_rule2();
	void operator_rule3();


	void simple_operator_rule1();
	void simple_operator_rule2();
	void simple_operator_rule3();
	void simple_operator_rule4();


	void for_rule();
	void for_init_rule();
	void for_expr_rule();

	void expr_rule();
	void expr_right_rule();


	void and_left_rule();
	void and_rithg_rule();

	void disunc_left_rule();
	void disunc_rithg_rule();

	void conuc_left_rule();
	void conuc_rithg_rule();

	void equals_left_rule();
	void equals_rithg_rule();

	void compare_left_rule();
	void compare_rithg_rule();

	void adition_left_rule();
	void adition_rithg_rule();

	void multipl_left_rule();
	void multipl_rithg_rule();

	void basic_expr_rule1();
	void basic_expr_rule2();
	void basic_expr_rule3();

	void ident_rule();
	void ident_tail_rule();

	void data_type_rule1();
	void data_type_rule2();


	void params_rule();
	void params_list_rule();
	void params_tail_rule();


	void value_rule();
	void value_tail_rule1();
	void value_tail_rule2();
	void call_param_rule();
	void call_param_list_rule();
	void call_param_tail_rule();
	void ident_operation_tail_rule1();
	void ident_operation_tail_rule2();
	void ident_operation_tail_rule3();
	void var_dec_tail_rule();

	void set_rulle(LexType lexem);
};