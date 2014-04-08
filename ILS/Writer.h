#pragma once
#include "Config.h"
#include "Results.h"
#include "IteratedLocalSearch.h"
class Writer
{
public:
	float time;

	Writer();
	~Writer();
	void printResult(Config & c, Results & r);
	void printExtended(Config & c, Results & r);
	void printExt(Config & c, IteratedLocalSearch * ils);
};

