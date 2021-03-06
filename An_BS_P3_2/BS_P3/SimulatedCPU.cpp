﻿#include "SimulatedCPU.h"
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


/*	keep the old adress with a certain probability	-	generate a random new adress otherwise	*/
void SimulatedCPU::adress_generator_rand_probabilty(const bool& preferCurrentAdress)
{
	enum ADRESS {
		CURRENT_ADRESS, NEW_ADRESS
	};

	const unsigned MAX_ADRESS = 0x34;
	const unsigned MIN_ADRESS = 0x04;

	/*	Pseudozahlen Generator	*/
	random_device rd;
	mt19937 gen(rd());
	double prob_current = 0, prob_new = 0;

	(preferCurrentAdress ? prob_current = 0.65 : prob_current = 0.45);

	prob_new = 1.0 - prob_current;
	discrete_distribution<> stochastic_distribution({ prob_current, prob_new });

	switch (ADRESS(stochastic_distribution(gen)))
	{
	case CURRENT_ADRESS:
		break;
	case NEW_ADRESS:
	{
					   unsigned temp_adress;
					   while (temp_adress = (rand() % 34 + 4), static_cast<int>(temp_adress) == static_cast<int>(m_current_adress)){}
					   m_current_adress = temp_adress;
					   if (m_current_adress < MIN_ADRESS) m_current_adress = MIN_ADRESS;
					   if (m_current_adress> MAX_ADRESS) m_current_adress = MAX_ADRESS;
					   break;
	}
	default:
		cerr << "ERROR_please try again.\n";
		break;
	}
}

SimulatedCPU::SimulatedCPU(vector<Process>& processes)
:ram(vector<unsigned char>(1000, NULL))
{
	m_processes = processes;
	m_current_process = &m_processes[rand() % m_processes.size()];	//	take rand process as current process
	mmu.assignCurrentTable(m_current_process->getPageTable());		// assign current page table to MMU
	for (Page& page : m_current_process->getVirtualMemory())
	{
		hard_disk.push_back(page);
	}
	cout << "PROCESS " << m_current_process->getId() << endl << endl;
}


/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void SimulatedCPU::execute(const int &cmd)
{
	/*	choose an adress generator	*/

	cout << "\n______________________________________\n\n";
	switch (INSTRUCTION(cmd))
	{
	case READ:				// LOAD address
		cout << "READING OPERATION\t";

		READING_COUNTER++;
		break;

	case WRITE:				// STORE address
		cout << "WRITING OPERATION\t";

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
			cout << "switched to process " << dec << m_current_process->getId() << '\n';

			PROCESS_SWITCH_COUNTER++;
		}

		else cerr << ">>\tno process left to switch to\t<<\n\n";

		break;
	default:
		cerr << ">> unknown CPU command <<\n";
		break;
	}

	/*	read/write [adress]	*/

	if (INSTRUCTION(cmd) == READ || INSTRUCTION(cmd) == WRITE)
	{
		adress_generator_delta(0x20);
		//adress_generator_rand_probabilty(true); // current adress preferred

		cout << "LOAD 0x" << hex << static_cast<int>(m_current_adress) << "\n\n";
		try{
			readOrWriteToRAM(INSTRUCTION(cmd), mmu.convertToPhysicalAdress(m_current_process, m_current_adress));
		}
		catch (const invalid_argument& eo)
		{
			cout << eo.what() << '\n';
			return;
		}
		catch (const exception& eo)
		{
			fixPageError();
		}
		cout << endl;
	}
}

void SimulatedCPU::fixPageError()
{
	try{
		mmu.getOS().assign(m_current_adress, hard_disk, m_current_process, ram, m_current_process->getPageTable());
		mmu.assignCurrentTable(m_current_process->getPageTable());
	}
	catch (const exception& eo){
		cerr << eo.what() << '\n';
		exit(1);
	}
	
}

void  SimulatedCPU::readOrWriteToRAM(const bool& isReading, const size_t& index)
{

	/* check if page frame is assigned to external process	*/
	if (static_cast<size_t>(ram[index]) != m_current_process->getId()){
		cout << "INDEX1:" << dec << static_cast<size_t>(ram[index]) << '\n';
		cout << "INDEX2:" << dec << m_current_process->getId() << '\n';
		cerr << "ERROR_ attempted to read or write from external process. \n";
		return;
	}
	switch (INSTRUCTION(isReading))
	{
	case READ:
		// Read content of page frame block ( adress + offset ) 
		for (size_t i = 0; i < Process::OFFSET_LENGTH; i++) {
			cout << dec<<'[' << static_cast<int>(ram[index + i]) << "]\n";
		}
		break;

	case WRITE:
		// Write content of page frame block ( adress + offset ) 
		for (size_t i = 0; i < Process::OFFSET_LENGTH; i++) {
			ram[index + i] = m_current_process->getId();
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
	cout <<dec<< "\n______________________________________\n";
	cout << "______________________________________\n\n";
	cout << "REPORT\n\n";
	cout << dec << left << setw(26) << "PAGE ERRORS IN TOTAL:" << OS::PAGE_ERROR_COUNTER;
	cout << "\n";
	cout << left << setw(26)<< "\nREADING OPERATIONS:" << READING_COUNTER << left << setw(26) << "\nWRITING OPERATIONS:" << WRITING_COUNTER << left << setw(26) << "\nPROCESS SWITCHES:" << PROCESS_SWITCH_COUNTER << right << endl << endl;
}
