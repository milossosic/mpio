#pragma once
#include <set>
class baseStation
{
public:
	int capacity;
	int scId;
	int id;

	std::set<int> users;
	baseStation();
	~baseStation();
};

