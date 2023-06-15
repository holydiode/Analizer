#pragma once
#include <vector>

class Triad;

struct MemoryCursore
{
	MemoryCursore(char* name);
	MemoryCursore(const char* name);
	char* name = (char*)"\0";
	bool is_regis = false;
	bool is_reserved = false;
	int size = 4;
	Triad* value;
};


class ProcessorModule
{
public:
	ProcessorModule();
	MemoryCursore* get_free_reg(int move);
	void reserve_reg(MemoryCursore* cursore);
	std::vector<MemoryCursore*> regs;

	MemoryCursore* EAX;
	MemoryCursore* EBX;
	MemoryCursore* ECX;
	MemoryCursore* EDX;
private:
	MemoryCursore* create_fake_reg(int move);
};