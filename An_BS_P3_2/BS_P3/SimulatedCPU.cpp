#include "SimulatedCPU.h"
#include<random>

size_t SimulatedCPU::READING_COUNTER = 0, SimulatedCPU::WRITING_COUNTER = 0, SimulatedCPU::PROCESS_SWITCH_COUNTER = 0;

SimulatedCPU::SimulatedCPU(vector<Process>& processes)
:m_processes(processes)
{
	m_current_process = m_processes[rand() % m_processes.size()];	//	take rand process as current process
}

/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void SimulatedCPU::execute(const int &cmd)
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

	case SWITCH_PROCESS:	//	willkuerlicher Prozesswechsel
		cout << "---\tPROCESS SWITCH\t---\n";
		cout << "___________________________________\n\n";

		/*	swap current process with random process in the container */
		cout << "Adresse: " << m_current_process.getId() << endl;
		if (!m_processes.empty())
		{
			int rand_number = 0;

			while (rand_number = rand() % m_processes.size(), &(m_processes[rand_number]) == &(m_current_process)) {}

			m_current_process = m_processes[rand_number];	// swap current process with a different random process

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
void SimulatedCPU::printReport() const
{
	const unsigned int sum_of_operations = WRITING_COUNTER + READING_COUNTER + PROCESS_SWITCH_COUNTER;

	cout << "*********************** REPORT *****************************\n";
	cout << "PAGE ERRORS IN TOTAL:\t";
	cout << "\n";
	cout << "\n__________________________________________________________\n";
	cout << "% of reading operations: " << READING_COUNTER / sum_of_operations << "\t|\t% of writing operations: " << WRITING_COUNTER / sum_of_operations << "\t|\tnumber of process switches: " << PROCESS_SWITCH_COUNTER << '\n';
}

/*	pick another adress by adding or subtracting (by a certain probaility) the specified delta adress	*/
void  SimulatedCPU::adress_generator_delta(const unsigned &delta_adress)
{
	const unsigned MAX_ADRESS = 0x34;
	int rand_num = rand() % 10 + 1;
	if (rand_num % 2 == 0)m_current_adress += delta_adress;
	else m_current_adress -= delta_adress;

	if (m_current_adress < 0x0) m_current_adress = 0x0;
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
		m_current_adress =  '0x' + rand() % 34;
		cout << hex<< (int) m_current_adress << '\n';
		break;
	default:
		cerr << "ERROR_please try again.\n";
		break;
	}
}

SimulatedCPU::~SimulatedCPU()
{
}
