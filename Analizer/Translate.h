#pragma once
#include "Semantic.h"
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
	void init();
	void add_param();
	void free_context();
	void check_is_variable();
	void init_call();
	void add_call_param();
	void set_call();
private:
	bool name_is_exeist();
	SemanticTree tree;
	Analizer* analizer;
	NodeFactory factory;
	SemanticNode* context_object;
	std::stack<int> stak_call_param_count;
};
