#pragma once
#include <string>
using namespace std;

class Test
{
public:
	static enum {ILS = 0, ACO, ACO_LS};
	int k;
	int instCount;
	string instPath;
	string instName;

	void initialize();
	void runMetaheuristic(int n);

	void testACO();

	Test();
	Test(string & path, string & name, int Count, int _k);
	~Test();
};

