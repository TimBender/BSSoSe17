#include"SimCPU.h"
#include<iostream>
#include<array>
#include<list>
#include<random>
#include<set>
#include<algorithm>


void simulateCPU(Process& process,const int &cmd);

using namespace std;
int main(int argc, char** argv)
{
	set<Process> processes;		// stores all processes
	SimCPU sim_CPU(processes);	//	simulated CPU
	array<char, 1000> ram;		//	1 char = 1B
	list<Page> hard_disk;		//	Festplatte: speichert min. alle - nicht eingelagerten - Seiten
	

	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());		

	int randNum;
	
	Process current_process;

	/** am wahrscheinlichsten (desc):
							0 = lesen
							1 = schreiben
							2 = Prozesswechsel */

	discrete_distribution<> stochastic_distribution({ 3, 2, 1 }); 
	for (size_t i = 0; i < 10; i++)
	{

		randNum = stochastic_distribution(gen);

		/*simulate CPU*/
		sim_CPU.execute(current_process,randNum);
	}

	/*Ausgabe der Statistik*/
	sim_CPU.printReport();


	return 0;
}

