#pragma once
#include "Solution.h"
#include "Config.h"
class Writer
{
public:
	float time;

	Writer();
	~Writer();
	void printResult(Config & c, Solution & s);
};

