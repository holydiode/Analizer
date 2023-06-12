#pragma once
enum class ObjectType
{
	REGION = 0,
	CLASS = 1,
	FUN = 2,
	VAR = 3,
	CONST = 4
};



class SemanticNode {
public:
	ObjectType type;
	SemanticNode* source_objetc;

	char* name;
	bool is_init;
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
	char* extended_name(bool is_top);
	SemanticNode* find_inside(char* name);
	SemanticNode* find_abowe(char* name);
	SemanticNode* deep_object_copy();
	void draw(int tab);
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
	void init();
private:
	SemanticNode* node;
};
