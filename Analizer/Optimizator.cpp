#include "Optimizator.h"

Optimizator::Optimizator(std::vector<Triad*>* triads)
{
	this->triads = triads;
}

void Optimizator::count_const()
{
	std::vector<Triad*>* remove_id = new std::vector<Triad*>();

	for (int i = 0; i < this->triads->size(); i++) { 
		Triad* cur = this->triads->at(i);

		//смотрим ссылки на коснтанты
		if (cur->left_operand &&
			cur->left_operand->link &&
			cur->left_operand->link->operation == nullptr ||
			cur->right_operand &&
			cur->right_operand->link &&
			cur->right_operand->link->operation == nullptr){

			if (cur->left_operand &&
				cur->left_operand->link &&
				cur->left_operand->link->operation == nullptr) {

				cur->left_operand = cur->left_operand->link->left_operand;

			}else {
				cur->right_operand = cur->right_operand->link->left_operand;
			}
		
		}

		//ищем вычисляемые выражения
		if (cur->left_operand &&
			cur->left_operand->is_const &&
			cur->right_operand &&
			cur->right_operand->is_const) {

			if (strcmp(cur->left_operand->source_object->name, "double") == 0) {
				double result = 0;
				double left = atoi(cur->left_operand->lex);
				double right = atoi(cur->right_operand->lex);


				switch (*cur->operation)
				{
				case('+'):
					result = left + right;
					break;
				case('-'):
					result = left - right;
					break;
				case('/'):
					result = left / right;
					break;
				case('*'):
					result = left * right;
					break;
				default:
					return;
				}
				cur->operation = nullptr;
				cur->left_operand->is_const = true;
				char* value = new char[40];
				sprintf_s(value, 40, "%f", result);
				cur->left_operand->lex = value;
				remove_id->push_back(cur);
			}
		}
	}

	//вырезаем лишние триады
	for (int i = 0; i < remove_id->size(); i++) {
		this->triads->erase(std::find(this->triads->begin(), this->triads->end(), remove_id->at(i)));
	}

	//пересчитываем индексы триад
	for (int i = 0; i < this->triads->size(); i++) {
		this->triads->at(i)->number = i;
	}
}
