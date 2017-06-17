#pragma once
#include"Process.h"
#include "MMU.h"
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;
class SimulatedCPU
{
public:
	friend class OS;
     static SimulatedCPU &getInstance() {
		 static SimulatedCPU simCPU;
		return simCPU;
	}
	static size_t READING_COUNTER, WRITING_COUNTER, PROCESS_SWITCH_COUNTER;
	void setProcesses(vector<Process>& processes);
	void execute(const int &cmd);
	void printReport() const;

	void adress_generator_rand_probabilty(const bool& preferCurrentAdress);	// keep current adress with certain probability
	void adress_generator_delta(const unsigned &delta_adress);					// generate adress by +- a delta adress
	void readOrWriteToRAM(const bool& isReading, const int& index);

private:
	~SimulatedCPU();
	SimulatedCPU(){};
	SimulatedCPU(const SimulatedCPU &obj);
	SimulatedCPU &operator=(const SimulatedCPU &obj);
	vector<Process> m_processes;
	Process m_current_process;
	unsigned m_current_adress = 0x04;

	/*	components*/
	MMU mmu;
	array<unsigned char, 1000> ram;		//	capacity: 1KB
	vector<Page> hard_disk;
};



