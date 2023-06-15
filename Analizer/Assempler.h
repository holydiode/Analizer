#pragma once
#include "SemanticTree.h"
#include "TriadGenerator.h"
#include <vector>
#include <map>
#include "ProcesMdel.h"




struct AssemblerData
{
	int size  = - 1;
};

class MetaDAta;
struct Triad;

struct AssemblerComand
{
	const char* oper =  "\0";
	const char* left =  "\0";
	const char* right = "\0";

	AssemblerComand();
	AssemblerComand(const char* oper, const char* left, const char* right) {
		this->oper =oper;
		this->left = left;
		this->right = right;
	}
	AssemblerComand(const char* oper, const char* left) {
		this->oper = oper;
		this->left = left;
	}

	void print();
};

class Assemler
{
public:
	Assemler();
	Assemler(SemanticTree* tree, MetaDAta* meta, std::vector<Triad*>* triads);
	void count_sizes();
	void count_addr();
	void draw_tree();
	void print();
	std::vector<AssemblerComand*>* recount_function(std::vector<Triad*>, SemanticNode* function);
	void asemble();
private:
	std::vector<AssemblerComand*>* save_reg(MemoryCursore* cursore, int move);
	std::vector<Triad*> slicing_function(int X);
	MemoryCursore* operand_to_operand(Operand* operand);
	SemanticTree* tree;
	MetaDAta* meta;
	std::vector<Triad*>* triads;
	std::vector<AssemblerComand*>* comand;
	ProcessorModule model;
};