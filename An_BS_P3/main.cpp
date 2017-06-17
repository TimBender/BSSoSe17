#include"SimCPU.h"
#include<iostream>
#include<array>
#include<list>
#include<random>
#include<set>
#include<algorithm>
#include<ctime>
#include<map>

bitset<32> adress_generator_rand(const Process& current_process, bitset<32>&  current_adress);
bitset<32> adress_generator_delta(const Process& current_process, bitset<32>&  current_adress);

using namespace std;
int main(int argc, char** argv)
{  
	srand(static_cast<unsigned int>(time(NULL)));
	const int NUMBER_OF_PROCESSES = 4;	
	bitset<32> adress_picker;

	set<Process> processes;		//	stores all processes
	Process current_process;


	array<Page, 250> ram;						//	1KB= 250Pages: 1KB / 4B (Groesse einer Seite)
	multimap< size_t , Page > hard_disk;		//	speichert min. alle - nicht eingelagerten - Seiten + ID des Prozesses fuer die Zuordnung
	
	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());

	/*Fuer Testzwecke: erstelle 4 Prozesse*/
	for (size_t i = 0; i < NUMBER_OF_PROCESSES; i++)
	{
		processes.insert(Process());
	}

	
	SimCPU sim_CPU(processes);	//	simulated CPU

	/*	ADRESSGENERATOR Funktionen:
		1) adress_generator_rand: mit einer gewissen Wahrscheinlichkeit die letzte Adresse wiederverwenden (und sonst zu einer neuen, zufälliger Adresse springen
		2) adress_generator_delta: Adress + 'Adressdelta'
	=======================================================================	*/
	adress_picker = adress_generator_rand(current_process, adress_picker);


	/*Pick a random adress at first*/
	int randNum;
	


	/** am wahrscheinlichsten (desc):
							0 = lesen
							1 = schreiben
							2 = Prozesswechsel */

	discrete_distribution<> stochastic_distribution({ 3, 2, 1 }); 
	for (size_t i = 0; i < 10; i++)
	{

		randNum = stochastic_distribution(gen);

		/*simulate CPU*/
		sim_CPU.execute(randNum);
	}

	/*Ausgabe der Statistik*/
	sim_CPU.printReport();


	return 0;
}

bitset<32> adress_generator_rand(const Process& current_process, bitset<32>&  current_adress)
{
	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());

	discrete_distribution<> generateAdress({ 1, 2 });

	int i = generateAdress(gen);
	if (i == 1){
		return current_process.getPages().at(rand() % current_process.getPages().size()).getBlock();
	}
	else return current_adress;
}

bitset<32> adress_generator_delta(const Process& current_process, bitset<32>&  current_adress)
{
	return bitset<32>(current_adress.to_ullong() + (bitset<32>(268435464)).to_ullong());
}