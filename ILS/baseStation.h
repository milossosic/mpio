#pragma once
#include <deque>
class baseStation
{
public:
	int capacity;
	int scId;
	int id;

	std::deque<int> users;
	baseStation();
	~baseStation();
};

