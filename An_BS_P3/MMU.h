#pragma once
#include"OS.h"
#include<map>

using namespace std;
class MMU
{
public:
	static size_t PAGE_ERROR_COUNTER;	//zaehlt die Seitenfehler fuer den Report
	MMU();
	void translateAddress(Page& page);
	void setCurrentPageChart(const multimap < Page, bool>& table);
	~MMU();
private:
	OS &m_OS; 
	multimap < Page, bool> m_current_page_table;
    
};

