#include <ctime>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <cstdlib>
#include <random>
#include "Config.h"
#include "Solution.h"
#include "Instance.h"
#include <sstream>
#include "Test.h"

std::default_random_engine Config::generator(time(NULL));

Config::Config()
{
	//output.open("log.txt");
}


Config::~Config()
{
}

void Config::openLog(string instName, int n)
{
	ostringstream ostr;
	//Test::metaheuristic mh = (Test::metaheuristic)n;
	string mh;
	switch (n)
	{
	case 0: mh = "ILS"; break;
	case 1: mh = "ACO"; break;
	case 2: mh = "ACO+LS"; break;
	}
	ostr << "results/" << instName << "_" << mh << ".txt";
	
	outputExt.open(ostr.str());
}

void Config::closeLog()
{
	outputExt.close();
}

bool Config::comparePairsAsc(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}

bool Config::comparePairsDsc(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second > b.second;
}


void Config::initialize(Solution & s, Instance * inst)
{
	s.originalBaseStations.resize(inst->bsOldCount + inst->bsNewCount);
	inst->users.clear();
	inst->users.resize(inst->usCount);

	for (int i = 0; i < inst->usCount; i++)
	{
		for (int j = 0; j < inst->bsOldCount + inst->bsNewCount; j++)
		{
			int rad = inst->usBsRadius[i][j];
			if (rad < inst->bsRadius)
			{
				inst->users[i].bsSet.push_back(make_pair(j, rad));
				s.originalBaseStations[j].users.insert(i);
			}
		}
	}
	inst->initialize();
	s.initialize(inst);
}

int Config::Rand()
{
	uniform_int_distribution<int> distribution(1, 2000202);
	return distribution(generator);
}
double Config::RandDouble()
{
	uniform_real_distribution<double> dist;
	return dist(generator);
}


vector<string> Config::dirList(string dir, string instName)
{
	vector<string> ret;
	WIN32_FIND_DATA ffd;
	wchar_t path[100];
	int i = 0;
	do
	{
		path[i] = dir.c_str()[i];
	} while (dir.c_str()[i++] != 0);
	
	if (path[_tcslen(path) - 1] != '\\')
		_tcscat(path, _T("\\"));
	_tcscat(path, _T("*.*"));

	HANDLE hFind = FindFirstFile(path, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//cerr << _T("Invalid handle value.") << GetLastError() << endl;
		return ret;
	}

	bool finished = false;
	bool diffe;
	while (!finished)
	{
		//cout << ffd.cFileName << endl;
		char name[50];
		int i = 0;
		do
		{
			name[i] = ffd.cFileName[i];
		} while (ffd.cFileName[i++] != 0);
		if (instName.size() > 0)
		{
			if (name[0] == '.')
			{
				if (!FindNextFile(hFind, &ffd))
					finished = true;
				continue;
			}
			diffe = false;
			for (int i = 0; i < instName.size() && !diffe; i++)
			{
				if (instName[i] != name[i])
				{
					if (!FindNextFile(hFind, &ffd))
						finished = true;
					diffe = true;
				}
			}
			if (diffe)
				continue;
		}
		ret.push_back(name);
		if (ret[ret.size() - 1][0] == '.')
			ret.erase(ret.begin() + ret.size()-1);

		if (!diffe && !FindNextFile(hFind, &ffd))
			finished = true;
	}

	return ret;
}