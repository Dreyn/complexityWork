#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

//###########################
// List Scheduling Algorithm

Solution LSA(Instance instance) //instance pass by reference
{
	Solution solution(instance);
	
	//references for simplier writing
	int nbMachines = instance.nbMachines;
	vector<int> & tasks = instance.tasks;
	
	vector<int> & assign = solution.assign;	
	vector<vector<int> > & schedule = solution.schedule;
	vector<int> & machineCharge = solution.machinesCharge;
	int & finalTime = solution.finalTime;
	
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
		
		for(int i=0; i < nbMachines ; ++i){ //looking for the least charged machine
			if(machineCharge[i] < minimalCharge)
				minimalCharge = machineCharge[i];
		}
	}
	
	finalTime=0;
	for(int i=0; i<machineCharge.size(); ++i){ //looking for the most charged machine
		if(machineCharge[i]>finalTime)
			finalTime = machineCharge[i];
	}
	
	return solution;
}


//###################################
// Largest Processing Time algorithm

Solution LPT(Instance instance)
{
	sort (instance.tasks.begin(), instance.tasks.end(), greater<int>());
	
	Solution solution = LSA(instance);
	
	return solution;
}


//######################
// Our custom algorithm

Solution MyAlgo(Instance instance)
{
	Solution sol = LPT(instance);
	vector<vector<int> > & sched = sol.schedule; //contains the num of the tasks for each machine
	vector<int> & tasks = instance.tasks; //contains the durations of the tasks
	
	bool found;
	do{
		// Getting machines sorted by their charge
		vector<int> sortedMachines = sol.machinesCharge;
		vector<int> machinesOrder = vector<int>(instance.nbMachines);
		
		sort(sortedMachines.begin(), sortedMachines.end(), greater<int>());
		for(int i=0; i<instance.nbMachines; ++i)
		{
			for(int j=0; j<instance.nbMachines; ++j)
			{
				if(sortedMachines[i]==sol.machinesCharge[j]){
					machinesOrder[i] = j;
					sol.machinesCharge[j] *= -1; //so that it won't be seen again
					break;
				}
			}
		}
		for(int i=0; i<instance.nbMachines; ++i)
			sol.machinesCharge[i] *= -1;
		
		// Trying to find 2 tasks to permute
		found = false;
		
		int a, b; //num of re-sorted machines
		int t1, t2; //num for tasks
		int taskLong, taskShort; //duration of 2 selected tasks
		int taskLongInd, taskShortInd; //their num in their respective machines 
		int machineShort; //machine of the smaller selected task
		int taskLongNum, taskShortNum; //their num in the original list
		int delta; //half the difference between the two watched machines
		
		taskLong = 0;
		taskShort = 0;
		taskLongInd = -1;
		taskShortInd = -1;
		taskLongNum = -1;
		taskShortNum = -1;
		machineShort = -1;
		a = machinesOrder[0];
		for(int k=instance.nbMachines - 1; k>0; --k)
		{
			b = machinesOrder[k];
			
			delta = sol.machinesCharge[a] - sol.machinesCharge[b];
			delta /= 2; //truncated toward zero
			
			if(delta <= taskLong - taskShort)
				break; //there will not be better tasks to be found
			
			for(int i=0; i<sched[a].size(); ++i)
			{
				t1 = sched[a][i];
				for(int j=sched[b].size()-1; j>=0; --j)
				{
					t2 = sched[b][j];
					if(tasks[t1] > tasks[t2])
					{
						if(tasks[t1] - tasks[t2] <= delta)
						{
							if(tasks[t1] - tasks[t2] > taskLong - taskShort){
								found = true;
								
								taskLong = tasks[t1];
								taskLongInd = i; //num in the machine
								taskLongNum = t1;
								
								taskShort = tasks[t2];
								taskShortInd = j;
								taskShortNum = t2;
								machineShort = b;
							}
							
							break; //we're only interested in the biggest smaller than delta
						}
					}
					else
						break; //might be ugly, but for loops are more friendly than while loops
				}
			}
		}
		
		if(found){
			b = machineShort; //shorter to write
			
			//updating assign (even it is actually never used, it is at least simple to update...)
			sol.assign[taskLongNum] = b;
			sol.assign[taskShortNum] = a;
			
			//updating sched[a]
			sched[a].erase( sched[a].begin()+taskLongInd );
			t1=0;
			while(t1 < sched[a].size() and tasks[sched[a][t1]] > taskShort){ //to find where to insert taskShort
				t1++;
			}
			sched[a].insert( sched[a].begin()+t1, taskShortNum);
			
			//updating sched[b]
			sched[b].erase( sched[b].begin()+taskShortInd );
			t2=0;
			while(t2 < sched[b].size() and tasks[sched[b][t2]] > taskLong){
				t2++;
			}
			sched[b].insert( sched[b].begin()+t2, taskLongNum);
			
			//updating machinesCharge
			sol.machinesCharge[a] -= taskLong - taskShort;
			sol.machinesCharge[b] += taskLong - taskShort;
			
			//updating finalTime
			sol.finalTime=0;
			for(int i=0; i<sol.machinesCharge.size(); ++i){
				if(sol.machinesCharge[i]>sol.finalTime)
					sol.finalTime = sol.machinesCharge[i];
			}
		}
	}while(found);
	
	return sol;
}
