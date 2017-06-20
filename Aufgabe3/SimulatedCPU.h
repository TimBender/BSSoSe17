#pragma once
#include "MMU.h"
#include "OS.h"

#include<iostream>
#include<vector>
#include<algorithm>
#include <signal.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>

using namespace std;
class SimulatedCPU
{
public:
	friend class OS;
        friend void handler(int i);
	static size_t READING_COUNTER, WRITING_COUNTER, PROCESS_SWITCH_COUNTER;
        
	SimulatedCPU(){};
	SimulatedCPU(vector<Process>& processes);
	void execute(const int &cmd);
	void printReport() const;                                               //  print out statistics
	void adress_generator_rand_probabilty(const bool& preferCurrentAdress);	// keep current adress with certain probability
	void adress_generator_delta(const unsigned &delta_adress);		// generate adress by +- a delta adress
	void readOrWriteToRAM(const bool& isReading, const size_t& index);      //  execute reading - writing command on RAM
	void fixPageError();                                                    //  resolve page errors by OS
        
	~SimulatedCPU(){};
private:
	enum INSTRUCTION{
		READ, WRITE, SWITCH_PROCESS
	};

	vector<Process> m_processes;
	Process *m_current_process;         //  currently executed process
	unsigned m_current_adress = 0x04;   //  currently demanded adress 
       
	/*	components*/
        OS& os= OS::getInstance();
	MMU mmu;
	vector<unsigned char> ram;		//	capacity: 1KB
	vector<Page> hard_disk;
};


//void stepHandler();
