#pragma once
#include <string>
using namespace std;

class Test
{
public:
	int k;
	int instCount;
	string instPath;
	string instName;

	void initialize();
	void runILS(bool n);

	Test();
	Test(string & path, string & name, int Count, int _k);
	~Test();
};

