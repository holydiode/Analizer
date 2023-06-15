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
	boolean_class->meta.sizes->size = 1;


	double_class->name = (char*)"double";
	double_class->type = ObjectType::CLASS;
	double_class->insert(new SemanticNode());
	double_class->meta.sizes->size = 8;
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
	this->meta.classes.push_back(node);
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
	tree.add(node);
	SemanticNode* base = node->outside();
	while (base->type != ObjectType::FUN && base->type != ObjectType::CLASS)
	{
		base = base->outside();
	}
	if (base->type == ObjectType::FUN)
		node->meta.alloc_type = MemmoryAlocationType::Local;
	if(base->type == ObjectType::CLASS)
		node->meta.alloc_type = MemmoryAlocationType::Object;

	base->meta.reserved_data.push_back(node);
	this->context_object = node;
}

void Translate::complite_dec_fun()
{
	SemanticNode* node = factory.create();
	node->type = ObjectType::FUN;
	this->context_object = nullptr;
	node->func_start = this->generator.get_number_last_comand() - node->count_param;
	tree.add(node);
	tree.region();
	for (int i = 0; i < node->count_param; i++) {
		this->tree.next();
	}
	this->meta.functions.push_back(node);
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
	this->generator.send_init_param(node);
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
	operand->source_object = this->context_object->source_objetc;
	operand->lex = this->context_object->extended_name(true);
	operand->init_node = this->context_object;
	generator.push_operand(operand);
}

void Translate::init_call()
{
	if (this->context_object->type != ObjectType::FUN) {
		this->analizer->error_code = Errors::NO_FUN;
	}
	this->stak_call_param_count.push(this->context_object->count_param);
	this->generator.call(this->context_object);
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
	if (this->generator.send_param(data_holder->source_objetc)) {
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
		operand->source_object = boolean_class;
	}
	else {
		operand->source_object = double_class;
	}
	generator.push_operand(operand);
}

void Translate::push_var() {
	Operand* operand = new Operand();
	operand->is_const = false;
	operand->link = nullptr;
	operand->source_object = this->context_object->source_objetc;
	operand->lex = this->context_object->name;
	operand->init_node = this->context_object;
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
	if (this->generator.ret_gen(this->tree.current()->outside()->source_objetc)) {
		analizer->error_code = Errors::NO_DATA_TYPE_MATCH;
	};
}

void Translate::prepare_for_assign_operation()
{
	Operand* operand = new Operand();
	operand->source_object = this->context_object->source_objetc;
	operand->init_node = this->context_object;
	operand->is_const = false;
	operand->lex = this->context_object->name;
	this->generator.push_operand(operand);
	this->free_context();
}

Assemler* Translate::get_assembler()
{
	return new Assemler(&this->tree, &this->meta, &generator.triads);
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