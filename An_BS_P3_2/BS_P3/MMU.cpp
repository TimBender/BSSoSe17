#include "MMU.h"
#include<iostream>


MMU::MMU()
{
}

void MMU::assignCurrentTable(vector<array<unsigned char, 4>>& current_table)
{
	m_page_table = current_table;
}

bitset<2> MMU::convertToPhysicalAdress(const bitset<6>& adress)
{
	int present_bit;
	/*	extract Index/Page Number from virtual adress	*/
	bitset<2> index = adress[adress.size() - 2] +  (2* (int)adress[adress.size() - 1]);

	//cout <<"MMU INDEX: "<< index.to_ulong()<< '\n';

	/*	look up absent bit at index	*/
	present_bit = m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 2];

	//	success: is in physical memory -> return index of physical memory
	if (present_bit == 1) return  m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 1]; //return page frame idx

	else{					// fail: Page Error -> call OS
		cout << "PAGE ERROR: GARBAGE PAGE FRAME INDEX\n";
		throw exception("\nPAGE ERROR RESOLVED\n");
	}
}

MMU::~MMU()
{
}
