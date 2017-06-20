#pragma once
#include"Process.h"
#include<vector>
#include<list>
#include <cstdlib>
#include <string>
#include <map>
#include <utility> 

using namespace std;

class OS {
public:
    static size_t PAGE_ERROR_COUNTER; // count number of page errors
    static size_t TIMEOUT_TRACKER; // counts number of executed operations to calculate the next timeout [NRU]
    static const size_t TIMEOUT_QUANTUM; // Timeout will be triggered when quantum is exhausted

    /*  Singleton Pattern */
    static OS &getInstance() {
        static OS os;
        return os;
    }

    void assign(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4 >> &table); // assign page to page frame/ physical memory
    void substitute(vector<Page>& hard_disk); // make room for page (page substitute algo)
    bool findEmptyMemoryspace(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram); //  checks if there is enough free space in RAM for the page to be added to
    void addToRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index); //  adds page to RAM
    void addToRAM(vector<unsigned char>& ram); //  adds page to RAM, after space was deallocated beforehand
    void updateRefModBit(const size_t& whichBit, Process* current_process, const bitset<6>& adress); //  NRU: set mod and/or ref bit to 1
    // Seitenersetzungsalgos: fuer Testzwecke eine auswählen: NRU - FIFO
    bool substitutePageByNRU(Process& process, vector<unsigned char>& ram); // FIFO Seitenersetzungsalgo      
    bool substitutePageByFIFO(const int& idx, vector<array<size_t, 4 >> &table, vector<array<size_t, 4 >> &current_table, vector<unsigned char>& ram); // FIFO Seitenersetzungsalgo
    void resetRefBit(Process& current_process); //  reset refrence bit to 0

    ~OS() {
    };
private:
    multimap<size_t, Page> m_page_classes; // NRU Seitenersetzungalgo: size_t= Klasse; 
    size_t determineClass(Process* current_process, const bitset<6>& adress); // returns page's Class 
    list<Page> m_fifo_queue; // FIFO tracken fuer den Seitenersetzungsalgo
    Page m_oldest_page; // oldest FIFO queue page
    size_t m_reallocated_pageframe_nr; // oldest page frame index
    Page m_assignable_page; //Page: waiting to be added to physical memory/RAM

    size_t extractPageNum(const bitset<6>& adress) const; // extracts the page number from the virtual adress
    Page& randLowestClass();

    /*  Singleton Pattern */
    OS() {
    };
    OS(const OS &obj);
    OS &operator=(const OS &obj);
};

