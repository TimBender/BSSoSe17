#pragma once
#include"Page.h"
#include<list>
#include<map>
#include<array>

using namespace std;
class Prozess
{
public:
	static size_t PAGE_COUNTER;
	Prozess();
	~Prozess();
	multimap < Page, bool >  getPageFrame() const;

private:	
	/*Funktionsoperator: Sortierung nach ID asc*/
	struct IDSorter{
		bool operator()(const Page& left, const Page& right)const{
			return left.m_id < right.m_id;
		}
	};

	multimap < Page, bool, IDSorter >  m_page_frame;	//	pair: bool [hat einen Seitenrahmen] + Seite [Blockgroesse: 8B]
};

