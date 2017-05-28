#pragma once
#include"Page.h"
#include<list>
#include<map>
#include<array>

using namespace std;
class Process
{
public:
	struct IDSorter;
	friend class OS;
	static size_t ID;
	static size_t PAGE_COUNTER;
	Process();
	Process(const unsigned int& id);
	bool operator==(const Process& right) const;
	bool operator<(const Process& right) const;
	multimap < Page, bool, IDSorter >  getPageFrame() const{ return m_page_table; }
	~Process();
private:	
	/*Funktionsoperator: Sortierung nach ID asc*/
	struct IDSorter{
		bool operator()(const Page& left, const Page& right)const{
			return left.m_id < right.m_id;
		}
	};
	size_t m_id;
	multimap < Page, bool, IDSorter >  m_page_table;	//	pair: bool [hat einen Seitenrahmen] + Seite [Blockgroesse: 8B]
};

