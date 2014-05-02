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
#include "AntColonyOptimization.h"
#include "AntColony_LocalSearch.h"
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

void Test::runMetaheuristic(int n)
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
				AntColonyOptimization *aco = new AntColonyOptimization();
				AntColony_LocalSearch *aco_ls = new AntColony_LocalSearch();
				Solution sol = *(new Solution());
				cout << conf.input << "." << j + 1 << endl;
				conf.outputExt << conf.input << "." << j + 1 << endl;
				conf.initialize(sol, inst);

				
				switch (n)
				{
				case ILS:
					 
					begin_time = clock();
					ils->runILSNew(sol, inst, conf); 
					results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
					results[i].solution.push_back(*(new Solution(ils->bestSolution)));
					break;
				case ACO:
					
					begin_time = clock();
					aco->runAco(sol, inst);
					results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
					results[i].solution.push_back(*(new Solution(aco->bestSolutionGlobal)));
					writer.printResult(conf, results[i]);
					break;
				case ACO_LS:

					begin_time = clock();
					aco_ls->runAcoLs(sol, inst);
					results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
					results[i].solution.push_back(*(new Solution(aco_ls->bestSolutionGlobal)));
					writer.printResult(conf, results[i]);
					break;
				}

				
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
void Test::testACO()
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
	for (int i = 0; i < 2; i++)
	{
		Instance *inst = new Instance();

		ostringstream ostr;
		ostr << instPath << "/" << instances[i];
		conf.input = ostr.str();

		reader.readInput(conf, inst);

		
			for (int ants = 4; ants <= 20; ants += 4)
			{
				for (double evap = 0.1; evap <= 0.41; evap += 0.1)
				{
					for (double alpha = 0; alpha <= 1.09; alpha += 0.2)
					{
						double beta = 1 - alpha;

						for (double power = 1; power < 5.1; power += 1)
						{
							for (int j = 0; j < 2; j++)
							{
							AntColonyOptimization *aco = new AntColonyOptimization(ants,100,100,evap,500000,power,alpha,beta);
							Solution sol = *(new Solution());
							cout << conf.input << "." << j + 1 << endl;
							conf.outputExt << conf.input << "." << j + 1 << endl;
							conf.outputExt << "ants: " << ants << " evap: " << evap << " alpha: " << alpha << " power: " << power << endl;
							
							conf.initialize(sol, inst);
							begin_time = clock();
							aco->runAco(sol, inst);
							//results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
							//results[i].solution.push_back(*(new Solution(aco->bestSolutionGlobal)));
							conf.outputExt << aco->bestSolutionGlobal.bestCost << " " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;


						}
					}
				}
			}

		}

	}
	conf.closeLog();
}
