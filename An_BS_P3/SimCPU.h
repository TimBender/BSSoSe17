#pragma once
#include"Process.h"
#include<iostream>
#include<set>
#include<algorithm>

using namespace std;
class SimCPU
{
public:
	static size_t READING_COUNTER, WRITING_COUNTER, PROCESS_SWITCH_COUNTER;
	SimCPU(set<Process>& processes);
	void execute(Process& process, const int &cmd);
	void printReport() const;
	~SimCPU();
private:
	set<Process> m_processes;
};



