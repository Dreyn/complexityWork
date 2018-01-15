#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

struct Instance
{
	int nbMachines;
	vector<int> tasks;
	
	int longestTask;
	int sumPerMachine;
	
	Instance(){} //constructeur à vide
	
	Instance(int nbMach, vector<int> task): nbMachines(nbMach), tasks(task)
	{
		longestTask = 0;
		
		int t;
		int sumOfTask = 0;
		
		for(int i=0; i<tasks.size(); ++i)
		{
			t = tasks[i];
			if(t>longestTask)
				longestTask = t;
			sumOfTask += t;
		}
		
		sumPerMachine = sumOfTask/nbMach;
		
	}
};
