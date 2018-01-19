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
	double sumPerMachine;
	
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
		
		sumPerMachine = double(sumOfTask)/double(nbMach);
	}
	
	void display(){
		
		cout << endl << "Displaying instance," << endl;
		cout << "Number of machines : " << nbMachines << endl;
		for(int i=0; i<tasks.size(); ++i){
			cout << tasks[i] << "\t";
		}
		cout << endl << "Longest task : " << longestTask << endl;
		cout << "Sum of tasks divided by machines : " << sumPerMachine << endl;
	}
};


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
		cout << endl << "Final time = " << finalTime << endl;
	}
		
};
