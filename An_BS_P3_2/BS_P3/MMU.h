#pragma once
#include"OS.h"
#include<vector>
#include<array>
#include<bitset>
#include<iostream>

using namespace std;
class MMU
{
public:
	MMU(){};
	OS& getOS(){ return os; }
	void assignCurrentTable(vector<array<unsigned char, 4>>& current_table);
	size_t convertToPhysicalAdress(Process* current_process, const bitset<6>& adress);
	~MMU(){};
private:
	OS& os= OS::getInstance();
	vector<array<unsigned char, 4>> m_page_table;
};

