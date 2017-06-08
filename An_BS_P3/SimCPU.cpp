#include "SimCPU.h"

size_t SimCPU::READING_COUNTER = 0, SimCPU::WRITING_COUNTER = 0, SimCPU::PROCESS_SWITCH_COUNTER = 0;
SimCPU::SimCPU(set<Process>& processes)
:m_processes(processes)
{
}

/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void SimCPU::execute(Process& process, const int &cmd)
{
	enum INSTRUCTION{
		READ, WRITE, SWITCH_PROCESS
	};

	Process temp_process;

	switch (INSTRUCTION(cmd))
	{
	case READ:
		cout << "---\tREADING OPERATION\t---\n";
		cout << "___________________________________\n\n";
		READING_COUNTER++;
		break;

	case WRITE:
		cout << "---\tWRITING OPERATION\t---\n";
		cout << "___________________________________\n\n";
		WRITING_COUNTER++;
		break;

	case SWITCH_PROCESS:	//	willk�rlicher Prozesswechsel
		cout << "---\tPROCESS SWITCH\t---\n";
		cout << "___________________________________\n\n";

		/*	swap current process with random process in the container	*/
		if (!m_processes.empty())
		{
			while (m_processes.find(Process(rand() % m_processes.size())), temp_process = *m_processes.find(Process(rand() % m_processes.size())), temp_process == process)
			{
                            
			}
			swap(process, temp_process);
			PROCESS_SWITCH_COUNTER++;
		}
                
		else cerr << ">>\tno process left to switch to\t<<\n\n";
		break;
	default:
		cerr << ">> unknown CPU command <<\n";
		break;
	}
}

/*Statistiken fuer den Laborbericht*/
void SimCPU::printReport() const
{
	const unsigned int sum_of_operations = WRITING_COUNTER + READING_COUNTER + PROCESS_SWITCH_COUNTER;

	cout << "*********************** REPORT *****************************\n";
	cout << "\n__________________________________________________________\n";
	cout << "average reading time:\t";
	cout << "\n";
	cout << "average writing time:\t";
	cout << "\n";
	cout << "page errors in total:\t";
	cout << "\n";
	cout << "\n__________________________________________________________\n";
	cout << "% of reading operations: " << READING_COUNTER / sum_of_operations << "\t|\t% of writing operations: " << WRITING_COUNTER/ sum_of_operations<<"\t|\t%of process switches: " << PROCESS_SWITCH_COUNTER/sum_of_operations<< '\n';
}


SimCPU::~SimCPU()
{
}
