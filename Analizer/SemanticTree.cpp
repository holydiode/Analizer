#include "SemanticTree.h"
#include <iostream>

SemanticNode::SemanticNode()
{
	name = nullptr;
	count_param = 0;
	func_start = 0;
	left = nullptr;
	right = nullptr;
	up = nullptr;
}

void SemanticNode::insert(SemanticNode* node)
{
	if (!this->right) {
		node->up = this;
		this->right = node;
	}
	else {
		this->right->beside(node);
	}
}

void SemanticNode::beside(SemanticNode* node)
{
	if (!this->left) {
		node->up = this;
		this->left = node;
	}
	else {
		this->left->beside(node);
	}
}

SemanticNode* SemanticNode::outside()
{
	while (this->up)
	{
		if (this->up->right == this)
			return this->up;
		return this->up->outside();
	}
	return nullptr;
}

SemanticNode* SemanticNode::inside()
{
	return this->right;
}

SemanticNode* SemanticNode::next()
{
	return this->left;
}

SemanticNode* SemanticNode::find_upside(char* name)
{
	if (this->name && strcmp(this->name, name) == 0)
		return this;
	if (this->up)
		return this->up->find_upside(name);
	return nullptr;
}

char* SemanticNode::full_name()
{
	char* full_name = new char[100];
	full_name[0] = '\0';
	if (this->up)
		strcpy_s(full_name, 100, this->outside()->full_name());
	if (name) {
		if (this->outside())
			strcat_s(full_name, 100, ".");
		strcat_s(full_name, 100, name);
	}
	return full_name;
}


char* SemanticNode::assemble_name()
{
	char* full_name = new char[100];
	full_name[0] = '\0';
	if (this->up)
		strcpy_s(full_name, 100, this->outside()->full_name());
	if (name) {
		if (this->outside())
			strcat_s(full_name, 100, "_");
		strcat_s(full_name, 100, name);
	}
	return full_name;
}

char* SemanticNode::extended_name(bool is_top = true)
{
	char* full_name = new char[100];
	full_name[0] = '\0';
	if (this->up)
		strcpy_s(full_name, 100, this->outside()->extended_name(false));
	if (is_top) {
		if (this->outside() && this->outside()->type == ObjectType::VAR)
			strcat_s(full_name, 100, ".");
		strcat_s(full_name, 100, name);
	}else if (name && type == ObjectType::VAR) {
		if (this->outside() && this->outside()->type == ObjectType::VAR)
			strcat_s(full_name, 100, ".");
		strcat_s(full_name, 100, name);
	}
	return full_name;
}


const char* object_type_name[5] = { "", "Class","Fun", "Var", ""};

SemanticNode* SemanticNode::find_inside(char* name)
{
	if (this->right)
		return this->right->find_abowe(name);
	return nullptr;
}

SemanticNode* SemanticNode::find_abowe(char* name)
{
	if (this->name && strcmp(this->name, name) == 0)
		return this;
	if (this->left)
		return this->left->find_abowe(name);
	return nullptr;
}

SemanticNode* SemanticNode::deep_object_copy()
{
	if (this->type == ObjectType::CLASS)
		return this->left->deep_object_copy();
	SemanticNode* copy = new SemanticNode();
	copy->count_param = count_param;
	copy->type = type;
	copy->meta.sizes = meta.sizes;
	copy->func_start = func_start;
	copy->source_objetc = source_objetc;
	copy->name = name;
	if (left) {
		copy->left = left->deep_object_copy();
		copy->left->up = copy;
	}
	if (right) {
		copy->right = right->deep_object_copy();
		copy->right->up = copy;
	}
	return copy;
}


void SemanticNode::draw(int tab)
{
	if (this->name) {
		for (int i = 0; i < tab; i++)
			printf("  ");
		switch (this->type)
		{
		case(ObjectType::VAR):
			printf("VAR %s:%s\n", this->name, this->datatype());
			break;
		case(ObjectType::CLASS):
			printf("CLASS %s\n", this->name);
			break;
		case(ObjectType::FUN):
			printf("VAR %s:%s(", this->name, this->datatype());
			SemanticNode* param = this->inside();
			for (int i = 0; i < this->count_param; i++) {
				param = param->next();
				printf("%s:%s", param->name, param->datatype());
				if (i + 1 != this->count_param)
					printf(",");
			}
			printf(")\n");
			break;
		}
	}
	if (this->right)
		if(this->type != ObjectType::FUN || ! this->outside() || this->outside()->type != ObjectType::VAR)
			this->right->draw(tab + 1);
	if (this->left)
		this->left->draw(tab);
}

void SemanticNode::draw_meta(int tab)
{
	if (this->name) {
		for (int i = 0; i < tab; i++)
			printf("  ");
		switch (this->type)
		{
		case(ObjectType::VAR):
			printf("VAR %s:(%s)\n", this->name, this->meta.addr);
			break;
		case(ObjectType::FUN):
			printf("FUN %s, sizeof(%d)\n", this->assemble_name(), this->meta.sizes->size);
			break;
		}
	}
	if (this->right)
		if (this->type != ObjectType::VAR)
			this->right->draw_meta(tab + 1);
	if (this->left)
		this->left->draw_meta(tab);
}

char* SemanticNode::datatype()
{
	return this->source_objetc->full_name();
}

void SemanticTree::up()
{
	SemanticNode* up_node = this->cursore->outside();
	if (up_node) {
		this->cursore = up_node;
	}
}

void SemanticTree::add(SemanticNode* node)
{
	if(!this->root){
		this->root = node;
		this->cursore = this->root;
	}
	else{
		this->cursore->beside(node);
		this->cursore = node;
	}
}

void SemanticTree::region() //проверить работу на вложенные параметры
{
	if (this->cursore->inside()) {
		this->cursore = this->cursore->inside();
	}
	else {
		SemanticNode* region = new SemanticNode();
		this->cursore->insert(region);
		this->cursore = region;
	}
}

void SemanticTree::draw()
{
	if(root)
		root->draw(0);
}

void SemanticTree::draw_meta()
{
	if (root)
		root->draw_meta(0);
}

void SemanticTree::next()
{
	this->cursore = this->cursore->next();
}

SemanticNode* SemanticTree::current()
{
	return this->cursore;
}

SemanticNode* SemanticTree::find(char* name)
{
	if (!cursore)
		return nullptr;
	return this->cursore->find_upside(name);
}


NodeFactory::NodeFactory()
{
	this->node = new SemanticNode();
}

SemanticNode* NodeFactory::create()
{
	SemanticNode* result_node = node;
	node = new SemanticNode();
	return result_node;
}

SemanticNode* NodeFactory::get()
{
	return this->node;
}

void NodeFactory::set_name(char* name)
{
	node->name = name;
}

void NodeFactory::set_type(ObjectType type)
{
	node->type = type;
}

void NodeFactory::add_param(SemanticNode* node)
{
	if(!this->node->inside())
		this->node->insert(new SemanticNode());
	this->node->count_param++;
	this->node->insert(node);
	this->node->meta.reserved_data.push_back(node);
}

void NodeFactory::set_class_object(SemanticNode* node)
{
	this->node->source_objetc = node;
}