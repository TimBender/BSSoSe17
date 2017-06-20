#pragma once
#include"OS.h"
#include<vector>
#include<array>
#include<bitset>
#include<iostream>
#include<stdexcept>
using namespace std;
class MMU
{
public:
	MMU(){};
	void assignCurrentTable(vector<array<size_t, 4>>& current_table);
	size_t convertToPhysicalAdress(Process* current_process, const bitset<6>& adress);
	~MMU(){};
private:
	OS& os= OS::getInstance();
	vector<array<size_t, 4>> m_page_table;
};

