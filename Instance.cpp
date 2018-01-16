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
