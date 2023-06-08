#pragma once
#include "Lexems.h"

class Clip {
private:
	LexType* data;
	LexType* cur;
	int size;
public:
	Clip();
	Clip(int size);
	LexType get();
	int set(LexType data);
	int set_array(int count, ...);
};
