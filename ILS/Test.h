#pragma once
#include <string>
using namespace std;

class Test
{
public:
	enum {OLD = 0, NEW, CPLEX};
	int k;
	int instCount;
	string instPath;
	string instName;

	void initialize();
	void runILS(int n);

	Test();
	Test(string & path, string & name, int Count, int _k);
	~Test();
};

