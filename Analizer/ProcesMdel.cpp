#include "ProcesMdel.h"

MemoryCursore::MemoryCursore(char* name)
{
	this->name = name;
}

MemoryCursore::MemoryCursore(const char* name)
{
	this->name = (char*)name;
}