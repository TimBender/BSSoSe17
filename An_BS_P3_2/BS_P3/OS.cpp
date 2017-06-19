#include "OS.h"
#include<iostream>
#include<cmath>
size_t OS::PAGE_ERROR_COUNTER = 0;


void OS::assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<unsigned char, 4>>& table)
{

	for (size_t i = 0; i < hard_disk.size(); i++){
		if (adress.to_ulong() == hard_disk[i].getVirtualAdress().to_ulong() && static_cast<int>(hard_disk[i].getContent()) == static_cast<int>(current_process->getId())){
			findEmptyMemoryspace(current_process, adress, ram);
		
			PAGE_ERROR_COUNTER++;	// increment: page error occured
			/*	search for an empty spot in ram	*/
			return;
		}
	}

	cerr << "ERROR_ page not found.\n";
}

bool OS::findEmptyMemoryspace(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram)
{
	unsigned physical_idx = 0;
	for (size_t i = 0; i < ram.size(); i++){
		if (ram[i] == NULL){
			cout << "assigned page "<< adress.to_string()<< " to physical memory\n\n";

			addToRAM(current_process, adress, ram, i);
			physical_idx = i;

			/* update present bit to 1 */
		
			bitset<2>idx = adress[current_process->find(adress).getVirtualAdress().size() - 2] + (2* static_cast<int>(adress[current_process->find(adress).getVirtualAdress().size() - 1]));

			current_process->getPageTable()[idx.to_ulong()][2] = 0x1;
			current_process->getPageTable()[idx.to_ulong()][3] = (int)physical_idx;	//	insert page frame index to page table
			cout << "\ncurrent page table is updated accordingly.\n";
			return true;
		}
	}
	cout << "RAM IS FULL - TIME TO MAKE ROOM!\n";

	/*	Seitenersetzungsalgo	*/

	/*	FIFO	*/
	/*	NRU		*/


		return false;
}

void OS::addToRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index)
{
	for (size_t j = index; j < index + Process::OFFSET_LENGTH; j++){
		/*Funktion auslagern*/
		ram[j] = current_process->find(adress).getContent();
		cout << "Index: 0x" << j << endl;
	}
}
OS::~OS()
{
}
