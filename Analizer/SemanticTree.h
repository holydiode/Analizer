#pragma once
#include "Vector"

enum class ObjectType
{
	REGION = 0,
	CLASS = 1,
	FUN = 2,
	VAR = 3,
	CONST = 4
};

class SemanticNode;

enum class MemmoryAlocationType
{
	Sended,
	Local,
	Object,
};

struct NodeMetadata
{
	int size = 0;
	int move = 0;
	char* addr = (char*)"null";
	MemmoryAlocationType alloc_type;
	std::vector<SemanticNode*> reserved_data;
};


class SemanticNode {
public:
	ObjectType type;
	SemanticNode* source_objetc;
	NodeMetadata meta;

	char* name;
	int count_param;
	int func_start;

	SemanticNode();
	void insert(SemanticNode* node);
	void beside(SemanticNode* node);
	SemanticNode* outside();
	SemanticNode* inside();
	SemanticNode* next();
	SemanticNode* find_upside(char* name);
	char* full_name();
	char* assemble_name();
	char* extended_name(bool is_top);
	SemanticNode* find_inside(char* name);
	SemanticNode* find_abowe(char* name);
	SemanticNode* deep_object_copy();
	void draw(int tab);
	void draw_meta(int tab);
	char* datatype();
private:
	SemanticNode* left;
	SemanticNode* right;
	SemanticNode* up;
};


class SemanticTree
{
public:
	void up();
	void add(SemanticNode* mode);
	void region();
	void draw();
	void draw_meta();
	void next();
	SemanticNode* current();
	SemanticNode* find(char* name);
private:
	SemanticNode* root;
	SemanticNode* cursore;
};


class NodeFactory {
public:
	NodeFactory();
	SemanticNode* create();
	SemanticNode* get();
	void set_name(char* name);
	void set_type(ObjectType type);
	void add_param(SemanticNode* node);
	void set_class_object(SemanticNode* node);
private:
	SemanticNode* node;
};
