#include "OS.h"
#include<iostream>
#include<cmath>
#include<stdexcept>

size_t OS::PAGE_ERROR_COUNTER = 0;


void OS::assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4>>& table)
{
	int num = 0;
	bool hasFreeRAM= 1;
	for (size_t i = 0; i < hard_disk.size(); i++){
		if (adress.to_ulong() == hard_disk[i].getVirtualAdress().to_ulong() && static_cast<int>(hard_disk[i].getContent()) == current_process->getId()){
			hasFreeRAM = findEmptyMemoryspace(current_process, adress, ram);
			if (!hasFreeRAM)	throw exception("RAM IS FULL - TIME TO MAKE ROOM!\n");
			else{
				PAGE_ERROR_COUNTER++;	// increment: page error occured
			}
			/*	search for an empty spot in ram	*/

		
			return;
		}
		num = i;
	}
cerr << "ERROR_ page not found.\n";
}

bool OS::findEmptyMemoryspace(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram)
{
	unsigned physical_idx = 0;
	for (size_t i = 0; i < ram.size(); i++){
		if (ram[i] == NULL){
			cout << "assigned page " << adress.to_string() << " to physical memory\n\n";

			addToRAM(current_process, adress, ram, i);
			physical_idx = i;

			/* update present bit to 1 */

			bitset<2>idx = adress[current_process->find(adress).getVirtualAdress().size() - 2] + (2 * static_cast<int>(adress[current_process->find(adress).getVirtualAdress().size() - 1]));

			current_process->getPageTable()[idx.to_ulong()][2] = 0x1;
			current_process->getPageTable()[idx.to_ulong()][3] = (int)physical_idx;	//	insert page frame index to page table
			cout << "\ncurrent page table is updated accordingly.\n";
			return true;
		}
	}

	return false;
	/*	Seitenersetzungsalgo	*/

	/*	FIFO	*/
	/*	NRU		*/

}

void OS::addToRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index)
{
	for (size_t j = index; j < index + Process::OFFSET_LENGTH; j++){
		/*Funktion auslagern*/
		ram[j] = current_process->find(adress).getContent();
		cout << "Index: 0x" << j << endl;
	}
}

void OS::substitutePageByFIFO()
{
	m_fifo_queue.front().getContent();	//	look up process id of first/ oldest process

}

