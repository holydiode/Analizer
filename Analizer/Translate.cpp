#include "Translate.h"
#include "Analizer.h"
#include <string.h>
SemanticNode* boolean_class = new SemanticNode();
SemanticNode* double_class = new SemanticNode();


Translate::Translate()
{
}

Translate::Translate(Analizer* analize)
{
	tree = SemanticTree();
	generator = TriadGenerator();
	this->analizer = analize;
	this->context_object = nullptr;
	boolean_class->name = (char*)"boolean";
	boolean_class->type = ObjectType::CLASS;
	boolean_class->insert(new SemanticNode());

	double_class->name = (char*)"double";
	double_class->type = ObjectType::CLASS;
	double_class->insert(new SemanticNode());
}

void Translate::set_class()
{
	SemanticNode* node = new SemanticNode();
	node->type = ObjectType::CLASS;
	node->name = analizer->last_readed_lexem();
	if (name_is_exeist()) {
		analizer->error_code = Errors::NAME_REZERV;
	}
	tree.add(node);
	tree.region();

}

void Translate::process_data_type()
{
	char* data_type_name = analizer->last_readed_lexem();
	if (strcmp(data_type_name, "boolean") == 0) {
		context_object = boolean_class;
	}else if(strcmp(data_type_name, "double") == 0) {
		context_object = double_class;
	}
}

void Translate::process_name()
{

	if (context_object->type != ObjectType::CLASS) {
		analizer->error_code = Errors::NO_DATA_TYPE;
		return;
	}
	factory.set_class_object(context_object);
	
	context_object = nullptr;
	if (name_is_exeist()) {
		analizer->error_code = Errors::NAME_REZERV;
	}
	else {
		factory.set_name(analizer->last_readed_lexem());
	}
}

void Translate::inside()
{
	tree.region();
}

void Translate::upside()
{
	tree.up();
}

void Translate::complite_dec_var()
{
	SemanticNode* node = factory.create();
	node->type = ObjectType::VAR;
	node->insert(node->source_objetc->inside()->deep_object_copy());
	this->context_object = nullptr;
	tree.add(node);
}

void Translate::complite_dec_fun()
{
	SemanticNode* node = factory.create();
	node->type = ObjectType::FUN;
	this->context_object = nullptr;
	node->func_start = this->generator.get_number_last_comand() - node->count_param * 2;
	tree.add(node);
	tree.region();
	for (int i = 0; i < node->count_param; i++) {
		this->tree.next();
	}
}

void Translate::draw_semantic_tree()
{
	tree.draw();
}

bool Translate::name_is_exeist() {
	SemanticNode* node = nullptr;
	char* name = this->analizer->last_readed_lexem();
	if (this->context_object) {
		node = this->context_object->find_inside(name);
	}
	else {
		node = this->tree.find(name);
	}
	if (node) {
		this->context_object = node;
		return true;
	}
	return false;
}

void Translate::chek_name()
{
	if (!name_is_exeist()) {
		analizer->error_code = Errors::NAME_NOT_EXIST;
	}
}

void Translate::add_param()
{
	SemanticNode* node = new SemanticNode();
	node->name = this->analizer->last_readed_lexem();
	node->type = ObjectType::VAR;
	if (factory.get()->find_inside(node->name) != nullptr) {
		analizer->error_code = Errors::NAME_REZERV;
		return;
	}
	if (context_object->type != ObjectType::CLASS) {
		analizer->error_code = Errors::NO_DATA_TYPE;
		return;
	}
	node->source_objetc = this->context_object;
	node->insert(node->source_objetc->inside()->deep_object_copy());
	this->factory.add_param(node);
	this->generator.send_init_param(this->analizer->last_readed_lexem());
	context_object = nullptr;
}

void Translate::free_context()
{
	this->context_object = nullptr;
}

void Translate::check_is_variable()
{
	if (this->context_object->type != ObjectType::VAR)
		analizer->error_code = Errors::NO_VARIABLE;

	Operand* operand = new Operand();
	operand->is_const = false;
	operand->link = nullptr;
	operand->data_type = this->context_object->datatype();
	operand->lex = this->context_object->extended_name(true);
	generator.push_operand(operand);
}

void Translate::init_call()
{
	if (this->context_object->type != ObjectType::FUN) {
		this->analizer->error_code = Errors::NO_FUN;
	}
	this->stak_call_param_count.push(this->context_object->count_param);
	this->generator.call(this->context_object->extended_name(true), this->context_object->func_start, this->context_object->datatype());
}

void Translate::add_call_param()
{
	SemanticNode* function = context_object;
	this->context_object = nullptr;
	int count = this->stak_call_param_count.top();
	this->stak_call_param_count.pop();
	count--;
	stak_call_param_count.push(count);
	SemanticNode* data_holder = function->inside()->next();
	for (int i = 0; i < function->count_param - count - 1; i++) {
		data_holder = data_holder->next();
	}
	if (this->generator.send_param(data_holder->datatype())) {
		analizer->error_code = Errors::NO_DATA_TYPE_MATCH;
	}
	this->context_object = function;
}

void Translate::set_call()
{
	int count_param = this->stak_call_param_count.top();
	this->stak_call_param_count.pop();
	if (count_param != 0) {
		this->analizer->error_code = Errors::NO_PARAM_MATCH;
	}
	this->context_object = nullptr;
}

void Translate::push_const()
{
	Operand* operand = new Operand();
	operand->is_const = true;
	operand->link = nullptr;
	operand->lex = this->analizer->last_readed_lexem();
	if (*operand->lex == 't' || *operand->lex == 'f') {
		operand->data_type = (char*)"boolean";
	}
	else {
		operand->data_type = (char*)"double";
	}
	generator.push_operand(operand);
}

void Translate::push_var() {
	Operand* operand = new Operand();
	operand->is_const = false;
	operand->link = nullptr;
	operand->data_type = this->context_object->datatype();
	operand->lex = this->context_object->name;
	generator.push_operand(operand);
}

void Translate::push_oper()
{
	this->generator.push_operation(this->analizer->last_readed_lexem());
}

void Translate::add_mark()
{
	this->marks.push(this->generator.get_number_last_comand());
}

void Translate::loop()
{
	int finish = this->marks.top();
	this->marks.pop();
	int start = this->marks.top();
	this->marks.pop();
	this->generator.loop(start, finish);
}

void Translate::return_operation()
{
	if (this->generator.ret_gen(this->tree.current()->outside()->datatype())) {
		analizer->error_code = Errors::NO_DATA_TYPE_MATCH;
	};
}

void Translate::prepare_for_assign_operation()
{
	Operand* operand = new Operand();
	operand->data_type = this->context_object->full_name();
	operand->is_const = false;
	operand->lex = this->analizer->last_readed_lexem();
	this->generator.push_operand(operand);
}

void Translate::generate_assing()
{
	this->generator.generate((char*)"=");
}

void Translate::write_triads()
{
	this->generator.print();
}

void Translate::generate_operation()
{
	if (this->generator.generate()) {
		analizer->error_code = Errors::NO_DATA_TYPE_MATCH;
	}
}