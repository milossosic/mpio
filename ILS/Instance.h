#pragma once
#include <queue>
#include "user.h"
class Instance
{
public:
	int usCount;
	int bsOldCount;
	int bsNewCount;
	int scOldCount;
	int scNewCount;

	int scCapacity;
	int bsCapacity;

	int scCost;
	int bsCost;

	int bsRadius;
	int usBsRadius[1000][100];
	int bsScConnCost[100][100];
	int bsScConnMatrix[1000][100];

	deque<user> users;

	void initialize();
	Instance();
	Instance(Instance & i);
	~Instance();
};

