#pragma once
#include "Lexems.h"

class Cursore
{
public:
	int row;
	int column;
};


class Scanner {
private:
	char* text;
	char* cur_char;
	Cursore skip_ignore();
	LexType word_lexem();
	LexType number_lexem();


public:
	LexType scan();
	LexType watch();
	void read(const char* filename);
};