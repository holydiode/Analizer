#pragma once
#include <vector>
#include "TriadGenerator.h"

class Optimizator {
public :
	Optimizator(std::vector<Triad*>* triads);
	void count_const();
private:
	std::vector<Triad*>* triads;
};