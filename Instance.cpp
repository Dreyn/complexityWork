#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

struct Instance
{
	int nbMachines;
	vector<int> tasks;
	
	Instance(int nbMach, vector<int> task): nbMachines(nbMach), tasks(task)
	{}
};
