#include "OS.h"
#include<iostream>
#include<cmath>
#include<stdexcept>

size_t OS::PAGE_ERROR_COUNTER = 0;

void OS::assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4 >> &table) {
    int num = 0;
    bool hasFreeRAM = 1;
    for (size_t i = 0; i < hard_disk.size(); i++) {
        if (adress.to_ulong() == hard_disk[i].getVirtualAdress().to_ulong() && hard_disk[i].getContent() == current_process->getId()) {
            hasFreeRAM = findEmptyMemoryspace(current_process, adress, ram);
            if (!hasFreeRAM) {
                m_assignable_page = hard_disk[i];
                throw exception(/*"RAM IS FULL - TIME TO MAKE ROOM!\n"*/);
            } else {
                PAGE_ERROR_COUNTER++; // increment: page error occured
            }
            /*	search for an empty spot in ram	*/


            return;
        }
        num = i;
    }

    cerr << "ERROR_ page not found.\n";
}

bool OS::findEmptyMemoryspace(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram) {
    size_t physical_idx = 0;
    for (size_t i = 0; i < ram.size(); i++) {
        if (ram[i] == NULL) {
            cout << "assigned page " << adress.to_string() << " to physical memory\n\n";

            addToRAM(current_process, adress, ram, i);
            physical_idx = i;

            /* update present bit to 1 */

            bitset<2>idx = adress[current_process->find(adress).getVirtualAdress().size() - 2] + (2 * static_cast<int> (adress[current_process->find(adress).getVirtualAdress().size() - 1]));

            current_process->getPageTable()[idx.to_ulong()][2] = 0x1;
            current_process->getPageTable()[idx.to_ulong()][3] = physical_idx; //	insert page frame index to page table

            cout << "\ncurrent page table is updated accordingly.\n";
            return true;
        }
    }

    return false;
    /*	Seitenersetzungsalgo	*/

    /*	FIFO	*/
    /*	NRU		*/

}

void OS::addToRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index) {
    m_fifo_queue.push_back(current_process->find(adress)); // Seitenersetzungsalgo: fifo queuing

    for (size_t j = index; j < index + Process::OFFSET_LENGTH; j++) {
        /*Funktion auslagern*/
        ram[j] = current_process->find(adress).getContent();
        cout << "Index: 0x" << j << endl;
    }
}
void OS::addToRAM(vector<unsigned char>& ram)
{
    m_fifo_queue.push_back(m_assignable_page); // Seitenersetzungsalgo: fifo queuing
    
    cout<<endl<<"Emptied memory space:\n";
    for (size_t j = m_oldest_idx; j < m_oldest_idx + Process::OFFSET_LENGTH; j++) {
        /*Funktion auslagern*/
        ram[j] = m_assignable_page.getContent();
        cout << "Index: 0x" << j << endl;
    } 
}
/**     1. Bei Seitenfehler wird die Seite am Kopf der Liste (d.h. die älteste) entfernt
        2. die eingelagerte Seite wird am Ende eingefügt.
 */


bool OS::substitutePageByFIFO(const int& idx, vector<array<size_t, 4> >& table, vector<array<size_t, 4> >& current_table, vector<unsigned char>& ram) {
    m_fifo_queue.front();
    m_oldest_page = m_fifo_queue.front();

    if (idx == m_oldest_page.getContent()) {
        bitset<2> index = m_oldest_page.getVirtualAdress()[m_oldest_page.getVirtualAdress().size() - 2] + 2 * m_oldest_page.getVirtualAdress()[ m_oldest_page.getVirtualAdress().size() - 1];
        table[index.to_ulong()][table[0].size() - 2] = 0; // set present bit to 0
        m_oldest_idx = table[index.to_ulong()][table[0].size() - 1]; // Adresse des aeltesten Prozesses im physischen Speicher/ RAM
        
        addToRAM(ram);
        
        m_fifo_queue.pop_front();   //  entferne aelteste Seite aus der FIFO queue
   
        return true;
    }
    return false;
}
