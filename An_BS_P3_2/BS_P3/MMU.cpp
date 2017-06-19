#include "MMU.h"


void MMU::assignCurrentTable(vector<array<size_t, 4>>& current_table)
{
	m_page_table = current_table;
}

size_t MMU::convertToPhysicalAdress(Process* current_process, const bitset<6>& adress)
{
	int present_bit;
	bitset<2> index = adress[adress.size() - 2] + (2 * (int)adress[adress.size() - 1]); 	//	extract Index/Page Number from virtual adress

	/*	check if adress is valid	*/
	try{
		current_process->find(adress);
	}
	catch (exception& eo){
		throw invalid_argument("ERROR_page not found\n\n");
	}

		/*	look up absent bit at index	*/
		present_bit = m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 2];

		//	success: is in physical memory -> return index of physical memory
	
		if (present_bit == 1) return  static_cast<int>(m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 1]); //return page frame idx

		else throw exception("\n"); 	// fail: Page Error -> call OS
}
