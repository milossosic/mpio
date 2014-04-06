#pragma once
#include <utility>
#include <vector>
using namespace std;
class user
{

public:
	int id;
	int bsId;
	//par id, rastojanje
	vector<pair<int, int>> bsSet;
	user();
	~user();
};

