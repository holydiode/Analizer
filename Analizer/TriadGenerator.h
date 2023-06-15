#pragma once
#include "SemanticTree.h"
#include "Lexems.h"
#include "Stack"
#include "Vector"

struct Triad;

struct Operand {
    bool is_const;
    Triad* link;
    char* lex;
    SemanticNode* source_object;
};

struct Triad {
    int number;
    Operand* left_operand;
    Operand* right_operand;

    char* operation;

    void print();
    char* link_from_number(int number);
};


class TriadGenerator {
private:
    SemanticTree* tree;
    std::vector<Triad*> triads;
    std::stack<Operand*> operands;
    std::stack<char*>  operations;
    Triad* vaited_link;
    Operand* pop_operand();
public:
    TriadGenerator();
    int generate(char*  function);
    int generate();
    void push_operation(char* operation);
    void push_operand(Operand* operand);
    void push_triad(Triad* triad, int index = -1, bool baited = false);
    void loop(int start, int end);
    void call(SemanticNode* node);
    void send_init_param(char* function);
    int ret_gen(SemanticNode* data_type);
    int send_param(SemanticNode* data_type);
    int get_number_last_comand();
    void print();
};
