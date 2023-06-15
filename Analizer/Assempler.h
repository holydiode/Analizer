#pragma once
#include "SemanticTree.h"
#include "TriadGenerator.h"
#include <vector>

struct AssemblerData
{
	int size  = - 1;
};

class MetaDAta;

class Assemler
{
public:
	Assemler();
	Assemler(SemanticTree* tree, MetaDAta* meta);
	void count_sizes();
	void count_addr();
	void draw_tree();
private:
	SemanticTree* tree;
	MetaDAta* meta;
};