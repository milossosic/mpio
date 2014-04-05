#include <iostream>
#include <fstream> 
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <list>
#include <sstream>
#include <random>
#include <functional>
#include <cstdlib>
#include <ctime>
using namespace std;

string input;
ofstream output;
const int MAX_ITER = 1000;
int currentIter = 0;
int noImprovementCount;
int previousCost;
int currentCost;

int currentBsCount;
int randConnSc;
int randBs;

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



deque<int> currentBaseStations;
deque<int> currentSwitchingCenters;

deque<int> usCovered;


int bsCoverCount[100];

int bsScConnCost[100][100];
int usBsConn[1000];
int bsScConn[100];
int bsScConnMatrix[100][100];

bool bsSolution[1000];
bool scSolution[100];

int bsCapacities[100];
int scCapacities[100];
int cap;

default_random_engine generator;

class user
{

public:
	int id;
	int bsId;
	//par id, rastojanje
	vector<pair<int, int>> bsSet;
	user() : bsId(-1)
	{};
	~user()
	{};
};

class switchingCenter
{
public:
	int id;
	int capacity;
	switchingCenter() : capacity(scCapacity)
	{};
	~switchingCenter(){};
};

vector<int> usPossibleBs[1000];
deque<user> users;

struct compare1 {
	bool operator() (const int a, const int b) const{
		return users[a].bsSet.size() < users[b].bsSet.size();
	}
};

bool comparePairs(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}

class baseStation
{
public:
	int capacity;
	int scId;
	set<int> usCoveredSet;
	set<int> usSet;
	int id;
	baseStation() : scId(-1)
	{};
	~baseStation(){};
};

deque<baseStation> originalBaseStations;
deque<switchingCenter> switchingCenters;
deque<baseStation> baseStations;

bool compareUsByBsCount(user& a, user& b)
{
	return a.bsSet.size() < b.bsSet.size();
}

deque<int> bsUsedIds;
deque<baseStation> bsUsed;
deque<pair<int, int>> bsScConnList;

class Solution
{
public:
	deque<int> scSet;
	deque<pair<int, int>> bsSet;
	int bsFixed;
	Solution(){};
	Solution(Solution & sol)
	{
		scSet = deque<int>(sol.scSet);
		bsSet = deque<pair<int, int>>(sol.bsSet);
		bsFixed = sol.bsFixed;
	};
	~Solution(){};

	//pravi id bs
	void insertBs(int id, int scId)
	{

		this->bsSet.push_back(make_pair(id, scId));
		originalBaseStations[id].scId = scId;
		//brisanje iz skupa slobodnih bs-a
		for (int i = 0; i < currentBaseStations.size(); i++)
		if (currentBaseStations[i] == id)
		{
			currentBaseStations.erase(currentBaseStations.begin() + i);
			break;
		}
	};

	// indeks u bsSet - u
	void removeBs(int id)
	{
		originalBaseStations[this->bsSet[id].first].scId = -1;
		currentBaseStations.push_back(this->bsSet[id].first);
		this->bsSet.erase(this->bsSet.begin() + id);
	}

	// id je redni broj sc-a u currentSwitchingCenters
	void insertSc(int id)
	{
		this->scSet.push_back(currentSwitchingCenters[id]);
		currentSwitchingCenters.erase(currentSwitchingCenters.begin() + id);
	};

	void removeSc(int id)
	{
		int id1 = this->scSet[id];
		switchingCenters[id1].capacity = scCapacity;
		this->scSet.erase(this->scSet.begin() + id);
		currentSwitchingCenters.push_back(id1);
	};
};


Solution solution;
Solution oldSolution;
Solution bestSolution;


bool compareBsByUsCount(baseStation& a, baseStation& b)
{
	return a.usSet.size() > b.usSet.size();
}

bool compareBsByUsCoveredCount(baseStation& a, baseStation& b)
{
	return a.usCoveredSet.size() > b.usCoveredSet.size();
}



int scTotalCost()
{
	return solution.scSet.size()*scCost;
}

int bsTotalCost()
{
	return solution.bsSet.size()*bsCost;
}

int connCost()
{
	int cost = 0;
	int x, y;
	for (int i = 0; i < solution.bsSet.size(); i++)
	{
		x = solution.bsSet[i].first;
		y = solution.bsSet[i].second;
		cost += bsScConnCost[x][y];
	}

	return cost;
}

int totalCost()
{
	return scTotalCost() + bsTotalCost() + connCost();
}

void generateInitialScSolutionSet()
{
	for (int i = 0; i < scNewCount; i++)
	{
		solution.scSet.push_back(i + scOldCount);
	}
}

void generateBsMustSet()
{
	int covered = 0;
	int bsId;
	bool flag;
	//ako se prvi put izracunava koje bs-e moraju biti u svakom resenju
	if (solution.bsFixed == -1)
	{
		//korisnici pokriveni samo jednom bs - ta bs mora biti u resenju
		for (int i = 0; i < usCount; i++)
		{
			if (users[i].bsSet.size() == 1)
			{
				bsId = users[i].bsSet[0].first;
				flag = false;
				//provera da li je bs vec u resenju
				for (int j = 0; j < solution.bsSet.size(); j++)
				{
					if (solution.bsSet[j].first == bsId)
						flag = true;
				}
				if (flag)
					continue;
				cout << bsId << " ";
				solution.bsSet.push_back(make_pair(bsId, -1));

				//izbacuje se trenutni bs iz liste trenutnih
				for (int i = 0; i < currentBaseStations.size(); i++)
				if (currentBaseStations[i] == bsId)
				{
					currentBaseStations.erase(currentBaseStations.begin() + i);
					break;
				}
				//povecava se broj bs-a
				covered++;
			}
		}
		solution.bsFixed = covered;
		cout << endl;
	}
}

void greedyConn(int bsId, int i)
{
	bsScConnList.clear();
	for (int i = 0; i < scOldCount; i++)
	{
		bsScConnList.push_back(make_pair(i, bsScConnCost[bsId][i]));
	}
	for (int i = 0; i < solution.scSet.size(); i++)
	{
		bsScConnList.push_back(make_pair(i, bsScConnCost[bsId][solution.scSet[i]]));
	}
	sort(bsScConnList.begin(), bsScConnList.end(), comparePairs);

	uniform_int_distribution<int> distConn(0, 1);
	int dice = (distConn(generator) + distConn(generator)) / 2;
	if (i != -1)
	{
		solution.bsSet[i].second = bsScConnList[dice].first;
		originalBaseStations[bsId].scId = bsScConnList[dice].first;
	}
	else
	{
		solution.bsSet.push_back(make_pair(bsId, bsScConnList[dice].first));
		originalBaseStations[bsId].scId = bsScConnList[dice].first;
	}
}

void generateInitialBsSolutionSet()
{

	//uniform_int_distribution<int> distribution(0, bsNewCount - 1);
	//int dice_roll = distribution(generator);  // generates number in the range 1..6 
	//auto randomBs = bind(distribution, generator);

	int bsId;
	//za fiksirane bs bira se slucajno samo konekcija ka sc
	for (int i = 0; i < solution.bsFixed; i++)
	{
		bsId = solution.bsSet[i].first;
		////FIX ME slucajno povezivanje bs na sc 
		////... moze i greedy, mozda je bolje

		randConnSc = rand() % (scOldCount + solution.scSet.size());
		if (randConnSc >= scOldCount)
		{
			randConnSc -= scOldCount;
			randConnSc = solution.scSet[randConnSc];
		}
		solution.bsSet[i].second = randConnSc;
		originalBaseStations[bsId].scId = randConnSc;

		//greedyConn(bsId,i);
	}

	//za ostale bs, bira se na slucajan nacin polovina koja ulazi u pocetno resenje
	for (int i = 0; i < (bsNewCount - solution.bsFixed) / 2; i++)
	{
		//generisanje slucajnih BS i slucajnih konekcija ka postojecim SC

		//jedan indeks od preostalih bs-ova
		int randBs = rand() % currentBaseStations.size();
		//indeks izabrane bs
		bsId = currentBaseStations[randBs];
		//FIX ME slucajno povezivanje bs na sc 
		//... moze i greedy, mozda je bolje

		randConnSc = rand() % (scOldCount + solution.scSet.size());
		if (randConnSc >= scOldCount)
		{
			randConnSc -= scOldCount;
			randConnSc = solution.scSet[randConnSc];
		}

		solution.insertBs(bsId, randConnSc);

	}
}

bool coverUsers()
{
	int rand;
	bool coverPossible = true;
	int ind, i, firstAvailable, bsInd;

	//kapaciteti
	for (int i = 0; i < switchingCenters.size(); i++)
	{
		switchingCenters[i].capacity = scCapacity;
	}
	for (int i = 0; i < originalBaseStations.size(); i++)
	{
		originalBaseStations[i].capacity = bsCapacity;
	}

	//10 random startova - pokusaja pokrivanja korisnika datim skupom baznih stanica
	for (int n = 0; coverPossible && n < 10; n++)
	{

		uniform_int_distribution<int> distribution(0, usCount - 1);
		rand = distribution(generator);

		for (i = 0; i < usCount; i++)
		{
			ind = (i + rand) % usCount;
			//trazi se prva slobodna bs, pocevsi od najblize,
			//koja zajedno sa njenim odgovarajucim sc-om moze da pokrije trenutnog korisnika
			for (firstAvailable = 0; firstAvailable < users[ind].bsSet.size(); firstAvailable++)
			{
				bsInd = users[ind].bsSet[firstAvailable].first;

				if (originalBaseStations[bsInd].scId == -1)
				{
					//bs nije u resenju
					continue;
				}
				if (originalBaseStations[bsInd].capacity > 0 && switchingCenters[originalBaseStations[bsInd].scId].capacity > 0)
				{
					originalBaseStations[bsInd].capacity--;
					switchingCenters[originalBaseStations[bsInd].scId].capacity--;
					break;
				}

			}

			if (firstAvailable == users[ind].bsSet.size())
			{
				//nije nadjen odgovarajuci bs
				coverPossible = false;
				break;
			}
		}
		//ako su pokriveni svi korisnici
		if (i == usCount)
		{
			return true;
		}
		//inace prelazak na sledecu iteraciju
	}
	return false;
}



void resetCapacities()
{
	for (int i = 0; i < switchingCenters.size(); i++)
	{
		switchingCenters[i].capacity = scCapacity;
	}
	for (int i = 0; i < originalBaseStations.size(); i++)
	{
		originalBaseStations[i].capacity = bsCapacity;
		//stare bs-e vec imaju dodeljene sc-e pa se resetuju samo nove bs-e
		if (i >= bsOldCount)
			originalBaseStations[i].scId = -1;
	}
}

void resetCurrentBS()
{
	currentBaseStations.clear();
	for (int i = bsOldCount; i < bsOldCount + bsNewCount; i++)
	{
		currentBaseStations.push_back(i);
	}
	//brisu se obavezne bs-e
	if (solution.bsFixed != -1)
	for (int i = 0; i < solution.bsFixed; i++)
	{
		int j = solution.bsSet[i].first;
		for (int k = 0; k < currentBaseStations.size(); k++)
		if (currentBaseStations[k] == j)
		{
			currentBaseStations.erase(currentBaseStations.begin() + k);
			break;
		}
	}
}

void resetSolutionBs()
{
	if (solution.bsFixed != -1)
	{
		for (int i = solution.bsSet.size() - 1; i >= solution.bsFixed; i--)
		{
			originalBaseStations[solution.bsSet[i].first].scId = -1;
			solution.bsSet.erase(solution.bsSet.begin() + i);

		}
	}
	else
	{
		//solution.bsSet.clear();
	}
}

void resetSolutionSc()
{
	solution.scSet.clear();
}

void resetSolution()
{
	resetCapacities();
	resetCurrentBS();
	resetSolutionBs();
	resetSolutionSc();
}

void generateInitialSolution()
{
	int i = 0;
	generateBsMustSet();
	do
	{
		resetSolution();
		generateInitialScSolutionSet();
		generateInitialBsSolutionSet();
		i++;
	} while (!coverUsers());
	previousCost = totalCost();
	//cout << "initial solution generated after " << i << " attempts" << endl;
	//output << "initial solution generated after "<<i<<" attempts" << endl;

}

void initialize()
{
	srand(time(NULL));
	int currentIter = 0;
	//solution.bsFixed = -1;

	currentBsCount = bsNewCount + bsOldCount;
	noImprovementCount = 0;
	//inicijalizacija skupova bs i us
	switchingCenters.clear();
	switchingCenters.resize(scOldCount + scNewCount);
	baseStations.clear();
	baseStations.resize(currentBsCount);
	users.clear();
	users.resize(usCount);

	currentSwitchingCenters.clear();

	for (int i = 0; i < usCount; i++)
	{
		users[i].id = i;
	}


	for (int j = 0; j < currentBsCount; j++)
	{
		//cout << j << ": ";
		baseStations[j].id = j;
		baseStations[j].capacity = bsCapacity;
		for (int i = 0; i < usCount; i++)
		{
			if (usBsRadius[i][j] < bsRadius)
			{
				//cout << i <<" ";
				baseStations[j].usSet.insert(i);
				users[i].bsSet.push_back(make_pair(j, usBsRadius[i][j]));
			}
		}

		//cout << baseStations[j].usSet.size() << endl;

	}

	//sortiranje po broju baznih stanica
	for (int i = 0; i < usCount; i++)
	{
		sort(users[i].bsSet.begin(), users[i].bsSet.end(), comparePairs);
	}

	//init kapaciteta sc-a
	for (int i = 0; i < scOldCount + scNewCount; i++)
	{
		switchingCenters[i].capacity = scCapacity;
	}

	//ispis DEBUG
	//printCurrentBs();
	originalBaseStations.clear();
	originalBaseStations = deque<baseStation>(baseStations);

	//punjenje skupa bs na raspolaganju
	resetCurrentBS();
	//stare veze bs - sc
	for (int i = 0; i < bsOldCount; i++)
	{
		for (int j = 0; j < scOldCount; j++)
		{
			if (bsScConnMatrix[i][j] == 1)
			{
				originalBaseStations[i].scId = j;
			}
		}
	}
	solution.bsFixed = -1;
	solution.bsSet.clear();
	solution.scSet.clear();
	//DEBUG
	//printCurrentBs();

}

void readInput()
{
	fstream fin;
	fin.open(input, ifstream::in);

	fin >> scOldCount >> scNewCount >> bsOldCount >> bsNewCount >> usCount;
	fin >> scCost >> bsCost >> scCapacity >> bsCapacity;
	fin >> bsRadius;
	for (int i = 0; i < bsOldCount; i++)
	for (int j = 0; j < scOldCount; j++)
	{
		fin >> bsScConnMatrix[i][j];
	}

	for (int i = 0; i < bsNewCount; i++)
	for (int j = 0; j < scOldCount + scNewCount; j++)
	{
		fin >> bsScConnCost[i][j];
	}

	for (int i = 0; i < usCount; i++)
	{

		for (int j = 0; j < bsNewCount + bsOldCount; j++)
		{
			fin >> usBsRadius[i][j];
		}
	}

	fin.close();

}



//uklanjanje jednog slucajnog sc-a
void localSearchScRemove()
{
	if (solution.scSet.size() <= 1)
		return;
	oldSolution = Solution(solution);

	//uniform_int_distribution<int> distConn(0, solution.scSet.size() - 1);
	int randSc = rand() % solution.scSet.size();
	//int randSc = distConn(generator);
	currentSwitchingCenters.push_back(solution.scSet[randSc]);
	solution.scSet.erase(solution.scSet.begin() + randSc);
	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{
		resetCapacities();
		resetCurrentBS();
		resetSolutionBs();
		generateInitialBsSolutionSet();
		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;
}

//uklanjanje jedne slucajne bs-e i postavljanje nove
void localSearchBsInvert()
{
	cout << "ls bs invert: ";
	oldSolution = Solution(solution);

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{

		int x = solution.bsSet.size() - solution.bsFixed;
		randBs = rand() % x + solution.bsFixed;
		//randBs = solution.bsSet[randBs].first;
		cout << solution.bsSet[randBs].first << " ";

		solution.removeBs(randBs);

		randBs = rand() % currentBaseStations.size();
		randBs = currentBaseStations[randBs];
		randConnSc = rand() % (scOldCount + solution.scSet.size());

		cout << randBs << endl;

		if (randConnSc >= scOldCount)
		{
			randConnSc -= scOldCount;
			randConnSc = solution.scSet[randConnSc];
		}

		solution.insertBs(randBs, randConnSc);

		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;

}

//uklanjanje jedne slucajne bs-e
void localSearchBsRemove()
{
	//if (solution.bsSet.size() == solution.bsFixed)
	return;
	oldSolution = Solution(solution);

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{
		//uklanjanje jedne slucajne bs-e i postavljanje nove
		int x = solution.bsSet.size() - solution.bsFixed;
		if (x == 0)
			return;
		int randBs = rand() % x + solution.bsFixed;

		solution.removeBs(randBs);

		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;

}


void acceptanceCriterion()
{
	currentCost = totalCost();
	if (currentCost < previousCost)
	{
		previousCost = currentCost;
		noImprovementCount = 0;
		bestSolution = Solution(solution);
		//cout << "bolje "<< endl;
	}
	else
	{
		noImprovementCount++;
	}
}

//bs zamena
void perturbationBsConnInvert()
{
	//uniform_int_distribution<int> distConn(0, solution.bsSet.size() - 1);
	int x = solution.bsSet.size() - solution.bsFixed;
	int randBs1 = rand() % x + solution.bsFixed;

	//int randBs1 = rand() % solution.bsSet.size();
	//int randBs1 = distConn(generator);
	int randBs2 = rand() % x + solution.bsFixed;
	//int randBs2 = distConn(generator);
	while (randBs1 == randBs2)
	{
		randBs2 = rand() % x + solution.bsFixed;
	}
	int temp = solution.bsSet[randBs1].first;
	solution.bsSet[randBs1].first = solution.bsSet[randBs2].first;
	solution.bsSet[randBs2].first = temp;
}

//sc zamena
void perturbationScInvert()
{
	//oldSolution = Solution(solution);
	//uklanjanje jednog slucajnog sc-a
	int randSc1 = rand() % (solution.scSet.size() - scOldCount);
	solution.removeSc(randSc1);
	//i vracanje sc-a
	int id;
	int randSc2 = rand() % currentSwitchingCenters.size();
	id = currentSwitchingCenters[randSc2];
	solution.insertSc(randSc2);

	for (int i = 0; i < solution.bsSet.size(); i++)
	{
		if (solution.bsSet[i].second == randSc1)
			solution.bsSet[i].second = id;
	}

}

void runILS()
{
	generateInitialSolution();


	currentIter = 0;
	while (currentIter++ < MAX_ITER && noImprovementCount < 100)
	{
		//if (noImprovementCount/10 % 2)
		perturbationBsConnInvert();

		//	perturbationScInvert();
		//localSearchScRemove();
		//localSearchBsInvert();
		//if (noImprovementCount / 20 % 2 == 0)
		localSearchBsInvert();
		//else
		localSearchBsRemove();
		acceptanceCriterion();
	}
}


void printResult()
{
	//cout << "total cost = " << totalCost() << ";\tminCost = " << previousCost << ";\titer = " << currentIter << ";\tnoImprovement = " << noImprovementCount << endl;
	cout << "sc: ";
	for (int i = 0; i < bestSolution.scSet.size(); i++)
	{
		cout << bestSolution.scSet[i] << " ";
	}
	cout << endl;
	cout << "bs: ";
	for (int i = 0; i < bestSolution.bsSet.size(); i++)
	{
		cout << bestSolution.bsSet[i].first << "->" << bestSolution.bsSet[i].second << " ";
	}
	cout << endl << previousCost << endl;
	output << previousCost << endl;
}

int main()
{

	output.open("log.txt");
	for (int i = 1; i <= 1; i++)
	{
		ostringstream ostr;
		ostr << "instance/instSmall/instSmall" << i << ".txt";

		input = ostr.str();
		readInput();

		initialize();

		//generateInitialSolution();
		runILS();
		//cout << i << endl;
		printResult();
	}
	//cin >> currentIter;
	return 0;
}