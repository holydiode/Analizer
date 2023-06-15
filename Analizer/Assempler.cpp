#include "Assempler.h"
#include "Translate.h"
#include "ProcesMdel.h"
#include <map>



Assemler::Assemler()
{
}

Assemler::Assemler(SemanticTree* tree, MetaDAta* meta, std::vector<Triad*>* triads)
{
	this->tree = tree;
	this->meta = meta;
	this->triads = triads;
	this->comand = new std::vector<AssemblerComand*>();
}

void Assemler::count_sizes()
{
	auto classes = this->meta->classes;
	for (int i = 0; i < classes.size(); i++) {
		int size = 0;
		for (int j = 0; j < classes[i]->meta.reserved_data.size(); j++) {
			classes[i]->meta.reserved_data[j]->meta.sizes->move = size;
			size += classes[i]->meta.reserved_data[j]->source_objetc->meta.sizes->size;
		}
		classes[i]->meta.sizes->size = size;
	}

	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		int size = 0;
		int size_param = 0;
		for (int j = 0; j < functions[i]->meta.reserved_data.size(); j++) {
			if (functions[i]->meta.reserved_data[j]->meta.alloc_type != MemmoryAlocationType::Sended) {
				functions[i]->meta.reserved_data[j]->meta.sizes->move = size;
				size += functions[i]->meta.reserved_data[j]->source_objetc->meta.sizes->size;
			}
			else {
				functions[i]->meta.reserved_data[j]->meta.sizes->move = size_param;
				size_param += functions[i]->meta.reserved_data[j]->source_objetc->meta.sizes->size;
			}
		}
		functions[i]->meta.sizes->size = size;
	}
}

void Assemler::count_addr()
{
	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		for (int j = 0; j < functions[i]->meta.reserved_data.size(); j++) {
			functions[i]->meta.reserved_data[j]->meta.addr = new char[50]{'\0'};
			if (functions[i]->meta.reserved_data[j]->meta.alloc_type == MemmoryAlocationType::Sended)
				functions[i]->meta.reserved_data[j]->meta.sizes->move = functions[i]->meta.sizes->size - functions[i]->meta.reserved_data[j]->meta.sizes->move;
			sprintf_s(functions[i]->meta.reserved_data[j]->meta.addr,50, "%d", functions[i]->meta.reserved_data[j]->meta.sizes->move);

			switch (functions[i]->meta.reserved_data[j]->meta.alloc_type) {
			case (MemmoryAlocationType::Sended):
				strcat_s(functions[i]->meta.reserved_data[j]->meta.addr, 50, "(%rpb)");
				break;
			case (MemmoryAlocationType::Object):
				strcat_s(functions[i]->meta.reserved_data[j]->meta.addr, 50, "(%rsp)");
				break;
			case (MemmoryAlocationType::Local):
				strcat_s(functions[i]->meta.reserved_data[j]->meta.addr, 50, "(%rdi)");
				break;
			default:
				break;
			};
		}
	}
}

void Assemler::draw_tree()
{
	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		functions[i]->draw_meta(0);
	}
}

void Assemler::print()
{
	for (int i = 0; i < this->comand->size(); i++) {
		this->comand->at(i)->print();
	}
}

std::vector<AssemblerComand*>* Assemler::recount_function(std::vector<Triad*> triads, SemanticNode* function)
{
	std::vector<AssemblerComand*>* comands = new std::vector<AssemblerComand*>();
	
	for (int i = 0; i < triads.size(); i++) {
		auto cur = triads[i];
		switch (*cur->operation)
		{
		case('+'): {
			auto left_operand = operand_to_operand(cur->left_operand);
			auto right_operand = operand_to_operand(cur->right_operand);
			
			auto save = save_reg(model.EAX, function->meta.sizes->size);
			if (save->size() > 0) {
				function->meta.sizes->size += model.EAX->size;
				comands->reserve(comands->size() + save->size());
				comands->insert(comands->end(), save->begin(), save->end());
			}
			model.EAX->value = cur;
			comands->push_back(new AssemblerComand("ADD", left_operand->name, right_operand->name));
			model.EAX->is_reserved = true;
			cur->mem = model.EAX;
		}break;
		case('='): {
			auto left_operand = operand_to_operand(cur->left_operand);
			auto right_operand = operand_to_operand(cur->right_operand);
			comands->push_back(new AssemblerComand("MOV", left_operand->name, right_operand->name));
		}break;
		case('*'): {
			auto left_operand = operand_to_operand(cur->left_operand);
			auto right_operand = operand_to_operand(cur->right_operand);

			auto save = save_reg(model.EAX, function->meta.sizes->size);
			if (save->size() > 0) {
				function->meta.sizes->size += model.EAX->size;
				comands->reserve(comands->size() + save->size());
				comands->insert(comands->end(), save->begin(), save->end());
			}
			if (left_operand == model.EAX) {
				comands->push_back(new AssemblerComand("MUL", right_operand->name));
			}
			else if (right_operand == model.EAX) {
				comands->push_back(new AssemblerComand("MUL", left_operand->name));
			}
			else {
				comands->push_back(new AssemblerComand("MOV", "EAX", right_operand->name));
				comands->push_back(new AssemblerComand("MUL", left_operand->name));
			}
			model.EAX->value = cur;
			model.EAX->is_reserved = true;
			cur->mem = model.EAX;
		}break;
		case('%'): {
			auto left_operand = operand_to_operand(cur->left_operand);
			auto save = save_reg(model.EAX, function->meta.sizes->size);

			if (save->size() > 0) {
				function->meta.sizes->size += model.EAX->size;
				comands->reserve(comands->size() + save->size());
				comands->insert(comands->end(), save->begin(), save->end());
			}

			auto right_operand = operand_to_operand(cur->right_operand);
			save = save_reg(model.EDX, function->meta.sizes->size);

			if (save->size() > 0) {
				function->meta.sizes->size += model.EAX->size;
				comands->reserve(comands->size() + save->size());
				comands->insert(comands->end(), save->begin(), save->end());
			}

			if (left_operand != model.EAX) {
				comands->push_back(new AssemblerComand("MOV", "EAX", left_operand->name));
			}
			comands->push_back(new AssemblerComand("DIV", right_operand->name));
			
			model.EDX->value = cur;
			model.EDX->is_reserved = true;
			cur->mem = model.EDX;
		}break;

		default:
			break;
		}
	}
	return comands;
}

void Assemler::asemble()
{
	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		auto fun = this->recount_function(this->slicing_function(functions[i]->func_start), functions[i]);
		this->comand->reserve(this->comand->size() + fun->size());
		this->comand->insert(this->comand->end(), fun->begin(), fun->end());
	}
}

std::vector<AssemblerComand*>* Assemler::save_reg(MemoryCursore* cursore, int move)
{
	std::vector<AssemblerComand*>* comands = new std::vector<AssemblerComand*>();
	if (cursore->is_reserved) {
		auto free = model.get_free_reg(move);
		free->is_reserved = true;
		cursore->value->mem = free;
		comands->push_back(new AssemblerComand("MOV", free->name, cursore->name));
	}
	return comands;
}

std::vector<Triad*> Assemler::slicing_function(int X)
{
	auto start = this->triads->begin() + X;
	auto end = this->triads->end();
	int i = X;
	for (;i < this->triads->size(); i++) {
		if (strcmp(this->triads->at(i)->operation, "RET") == 0) {
			end = this->triads->begin() + i;
			break;
		}
	}

	std::vector<Triad*> result(i - X + 1);
	copy(start, end + 1, result.begin());
	return result;
}

MemoryCursore* Assemler::operand_to_operand(Operand* operand)
{
	if (operand->is_const) {
		if (strcmp(operand->source_object->name, "boolean") == 0) {
			if (strcmp(operand->lex, "true") == 0) {
				return new MemoryCursore("1");
			}
			else {
				return new MemoryCursore("0");
			}
		}
		else {
			return new MemoryCursore(operand->lex);
		}
	}

	else if(operand->link) {
		operand->link->mem->is_reserved = false;
		return operand->link->mem;
	}
	else {
		if (operand->init_node->meta.addr == nullptr) {
			operand->init_node->meta.addr = new char[50]{ '\0' };
			int move = operand->init_node->meta.sizes->move;
			SemanticNode* node = operand->init_node->outside();
			while (node->outside()->type == ObjectType::VAR)
			{
				move += node->meta.sizes->move;
				node = node->outside();
			}
			move += node->meta.sizes->move;
			sprintf_s(operand->init_node->meta.addr, 50, "%d", move);

			switch (node->meta.alloc_type) {
			case (MemmoryAlocationType::Sended):
				strcat_s(operand->init_node->meta.addr, 50, "(%rpb)");
				break;
			case (MemmoryAlocationType::Object):
				strcat_s(operand->init_node->meta.addr, 50, "(%rsp)");
				break;
			case (MemmoryAlocationType::Local):
				strcat_s(operand->init_node->meta.addr, 50, "(%rdi)");
				break;
			default:
				break;
			};
		}
		return new MemoryCursore(operand->init_node->meta.addr);
	}
}

AssemblerComand::AssemblerComand()
{
	oper = "\0";
	left = "\0";
	right = "\0";
}

void AssemblerComand::print()
{
	printf("    %s %s %s\n", this->oper, this->left, this->right);
}
