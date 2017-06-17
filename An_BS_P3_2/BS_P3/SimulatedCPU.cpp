#include "SimulatedCPU.h"
#include <random>
#include <stdexcept>
#include<iomanip>

size_t SimulatedCPU::READING_COUNTER = 0, SimulatedCPU::WRITING_COUNTER = 0, SimulatedCPU::PROCESS_SWITCH_COUNTER = 0;


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

void  SimulatedCPU::setProcesses(vector<Process>& processes)
{
	m_processes = processes;
	m_current_process = &m_processes[rand() % m_processes.size()];	//	take rand process as current process
	mmu.assignCurrentTable(m_current_process->getPageTable());		// assign current page table to MMU
	for (Page page : m_current_process->getVirtualMemory())
	{
		hard_disk.push_back(page);
	}
	cout << "PROCESS " << m_current_process->getId() << endl<<endl;
}

/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void SimulatedCPU::execute(const int &cmd)
{
	enum INSTRUCTION{
		READ, WRITE, SWITCH_PROCESS
	};

	switch (INSTRUCTION(cmd))
	{
	case READ:				// LOAD address
		cout << "READING OPERATION\t";
		adress_generator_delta(0x20);
		cout << "LOAD 0x" << hex << static_cast<int>(m_current_adress) << "\n\n";
		try{
			readOrWriteToRAM(1, static_cast<int>(mmu.convertToPhysicalAdress(m_current_adress).to_ulong()));
		}
		catch (const exception& eo)
		{
			fixPageError();
			cerr << setw(15) << eo.what();
		}
		cout << "______________________________________\n\n";
		READING_COUNTER++;
		break;

	case WRITE:				// STORE address
		cout << "WRITING OPERATION\t";
		adress_generator_delta(0x20);
		cout << "STORE 0x" << hex << static_cast<int>(m_current_adress) << "\n\n";
		try{
			readOrWriteToRAM(0, static_cast<int>(mmu.convertToPhysicalAdress(m_current_adress).to_ulong()));
		}
		catch (const exception& eo)
		{
			fixPageError();
			cerr << eo.what();
		}
		cout << "______________________________________\n\n";
		WRITING_COUNTER++;
		break;

	case SWITCH_PROCESS:	//	random process switch
		cout << "PROCESS SWITCH\t";

		/*	swap current process with random process in the container */

		if (!m_processes.empty())
		{
			int rand_number = 0;

			while (rand_number = rand() % m_processes.size(), (m_processes[rand_number]).getId() == (m_current_process)->getId()) {}

			m_current_process = &m_processes[rand_number];					// swap current process with a different random process
			mmu.assignCurrentTable(m_current_process->getPageTable());		// assign current page table to MMU

			for (Page& page : m_current_process->getVirtualMemory())			// allocate pages without frame in hard disk
			{
				hard_disk.push_back(page);
			}
			cout << "switched to process " << m_current_process->getId() << '\n';

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

void SimulatedCPU::fixPageError()
{
	mmu.getOS().assign(m_current_adress, hard_disk, m_current_process, ram, m_current_process->getPageTable());
	mmu.assignCurrentTable(m_current_process->getPageTable());
}

void  SimulatedCPU::readOrWriteToRAM(const bool& isReading, const int& index)
{
	enum INSTRUCTION{
		WRITE, READ
	};

	/* check if page frame is assigned to external process	*/


	switch (INSTRUCTION(isReading))
	{
	case WRITE:
		// Write content of page frame block ( adress + offset ) 
		for (size_t i = 0; i < Process::OFFSET_LENGTH; i++){
			ram[index + i] = m_current_process->getId();
		}
		break;

	case READ:
		// Read content of page frame block ( adress + offset ) 
		for (size_t i = 0; i < Process::OFFSET_LENGTH; i++){
			cout << '[' <<static_cast<int>(ram[index + i]) << "]\n";
		}
		break;

	default:
		cerr << "Invalid operation\n";
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
	cout << "PAGE ERRORS IN TOTAL:\t"<< OS::PAGE_ERROR_COUNTER;
	cout << "\n";
	cout << dec<<"% of reading operations: " << READING_COUNTER / sum_of_operations << "\n|% of writing operations: " << WRITING_COUNTER / sum_of_operations << "\n|number of process switches: " << PROCESS_SWITCH_COUNTER << '\n';
}

SimulatedCPU::~SimulatedCPU()
{
}
