#pragma once
#include "Config.h"
#include "Results.h"
class Writer
{
public:
	float time;

	Writer();
	~Writer();
	void printResult(Config & c, Results & r);
	void printExtended(Config & c, Results & r);
};

