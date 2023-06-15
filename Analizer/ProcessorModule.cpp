#include "ProcesMdel.h"


ProcessorModule::ProcessorModule()
{
	this->EAX = new MemoryCursore("EAX");
	this->EBX = new MemoryCursore("EBX");
	this->ECX = new MemoryCursore("ECX");
	this->EDX = new MemoryCursore("EDX");

	this->regs.push_back(EAX);
	this->regs.push_back(EBX);
	this->regs.push_back(ECX);
	this->regs.push_back(EDX);
}


MemoryCursore* ProcessorModule::get_free_reg(int move)
{
	for (int i = 0; i < this->regs.size(); i++) {
		if (!regs[i]->is_reserved) {
			return regs[i];
		}
	}
	auto cursore = this->create_fake_reg(move);
}

void ProcessorModule::reserve_reg(MemoryCursore* cursore)
{

}

MemoryCursore* ProcessorModule::create_fake_reg(int move)
{
	auto name = new char[50]{ '\0' };
	sprintf_s(name, 50, "%d", move);
	strcat_s(name, 50, "(%rsp)");
	auto cursore = new MemoryCursore(name);
	cursore->is_regis = true;
	return cursore;
}