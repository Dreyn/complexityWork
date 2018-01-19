#include <iostream> //cout
#include <fstream> //file reading
#include <sstream> //istringstream
#include <string>
#include <vector>
#include <time.h>

#include <bits/stdc++.h> //sort

#include "Instance.cpp"
#include "Solution.cpp"

using namespace std;

//#################################
// Méthodes d'entrée des Instances

Instance instanceCreation(string input)
{
	istringstream iss(input); //will enable to extract words (or numbers) separated by spaces
	
	vector<int> tasks;
	int nbMach;
	int nbTasks;
	
	string s;
	int t;
	
	iss >> nbMach; // >> operator for istringstream : "Extract formatted input"
	iss >> s;
	iss >> nbTasks; //ne servira à rien \o/
	
	while(iss.tellg() >= 0) //iss.tellg() : "Get position in input sequence"
	{
		iss >> s;
		iss >> t;
		
		tasks.push_back(t);
	}
	
	return Instance(nbMach, tasks);
}

//###########################
// Algorithmes de résolution

//List Scheduling Algorithm
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
		
		for(int i=0; i < nbMachines ; ++i){ //
			if(machineCharge[i] < minimalCharge)
				minimalCharge = machineCharge[i];
		}
	}
	
	finalTime=0;
	for(int i=0; i<machineCharge.size(); ++i){
		if(machineCharge[i]>finalTime)
			finalTime = machineCharge[i];
	}
	
	return solution;
}

//Largest Processing Time algorithm
Solution LPT(Instance instance)
{
	sort (instance.tasks.begin(), instance.tasks.end(), greater<int>());
	
	Solution solution = LSA(instance);
	
	return solution;
}

// Our custom algorithm
Solution MyAlgo(Instance instance)
{
	Solution sol = LPT(instance);
	vector<vector<int> > & sched = sol.schedule; //contains the indices of the tasks for each machine
	vector<int> & tasks = instance.tasks; //contains the durations of the tasks
	
	bool found = false;
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
					sol.machinesCharge[j] *= -1;
					break;
				}
			}
		}
		for(int i=0; i<instance.nbMachines; ++i)
			sol.machinesCharge[i] *= -1;
		
		// Trying to find 2 tasks to permute
		found = false;
		
		int a, b; //indices of re-sorted machines
		int t1, t2; //indices for tasks
		int taskLong, taskShort, taskLongInd, taskShortInd, machineShort, taskLongNum, taskShortNum;
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
								taskLongInd = i;
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
			b = machineShort;
			
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

//##################################
// Interface utilisateur interactif

void generateFromFile(string file); //forward declaration
void generateFromInput();
void generateFromRandom();
void singleInstance(Instance inst);

void userInterface(int argc, char* argv[])
{
	int numChoice;
	string textChoice;
	char alphaChoice;
	
	string input;
	
	if(argc == 1)
	{
		
		
		cout << "Veuillez choisir comment générer votre instance :" << endl;
		cout << "1: Entrer l'instance au clavier," << endl;
		cout << "2: Entrer le nom d'un fichier contenant une instance," << endl;
		cout << "3: Générer aléatoirement une instance." << endl;
		
		do{
			cin >> numChoice;
			switch(numChoice){
				case 1 :
					generateFromInput();
					
					break;
				case 2 :
					cout << "Veuillez entrer le nom du fichier à ouvrir :" << endl;
					cin >> input;
					
					generateFromFile(input);
					
					break;
				case 3 :
					generateFromRandom();
					
					break;
				default :
					cout << "Veuillez choisir un chiffre entre 1 et 3, ou faire Ctrl+C pour quitter ce programme." << endl;
					
					numChoice = 4; //pour avoir numChoice>3 même si l'utilisateur a choisi un nombre négatif...
			}
			
		}while( numChoice > 3 );
	}
	else
	{
		cout << "Détection d'un paramètre de lancement de programme," << endl;
		cout << " traitement..." << endl;
		
		input = argv[0];
		
		generateFromFile(input);
		
	}
}

void generateFromInput()
{
	string input;
	
	cout << "Veuillez entrer une chaine de la forme \"m : n : t1 : t2 : ... : tn\"," << endl;
	cout << " où m est le numéro de la machine, n le nombre de tâches et t1, t2... tn sont les poids de vos n tâches." << endl;
	cout << " Ne mettez pas de guillemets, mettez un séparateur et des espaces." << endl;
	cout << " Et ne vous gourrez pas, on ne va pas vérifier derrière vous !" << endl;
	
	getline(cin,input); //la première fois ne fonctionne pas... WTF!?
	getline(cin,input); 
	
	Instance inst = instanceCreation(input);
	
	singleInstance(inst);
}

void generateFromFile(string file)
{
	string line;
	
	ifstream myfile(file.c_str());
	if (myfile.is_open())
	{
		getline(myfile,line);
		
		myfile.close();
		
		Instance inst = instanceCreation(line);
		
		cout << "Une instance a été créé." << endl;
		
		singleInstance(inst);
	}
	else cout << "Unable to open file"; 
	
}

void singleInstance(Instance instance)
{
	Solution solLSA, solLPT, solMyAlgo;
	
	solLSA = LSA(instance);
	solLPT = LPT(instance);
	solMyAlgo = MyAlgo(instance);
	
	cout << endl << "Résultat des algorithmes :" << endl << endl;
	
	cout << "Borne inférieure \"maximale\" = " << instance.longestTask << endl;
	cout << "Borne inférieure \"moyenne\"  = " << instance.sumPerMachine << endl;
	cout << "Résultat LSA = " << solLSA.finalTime << endl;
	cout << "Résultat LPT = " << solLPT.finalTime << endl;
	cout << "Résultat MyAlgo = " << solMyAlgo.finalTime << endl;
	
}


void generateFromRandom()
{
	//############################
	// Configurating generation :
	
	int k, m, n, min, max;
	
	cout << "Vous allez saisir le nombre d'instance à créer, le nombre de machines et de tâches par instance, et les bornes minimales et maximales des durées des tâches." << endl;
	
	cout << "Nombre d'instance k = ";
	cin >> k;
	cout << "Nombre de machines m = ";
	cin >> m;
	cout << "Nombre de tâches t = ";
	cin >> n;
	cout << "Durée minimale des tâches : min = ";
	cin >> min;
	cout << "Durée maximale des tâches : max = ";
	cin >> max;
	
	//########################
	// Generating instances :
	
	vector< vector<int> > taskLists = vector< vector<int> >(k, vector<int>(n));
	vector<Instance> instances = vector<Instance>(k);
	
	for(int i=0; i<k; ++i)
	{
		for(int j=0; j<n; ++j)
		{
			taskLists[i][j] = (rand()%max)+min;
		}
		instances[i] = Instance(m, taskLists[i]);
	}
	
	//########################
	// Generating solutions :
	
	vector<Solution> solLSA = vector<Solution>(k);
	vector<Solution> solLPT = vector<Solution>(k);
	vector<Solution> solMyAlgo = vector<Solution>(k);
	
	double timeLSA, timeLPT, timeMyAlgo;
	double longestTask, sumPerMach, minimalBound;
	
	double sumOfRatioLSA = 0;
	double sumOfRatioLPT = 0;
	double sumOfRatioMyAlgo = 0;
	double avgOfRatioLSA, avgOfRatioLPT, avgOfRatioMyAlgo;
	
	for(int i=0; i<k; ++i)
	{
		solLSA[i] = LSA(instances[i]);
		solLPT[i] = LPT(instances[i]);
		solMyAlgo[i] = MyAlgo(instances[i]);
		
		timeLSA = (double)solLSA[i].finalTime;
		timeLPT = (double)solLPT[i].finalTime;
		timeMyAlgo = (double)solMyAlgo[i].finalTime;
		
		longestTask = (double)instances[i].longestTask;
		sumPerMach = (double)instances[i].sumPerMachine;
		minimalBound = (longestTask > sumPerMach) ? longestTask : sumPerMach;
		
		sumOfRatioLSA += timeLSA/minimalBound;
		sumOfRatioLPT += timeLPT/minimalBound;
		sumOfRatioMyAlgo += timeMyAlgo/minimalBound;
	}
	
	avgOfRatioLSA = sumOfRatioLSA/k;
	avgOfRatioLPT = sumOfRatioLPT/k;
	avgOfRatioMyAlgo = sumOfRatioMyAlgo/k;
	
	//#######################
	// Generating the file :
	
	string fileName;
	
	cout << endl << "Les instances ont été générées et traitées." << endl;
	cout << "Les résultats seront mis dans un fichier." << endl;
	cout << "Entrez le nom du fichier à créer :" << endl;
	
	cin >> fileName;
	
	ofstream outfile(fileName.c_str());
	
	outfile << k << " instances à " << m << " machines et " << n << " tâches.\n\n";
	
	for(int i=0; i<k; ++i)
	{
		outfile << "Tâches";
		for(int j=0; j<n; ++j){
			outfile << " : " << instances[i].tasks[j];
		}
		outfile << endl;
		
		outfile << "Borne inférieure \"maximale\" = " << instances[i].longestTask << endl;
		outfile << "Borne inférieure \"moyenne\"  = " << instances[i].sumPerMachine << endl;
		outfile << "Résultat LSA = " << solLSA[i].finalTime << endl;
		outfile << "Résultat LPT = " << solLPT[i].finalTime << endl;
		outfile << "Résultat MyAlgo = " << solMyAlgo[i].finalTime << endl;
		
		outfile << "=====================================" << endl;
	}
	
	outfile << "Ratio d'approximation moyen de LSA = " << avgOfRatioLSA << endl;
	outfile << "Ratio d'approximation moyen de LPT = " << avgOfRatioLPT << endl;
	outfile << "Ratio d'approximation moyen de MyAlgo = " << avgOfRatioMyAlgo << endl;
	
	outfile.close();
}


//#############
// Code client

int main(int argc, char* argv[]){
	// initialize random seed (not useful yet)
	srand (time(NULL));
	
	cout << "Bonjour," << endl;
	cout << "Ce programme consiste à résoudre une instance donnée." << endl;
	
	userInterface(argc, argv);
	
	return 0;
}
