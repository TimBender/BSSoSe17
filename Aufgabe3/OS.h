#pragma once
#include"Process.h"
#include<vector>
#include<list>
#include <cstdlib>
#include <string>

using namespace std;
class OS
{
public:
	static size_t PAGE_ERROR_COUNTER;

	static OS &getInstance() {
		static OS os;
		return os;
	}

	void transferAdress(const bitset<6>& adress);
	void assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4>>& table);		// assign page to page frame/ physical memory
	void substitute(vector<Page>& hard_disk);	// make room for page (page substitute algo)
	bool findEmptyMemoryspace(Process* current_process,const bitset<6>& adress, vector<unsigned char>& ram);
	void addToRAM(Process* current_process, const bitset<6>& adress,vector<unsigned char>& ram, const size_t& index);
	// 2 Funktionen: NRU + FIFO
	void substitutePageByFIFO();
	~OS(){};
private:
	list<Page> m_fifo_queue;	// FIFO tracken fuer den Seitenersetzungsalgo
	OS(){};
	OS(const OS &obj);
	OS &operator=(const OS &obj);
	bitset<6> m_current_adress;
};

