#pragma once
#include "Solution.h"
#include "Config.h"
class Writer
{
public:
	Writer();
	~Writer();
	void printResult(Config & c, Solution & s);
};

