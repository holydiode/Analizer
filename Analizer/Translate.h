#pragma once
#include "Semantic.h"
#include "TriadGenerator.h"
#include <stack>

class Analizer;

class Translate
{
public:
	Translate();
	Translate(Analizer* analize);
	void set_class();
	void process_data_type();
	void process_name();
	void inside();
	void upside();
	void complite_dec_var();
	void complite_dec_fun();
	void draw_semantic_tree();
	void chek_name();
	void add_param();
	void free_context();
	void check_is_variable();
	void init_call();
	void add_call_param();
	void set_call();
	void push_const();
	void push_var();
	void generate_assing();
	void write_triads();
	void generate_operation();
	void push_oper();
	void add_mark();
	void loop();
	void return_operation();
	void prepare_for_assign_operation();
private:
	bool name_is_exeist();
	SemanticTree tree;
	Analizer* analizer;
	NodeFactory factory;
	TriadGenerator generator;
	SemanticNode* context_object;
	std::stack<int> stak_call_param_count;
	std::stack<int> marks;
};
