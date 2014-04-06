#pragma once
#include "Instance.h"
#include "Config.h"
class Reader
{
public:
	Reader();
	~Reader();
	static void readInput(Config & c, Instance &inst);
};

