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

void Translate::init()
{
	this->factory.init();
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
	node->is_init = true;
	this->factory.add_param(node);
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
}

void Translate::init_call()
{
	if (this->context_object->type != ObjectType::FUN) {
		this->analizer->error_code = Errors::NO_FUN;
	}
	this->stak_call_param_count.push(this->context_object->count_param);
	this->context_object = nullptr;
}


void Translate::add_call_param()
{
	int count = this->stak_call_param_count.top();
	this->stak_call_param_count.pop();
	count--;
	stak_call_param_count.push(count);
}

void Translate::set_call()
{
	int count_param = this->stak_call_param_count.top();
	this->stak_call_param_count.pop();
	if (count_param != 0) {
		this->analizer->error_code = Errors::NO_PARAM_MATCH;
	}
}
