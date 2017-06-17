#include"Process.h"
#include"MMU.h"
#include"SimulatedCPU.h"
#include<vector>
#include<random>
#include<ctime>

using namespace std;

int main(int argc, char** argv)
{

	srand(static_cast<unsigned int>(time(NULL)));

	const size_t NUM_OF_PROCESS = 4;

	vector<Process> processes;



	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());

	/*Pick a random adress at first*/
	int rand_command;

	/** generate Processes for testing purposes	
	===========================================	*/
	for (size_t i = 0; i < NUM_OF_PROCESS; i++){
		processes.push_back(Process());
	}

	SimulatedCPU& sim_cpu= SimulatedCPU::getInstance();	//	CPU
	sim_cpu.setProcesses(processes);

	/**	CPU picks a command	
	===========================================	
	probability (desc):
	0 = read
	1 = write
	2 = process switch */

	discrete_distribution<> stochastic_distribution({ 3, 2, 1 });
	for (size_t i = 0; i < 20; i++)
	{
		rand_command = stochastic_distribution(gen);
		sim_cpu.execute(rand_command);		// default: execute first process;
	}

	/*	print statistics	*/
	sim_cpu.printReport();

	return 0;
}