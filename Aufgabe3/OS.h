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
        void addToRAM( vector<unsigned char>& ram);
	// 2 Funktionen: NRU + FIFO
        bool substitutePageByFIFO (const int& idx, vector<array<size_t,4>>& table, vector<array<size_t, 4>>& current_table ,vector<unsigned char>& ram);
      
	~OS(){};
private:
        void modifyPageTable();
	list<Page> m_fifo_queue;	// FIFO tracken fuer den Seitenersetzungsalgo
        Page m_oldest_page;
	OS(){};
	OS(const OS &obj);
	OS &operator=(const OS &obj);
	size_t m_oldest_idx; //Aeltester Seitenrahmen-Index
        Page m_assignable_page;
};

