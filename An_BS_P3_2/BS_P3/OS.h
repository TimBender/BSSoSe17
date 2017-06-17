#pragma once
#include"Process.h"
#include<vector>

using namespace std;
class OS
{
public:
	static OS &getInstance() {
		static OS simCPU;
		return simCPU;
	}

	static size_t PAGE_ERROR_COUNTER;
	
	void transferAdress(const bitset<6>& adress);
	void assign(bitset<6> adress, vector<Page>& hard_disk, Process& current_process, array<unsigned char, 1000>& ram, vector<array<unsigned char, 4>>& table);		// assign page to page frame/ physical memory
	void substitute(vector<Page>& hard_disk);	// make room for page (page substitute algo)
	// 2 Funktionen: NRU + FIFO
	~OS();
private:
	OS(){};
	OS(const OS &obj);
	OS &operator=(const OS &obj);
	bitset<6> m_current_adress;
};

