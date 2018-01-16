#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

struct Solution
{
	vector<int> tasksWeight;
	vector<int> assign; //assign[t]: machine's assignment of the task t
	
	vector<vector<int> > schedule; //schedule[i][j]: jth task of the machine i
	vector<int> machinesCharge;
	int finalTime;
	
	Solution(){} //constructeur à vide
	
	Solution(Instance instance): tasksWeight(instance.tasks),
								   assign(instance.tasks.size()),
								   schedule(instance.nbMachines, vector<int>()),
								   machinesCharge(instance.nbMachines, 0)
	{}
	
	void display(){
		
		cout << endl << "Displaying schedule (task: weight)" << endl;
		for(int i=0; i<schedule.size(); ++i){
			cout << "M" << i << "\t";
			for(int t=0; t<schedule[i].size(); ++t){
				
				cout << schedule[i][t] << ": " << tasksWeight[schedule[i][t]] << "\t";
				
			}
			cout << endl;
		}
		
	}
		
};
