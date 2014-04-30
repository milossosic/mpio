#include <sstream>
#include <iostream>
#include <ctime>
#include "IteratedLocalSearch.h"
#include "Reader.h"
#include "Writer.h"
#include "Solution.h"
#include "Config.h"
#include "Instance.h"
#include "Test.h"
#include "Results.h"

Test::Test()
{
}

Test::Test(string & path, string & name, int count, int _k) :
 instCount(count), k(_k)
{
	instPath = path;
	instName = name;
}
Test::~Test()
{
}

void Test::runILS(int n)
{
	Config conf;
	Reader reader;
	Writer writer;
	clock_t begin_time;
	vector<Results> results;
	conf.openLog();
	
	results.resize(instCount);
	
	vector<string> instances;
	
	instances = Config::dirList(instPath, instName);

	for (int i = 0; i < instances.size();i++)
	{
			Instance *inst = new Instance();
			
			ostringstream ostr;
			ostr << instPath << "/" << instances[i];
			conf.input = ostr.str();

			reader.readInput(conf, inst);
			for (int j = 0; j < k; j++)
			{
				IteratedLocalSearch *ils = new IteratedLocalSearch();
				Solution sol = *(new Solution());
				cout << conf.input << "." << j + 1 << endl;
				conf.outputExt << conf.input << "." << j + 1 << endl;
				conf.initialize(sol, inst);

				begin_time = clock();
				switch (n)
				{
				case NEW:	ils->runILSNew(sol, inst, conf); break;
				}

				results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
				results[i].solution.push_back(*(new Solution(ils->bestSolution)));
			}
		}
		
	

	for (int j = 0; j < instances.size(); j++)
	{
		results[j].calculate();
		writer.printExtended(conf, results[j]);
	}

	conf.closeLog();
}

void Test::initialize()
{

}