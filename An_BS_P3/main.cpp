#include"Prozess.h"
#include<iostream>
#include<array>
#include<random>

using namespace std;
void simulateCPU(const int &cmd);
int main(int argc, char** argv)
{
	std::random_device rd;
	std::mt19937 gen(rd());		//	Pseudozahlen Generator

	int randNum;
	array<char, 1000> ram;		//	1 char = 1B
	Prozess process;

	/** am wahrscheinlichsten (desc):
							0 = schreiben
							1 = lesen
							2 = Prozesswechsel */

	discrete_distribution<> stochastic_distribution({ 3, 2, 1 }); 
	for (size_t i = 0; i < 10; i++)
	{

		randNum = stochastic_distribution(gen);

		/*simulate CPU*/
		simulateCPU(randNum);
	}
}

void simulateCPU(const int &cmd)
{
	enum INSTRUCTION{
		WRITE, READ, SWITCH_PROCESS
	};

	switch (INSTRUCTION(cmd))
	{
	case WRITE:
		cout <<"write\n";
		break;
	case READ:
		cout << "read\n";
		break;
	case SWITCH_PROCESS:
		cout << "switch\n";
		break;
	default:
		cerr << "- unknown CPU command -\n";
		break;
	}


}