#pragma once
#include"Process.h"
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;
class SimulatedCPU
{
public:
	static size_t READING_COUNTER, WRITING_COUNTER, PROCESS_SWITCH_COUNTER;
	SimulatedCPU(vector<Process>& processes);
	void execute(const int &cmd);
	void printReport() const;

	void adress_generator_rand_probabilty(const bool& preferCurrentAdress);	// keep current adress with certain probability
	void adress_generator_delta(const unsigned &delta_adress);					// generate adress by +- a delta adress

	~SimulatedCPU();
private:
	vector<Process> m_processes;
	Process m_current_process;
	unsigned m_current_adress;
};



