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
	
	string s;
	int n;
	
	iss >> nbMach; // >> operator for istringstream : "Extract formatted input"
	
	while(iss.tellg() >= 0) //iss.tellg() : "Get position in input sequence"
	{
		iss >> s;
		iss >> n;
		
		tasks.push_back(n);
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
	sort (instance.tasks.begin(), instance.tasks.end(), greater<int>());
	
	Solution solution = LSA(instance);;
	
	return solution;
}

//##################################
// Interface utilisateur interactif

void generateFromFile(string file); //forward declaration
void generateFromInput();
void generateFromRandom();
//procédure suivante ?
void resultDisplay(Solution solution);

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
					void generateFromRandom();
					
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
	
	cout << "Veuillez entrer une chaine de la forme \"m : t1 : t2 : ... : tn\"," << endl;
	cout << " où m est le numéro de la machine et t1, t2... tn sont les poids de vos n tâches." << endl;
	cout << " Ne mettez pas de guillemets, mettez un séparateur et des espaces." << endl;
	cout << " Et ne vous gourrez pas, on ne va pas vérifier derrière vous !" << endl;
	
	getline(cin, input);
	
	Instance inst = instanceCreation(input);
	
	//procédure suivante...		
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
		
		//procédure suivante...
	}
	else cout << "Unable to open file"; 
	
}

void generateFromRandom()
{
	cout << "Génération aléatoire à implémenter..." << endl;
}


void resultDisplay(Solution solution)
{
	
}

//#############
// Code client

int main(int argc, char* argv[]){
	// initialize random seed (not useful yet)
	srand (time(NULL));
	
	cout << "Bonjour," << endl;
	cout << "Ce programme consiste à résoudre une instance donnée." << endl;
	
	string input = "3 : 2 : 7 : 1 : 3 : 2 : 6 : 2 : 3 : 6 : 2 : 5";
	
	Instance inst = instanceCreation(input);
	
	//LSA Test
	Solution sol = LSA(inst);
	
	sol.display();
	
	//LPT Test
	Solution sol2 = LPT(inst);
	
	sol2.display();
	
	
	return 0;
}
