#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include <bits/stdc++.h> //sort

#include "Instance.cpp"
#include "Solution.cpp"

using namespace std;

//#################################
// Méthodes d'entrée des Instances



//###########################
// Algorithmes de résolution

//List Scheduling Algorithm
Solution LSA(Instance instance)
{
	Solution solution(instance);
	
	//references for simplier writing
	int & nbMachines = instance.nbMachines;
	vector<int> & tasks = instance.tasks;
	
	vector<int> & assign = solution.assign;	
	vector<vector<int> > & schedule = solution.schedule;
	vector<int> & machineCharge = solution.machinesCharge;
	
	//beggining
	int minimalCharge = 0;
	for(int t=0; t < tasks.size(); ++t){ //for each task
		
		for(int i=0; i < nbMachines ; ++i){ //look for the least charged machine
			
			if(machineCharge[i] == minimalCharge){
				assign[t] = i;
				schedule[i].push_back(t); //add t in schedule[i]
				machineCharge[i] += tasks[t];
				
				minimalCharge = machineCharge[i];
				break;
			}
			
		}
		
		for(int i=0; i < nbMachines ; ++i){ //
			if(machineCharge[i] < minimalCharge)
				minimalCharge = machineCharge[i];
		}
	}
	
	return solution;
}

//Largest Processing Time algorithm
Solution LPT(Instance instance)
{
	std::sort (instance.tasks.begin(), instance.tasks.end(), greater<int>());
	
	Solution solution = LSA(instance);;
	
	return solution;
}

//################################
// Code client

int main(){
	// initialize random seed (not useful yet)
	srand (time(NULL));
	
	// initializing an instance
	int tasksList[] = {2,7,1,3,2,6,2,3,6,2,5};
	vector<int> tasks(tasksList, tasksList+11);
	int nbMach=3;
	
	Instance inst(nbMach, tasks);
	
	//LSA Test
	Solution sol = LSA(inst);
	
	sol.display();
	
	//LPT Test
	Solution sol2 = LPT(inst);
	
	sol2.display();
	
	
	return 0;
}
