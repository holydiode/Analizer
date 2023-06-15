#include "Assempler.h"
#include "Translate.h"

Assemler::Assemler(SemanticTree* tree, MetaDAta* meta)
{
	this->tree = tree;
	this->meta = meta;
}

void Assemler::count_sizes()
{
	auto classes = this->meta->classes;
	for (int i = 0; i < classes.size(); i++) {
		int size = 0;
		for (int j = 0; j < classes[i]->meta.reserved_data.size(); j++) {
			classes[i]->meta.reserved_data[j]->meta.move = size;
			size += classes[i]->meta.reserved_data[j]->source_objetc->meta.size;
		}
		classes[i]->meta.size = size;
	}

	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		int size = 0;
		int size_param = 0;
		for (int j = 0; j < functions[i]->meta.reserved_data.size(); j++) {
			if (functions[i]->meta.reserved_data[j]->meta.alloc_type != MemmoryAlocationType::Sended) {
				functions[i]->meta.reserved_data[j]->meta.move = size;
				size += functions[i]->meta.reserved_data[j]->source_objetc->meta.size;
			}
			else {
				functions[i]->meta.reserved_data[j]->meta.move = size_param;
				size_param += functions[i]->meta.reserved_data[j]->source_objetc->meta.size;
			}
		}
		functions[i]->meta.size = size;
	}
}

void Assemler::count_addr()
{
	auto functions = this->meta->functions;
	for (int i = 0; i < functions.size(); i++) {
		for (int j = 0; j < functions[i]->meta.reserved_data.size(); j++) {
			functions[i]->meta.reserved_data[j]->meta.addr = new char[50]{'\0'};
			if (functions[i]->meta.reserved_data[j]->meta.alloc_type == MemmoryAlocationType::Sended)
				functions[i]->meta.reserved_data[j]->meta.move = functions[i]->meta.size - functions[i]->meta.reserved_data[j]->meta.move;
			sprintf_s(functions[i]->meta.reserved_data[j]->meta.addr,50, "%d", functions[i]->meta.reserved_data[j]->meta.move);

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






