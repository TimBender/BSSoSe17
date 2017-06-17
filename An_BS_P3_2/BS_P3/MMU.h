#pragma once
#include<vector>
#include<array>
#include<bitset>

using namespace std;
class MMU
{
public:
	MMU();
	void assignCurrentTable(vector<array<unsigned char, 4>>& current_table);
	bitset<6> convertToPhysicalAdress(const bitset<6>& adress);
	~MMU();
private:
	vector<array<unsigned char, 4>> m_page_table;
};

