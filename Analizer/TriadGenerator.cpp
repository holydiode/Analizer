#include "TriadGenerator.h"

int TriadGenerator::generate(char*  type)
{
    Triad* triad = new Triad();
    triad->right_operand = this->pop_operand();
    triad->left_operand = this->pop_operand();
    if (strcmp(triad->left_operand->data_type, triad->right_operand->data_type) != 0) {
        return 1;
    }

    triad->number = triads.size();
    triad->operation = type;
    this->push_triad(triad);

    Operand* link = new Operand;
    link->is_const = false;
    link->link = triad;
    link->data_type = triad->left_operand->data_type;
    operands.push(link);
    return 0;
}

int TriadGenerator::generate()
{
    char* operation = this->operations.top();
    operations.pop();
    return generate(operation);
}

void TriadGenerator::push_operation(char* operation)
{
    this->operations.push(operation);
}

Operand* TriadGenerator::pop_operand()
{   
    Operand* oper = this->operands.top();
    this->operands.pop();
    return oper;
}

TriadGenerator::TriadGenerator()
{
    this->vaited_link = nullptr;
}

void TriadGenerator::push_operand(Operand* operand)
{
    this->operands.push(operand);
}

void TriadGenerator::push_triad(Triad* triad, int index, bool baited)
{
    if (baited) {
        this->vaited_link = triad;
    }else if (this->vaited_link) {
        this->vaited_link->left_operand->link = triad;
        this->vaited_link = nullptr;
    }
    if (index == -1) {
        this->triads.push_back(triad);
    }
    else {
        this->triads.insert(this->triads.begin() + index, triad);
        for (int i = index; i < this->triads.size(); i++) {
            this->triads[i]->number = i;
        }
    }
}

void TriadGenerator::loop(int start, int end)
{
    Operand* start_pos = new Operand();
    start_pos->link = this->triads[start];
    start_pos->is_const = false;


    Operand* end_pos = new Operand();
    end_pos->is_const = false;

    Triad* start_loop = new Triad();

    start_loop->left_operand = end_pos;
    start_loop->operation = (char*)"JIF";
    start_loop->number = this->triads.size();

    Triad* end_loop = new Triad();
    end_loop->left_operand = start_pos;
    end_loop->operation = (char*) "JMP";
    end_loop->number = this->triads.size();
    this->push_triad(end_loop);
    this->push_triad(start_loop, start + 1, true);
}

void TriadGenerator::call(char* function, int pos, char* datatype)
{
    Operand* name = new Operand();
    name->lex = function;
    name->is_const = false;

    Operand* start_pos = new Operand();
    start_pos->link = this->triads[pos];
    start_pos->is_const = false;


    Triad* ret_loop = new Triad();
    ret_loop->left_operand = name;
    ret_loop->right_operand = start_pos;
    ret_loop->operation = (char*)"CALL";
    ret_loop->number = this->triads.size();
    this->push_triad(ret_loop);

    Operand* link = new Operand;
    link->is_const = false;
    link->link = ret_loop;
    link->data_type = datatype;
    operands.push(link);
}

void TriadGenerator::send_init_param(char* name)
{
    Operand* param_name = new Operand();
    param_name->lex = name;
    param_name->is_const = false;

    Triad* operand = new Triad();
    operand->operation = (char*)"POP";
    operand->number = this->triads.size();
    this->push_triad(operand);

    Operand* link = new Operand;
    link->is_const = false;
    link->link = operand;
    operands.push(link);

    Triad* init_operand = new Triad();
    init_operand->left_operand = param_name;
    init_operand->right_operand = this->pop_operand();
    init_operand->operation = (char*)"=";
    init_operand->number = this->triads.size();
    this->push_triad(init_operand);
}

int TriadGenerator::ret_gen(char* data_type)
{
    Triad* ret_loop = new Triad();
    ret_loop->left_operand = this->pop_operand();
    if (strcmp(ret_loop->left_operand->data_type, data_type) != 0) {
        return 1;
    }
    ret_loop->operation = (char*)"RET";
    ret_loop->number = this->triads.size();
    this->push_triad(ret_loop);
    return 0;
}

int TriadGenerator::send_param(char* data_type)
{
    Triad* operand = new Triad();
    operand->left_operand = this->pop_operand();
    if (strcmp(operand->left_operand->data_type, data_type) != 0) {
        return 1;
    }
    operand->operation = (char*)"PUSH";
    operand->number = this->triads.size(); 
    this->push_triad(operand, this->get_number_last_comand() - 1);
    return 0;
}

int TriadGenerator::get_number_last_comand()
{
    return this->triads.size();
}

void TriadGenerator::print()
{
    for (int i = 0; i < this->triads.size(); i++) {
        this->triads[i]->print();
    }
}

void Triad::print()
{
    printf("%d) %s %s %s\n",
        this->number,
        this->operation,
        this->left_operand ? (this->left_operand->link? this->link_from_number(this->left_operand->link->number) : this->left_operand->lex) : "",
        this->right_operand ? (this->right_operand->link ? this->link_from_number(this->right_operand->link->number) : this->right_operand->lex) : "");
}

char* Triad::link_from_number(int number)
{
    char* link_name = new char[20];
    sprintf_s(link_name, 20, "(%d)", number);
    return link_name;
}
