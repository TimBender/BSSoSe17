#include "OS.h"
#include<iostream>
#include<cmath>
#include<stdexcept>

size_t OS::PAGE_ERROR_COUNTER = 0;
size_t OS::TIMEOUT_TRACKER = 0;
const size_t OS::TIMEOUT_QUANTUM = 4;

void OS::assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4 >> &table) {
    int num = 0;
    bool hasFreeRAM = 1;
    for (size_t i = 0; i < hard_disk.size(); i++) {
        if (adress.to_ulong() == hard_disk[i].getVirtualAdress().to_ulong() && hard_disk[i].getContent() == current_process->getId()) {
            
            // bei OS Erstellung 1x alle Pages einlesen mit Klasse!
            // NRU UNIKATE VERMEIDEN! nach Page suchen dann updaten -> doppelte Schleife 
            
            
            hasFreeRAM = findEmptyMemoryspace(current_process, adress, ram);
            if (!hasFreeRAM) {
                m_assignable_page = hard_disk[i];
                throw exception();
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
        if (ram[i] == 0) {
            cout << "assigned page " << adress.to_string() << " to physical memory\n\n";

            addToRAM(current_process, adress, ram, i);
            physical_idx = i;

            /* update present bit to 1 */
            size_t idx = extractPageNum(current_process->find(adress).getVirtualAdress());
            // bitset<2>idx = adress[current_process->find(adress).getVirtualAdress().size() - 2] + (2 * static_cast<int> (adress[current_process->find(adress).getVirtualAdress().size() - 1]));

            current_process->getPageTable()[idx][2] = 0x1;
            current_process->getPageTable()[idx][3] = physical_idx; //	insert page frame index to page table

            cout << "\ncurrent page table is updated accordingly.\n";
            return true;
        }
    }

    return false;
}

void OS::addToRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index) {
    m_fifo_queue.push_back(current_process->find(adress)); // Seitenersetzungsalgo: fifo queuing

    size_t pClass = determineClass(current_process, adress);

    m_page_classes.insert(make_pair(pClass, current_process->find(adress)));

    for (size_t j = index; j < index + Process::OFFSET_LENGTH; j++) {
        /*Funktion auslagern*/
        ram[j] = current_process->find(adress).getContent();
        cout << "Index: 0x" << j << endl;
    }
}

void OS::updateRefModBit(const size_t& whichBit, Process* current_process, const bitset<6>& adress) {

    enum BIT {
        REF_BIT, MOD_BIT
    };

    size_t index = extractPageNum(adress);
    switch (BIT(whichBit)) {
        case REF_BIT:
            current_process->getPageTable()[index][0] = 1;
            break;
        case MOD_BIT:
            current_process->getPageTable()[index][0] = 1;
            current_process->getPageTable()[index][1] = 1;
            break;
        default:
            cerr << "Invalid command - Unspecified bit\n";
            break;
    }
}

Page& OS::randLowestClass() {
    size_t counter = 0;
    int randIdx = static_cast<int> (rand() % m_page_classes.size()); // generate random index for picking a page of the lowest class   
    auto p_lowest_class = m_page_classes.equal_range(m_page_classes.begin()->first); // get iterator pointing to pages of the lowest existing class

    for (auto it = p_lowest_class.first; it != p_lowest_class.second; it++) {
        if (counter == randIdx) return it->second;
        counter++;
    }
}

size_t OS::determineClass(Process* current_process, const bitset<6>& adress) {

    enum CLASS {
        CLASS1 = 1, CLASS2, CLASS3, CLASS4
    };

    const size_t INDEX = extractPageNum(adress);

    /*  look up Reference Bit & Modified Bit    */
    const size_t REF_BIT = current_process->getPageTable()[INDEX][0];
    const size_t MOD_BIT = current_process->getPageTable()[INDEX][1];

    if (REF_BIT == 0 && MOD_BIT == 0) return static_cast<size_t> (CLASS1);
    if (REF_BIT == 0 && MOD_BIT == 1) return static_cast<size_t> (CLASS2);
    if (REF_BIT == 1 && MOD_BIT == 0) return static_cast<size_t> (CLASS3);
    if (REF_BIT == 1 && MOD_BIT == 1) return static_cast<size_t> (CLASS4);
}

void OS::resetRefBit(Process& current_process) {
    /*  iterate through every page table entry and reset refrence bit to 0*/
    for (array<size_t, 4>& p_table_row : current_process.getPageTable()) {
        p_table_row[0] = 0;
    }
}

void OS::addToRAM(vector<unsigned char>& ram) {
    m_fifo_queue.push_back(m_assignable_page); // Seitenersetzungsalgo: fifo queuing

    cout << endl << "Emptied memory space:\n";
    for (size_t j = m_reallocated_pageframe_nr; j < m_reallocated_pageframe_nr + Process::OFFSET_LENGTH; j++) {
        /*Funktion auslagern*/
        ram[j] = m_assignable_page.getContent();
        cout << "Index: 0x" << j << endl;
    }
}

bool OS::substitutePageByNRU(Process& process, vector<unsigned char>& ram) {
    Page& rand_lowest_class_page = randLowestClass();
    size_t pageNr;
    if (process.getId() == rand_lowest_class_page.getContent()) {
        pageNr = extractPageNum(rand_lowest_class_page.getVirtualAdress());
        /*  reset modified and reference bit to 0*/
        //        process.getPageTable()[pageNr][0] = 0;
        //        process.getPageTable()[pageNr][1] = 0;
        process.getPageTable()[pageNr][process.getPageTable()[0].size() - 2] = 0; // set present bit to 0
        m_reallocated_pageframe_nr = process.getPageTable()[pageNr][process.getPageTable()[0].size() - 1]; // page frame number of reallocated page width lowest class

        size_t assignable_pageNr = extractPageNum(m_assignable_page.getVirtualAdress());
        process.getPageTable()[assignable_pageNr][ process.getPageTable()[0].size() - 1] = m_reallocated_pageframe_nr; // replace garbage with correct pageframe number 
        process.getPageTable()[assignable_pageNr][ process.getPageTable()[0].size() - 2] = 1; // set present bit of assignable page to 1

        addToRAM(ram);
        return true;
    }
    return false;
}

/**     1. Bei Seitenfehler wird die Seite am Kopf der Liste (d.h. die älteste) entfernt
        2. die eingelagerte Seite wird am Ende eingefügt.
 */
bool OS::substitutePageByFIFO(const int& idx, vector<array<size_t, 4> >& table, vector<array<size_t, 4> >& current_table, vector<unsigned char>& ram) {
    m_fifo_queue.front();
    m_oldest_page = m_fifo_queue.front();

    if (idx == m_oldest_page.getContent()) {
        size_t pageNr = extractPageNum(m_oldest_page.getVirtualAdress());
        // bitset<2> index = m_oldest_page.getVirtualAdress()[m_oldest_page.getVirtualAdress().size() - 2] + 2 * m_oldest_page.getVirtualAdress()[ m_oldest_page.getVirtualAdress().size() - 1];
        table[pageNr ][table[0].size() - 2] = 0; // set present bit to 0
        m_reallocated_pageframe_nr = table[pageNr][table[0].size() - 1]; // Adresse des aeltesten Prozesses im physischen Speicher/ RAM

        size_t assignable_pageNr = extractPageNum(m_assignable_page.getVirtualAdress());
        current_table[assignable_pageNr][current_table[0].size() - 1] = m_reallocated_pageframe_nr; // replace garbage with correct pageframe number 
        current_table[assignable_pageNr][current_table[0].size() - 2] = 1; // set present bit of assignable page to 1
        addToRAM(ram);

        m_fifo_queue.pop_front(); //  entferne aelteste Seite aus der FIFO queue

        return true;
    }
    return false;
}

size_t OS::extractPageNum(const bitset<6>& adress) const {
    bitset<2> index = adress[adress.size() - 2] + 2 * adress[ adress.size() - 1];
    return index.to_ulong();
}