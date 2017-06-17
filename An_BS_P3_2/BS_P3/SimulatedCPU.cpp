#include "SimulatedCPU.h"
#include <random>
#include <stdexcept>
#include<iomanip>

size_t SimulatedCPU::READING_COUNTER = 0, SimulatedCPU::WRITING_COUNTER = 0, SimulatedCPU::PROCESS_SWITCH_COUNTER = 0;

SimulatedCPU::SimulatedCPU(vector<Process>& processes)
:m_processes(processes)
{
	m_current_process = m_processes[rand() % m_processes.size()];	//	take rand process as current process
	mmu.assignCurrentTable(m_current_process.getPageTable());		// assign current page table to MMU
}

/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void SimulatedCPU::execute(const int &cmd)
{
	array<unsigned char, 1000> RAM;		//	capacity: 1KB
	vector<Page> m_hard_disk;

	enum INSTRUCTION{
		READ, WRITE, SWITCH_PROCESS
	};

	switch (INSTRUCTION(cmd))
	{
	case READ:				// LOAD address
		cout << "READING OPERATION\t";
		adress_generator_delta(0x20);
		cout <<"LOAD 0x" <<hex<< static_cast<int>(m_current_adress) << "\n\n";
		try{
			mmu.convertToPhysicalAdress(m_current_adress);

		}
		catch (const exception& eo)
		{
			cerr << setw(15) << eo.what();
		}

		cout << hex << (int)m_current_adress << '\n';
		cout << "______________________________________\n\n";
		READING_COUNTER++;
		break;

	case WRITE:				// STORE address
		cout << "WRITING OPERATION\t";
		adress_generator_delta(0x20);
		cout << "STORE 0x" << hex << static_cast<int>(m_current_adress) << "\n\n";
		try{
			mmu.convertToPhysicalAdress(m_current_adress);

		}
		catch (const exception& eo)
		{
			cerr << eo.what();
		}

		cout << hex << (int)m_current_adress << '\n';
		cout << "______________________________________\n\n";
		WRITING_COUNTER++;
		break;

	case SWITCH_PROCESS:	//	random process switch
		cout << "PROCESS SWITCH\t";

		/*	swap current process with random process in the container */

		if (!m_processes.empty())
		{
			int rand_number = 0;

			while (rand_number = rand() % m_processes.size(), (m_processes[rand_number]).getId() == (m_current_process).getId()) {}

			m_current_process = m_processes[rand_number];					// swap current process with a different random process
			mmu.assignCurrentTable(m_current_process.getPageTable());		// assign current page table to MMU
			cout << "switched to process " << m_current_process.getId() << '\n';
	
			PROCESS_SWITCH_COUNTER++;
		}

		else cerr << ">>\tno process left to switch to\t<<\n\n";
		cout << "______________________________________\n\n";
		break;
	default:
		cerr << ">> unknown CPU command <<\n";
		break;
	}
}

/*Statistiken fuer den Laborbericht*/
void SimulatedCPU::printReport() const
{
	const unsigned int sum_of_operations = WRITING_COUNTER + READING_COUNTER + PROCESS_SWITCH_COUNTER;
	cout << "______________________________________\n";
	cout << "______________________________________\n\n";
	cout << "REPORT\n\n";
	cout << "PAGE ERRORS IN TOTAL:\t";
	cout << "\n";
	cout << "% of reading operations: " << READING_COUNTER / sum_of_operations << "\n|% of writing operations: " << WRITING_COUNTER / sum_of_operations << "\n|number of process switches: " << PROCESS_SWITCH_COUNTER << '\n';
}

/*	pick another adress by adding or subtracting (by a certain probaility) the specified delta adress	*/
void  SimulatedCPU::adress_generator_delta(const unsigned &delta_adress)
{
	const unsigned MAX_ADRESS = 0x34;
	const unsigned MIN_ADRESS = 0x04;

	int rand_num = rand() % 10 + 1;
	if (rand_num % 2 == 0)m_current_adress += delta_adress;
	else m_current_adress -= delta_adress;

	if (m_current_adress < MIN_ADRESS) m_current_adress = MIN_ADRESS;
	if (m_current_adress> MAX_ADRESS) m_current_adress = MAX_ADRESS;
}


void SimulatedCPU::adress_generator_rand_probabilty(const bool& preferCurrentAdress)
{
	enum ADRESS{
		CURRENT_ADRESS, NEW_ADRESS
	};

	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());
	double prob_current = 0, prob_new = 0;


	(preferCurrentAdress ? prob_current = 2, prob_new = 1 : prob_current = 1, prob_new = 2);

	discrete_distribution<> stochastic_distribution({ prob_current, prob_new });

	switch (ADRESS(stochastic_distribution(gen)))
	{
	case CURRENT_ADRESS:
		break;
	case NEW_ADRESS:
		m_current_adress = '0x' + (rand() % 34 + 4);
		cout << hex << (int)m_current_adress << '\n';
		break;
	default:
		cerr << "ERROR_please try again.\n";
		break;
	}
}

SimulatedCPU::~SimulatedCPU()
{
}
