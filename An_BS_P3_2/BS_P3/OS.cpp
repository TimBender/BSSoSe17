#include "OS.h"
#include<iostream>

size_t OS::PAGE_ERROR_COUNTER = 0;


void OS::assign(bitset<6> adress, vector<Page>& hard_disk, Process& current_process, array<unsigned char, 1000>& ram, vector<array<unsigned char, 4>>& table)
{
	PAGE_ERROR_COUNTER++;	// increment: page error occured
	for (size_t i = 0; i < hard_disk.size(); i++){
		if (adress == hard_disk[i].getVirtualAdress() && hard_disk[i].getContent() == to_string(current_process.getId())){
			hard_disk.erase(hard_disk.begin() + i);

			/*	search for an empty spot in ram	*/
			return;
		}
	}
	cerr << "ERROR_ page not found.\n";

}

OS::~OS()
{
}
