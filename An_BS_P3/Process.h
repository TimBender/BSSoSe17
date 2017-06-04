#pragma once
#include"Page.h"
#include<list>
#include<vector>
#include<array>
#include<cmath>

using namespace std;
class Process
{

public:
	friend class OS;
	static size_t ID;
	Process();
	Process(const unsigned int& id);
	vector<Page>getPages()const{ return m_pages; }
	bool operator==(const Process& right) const;
	bool operator<(const Process& right) const;
	
	~Process();
private:
	size_t m_id;
	vector<Page> m_pages=  vector<Page>(pow(2, Page::PAGE_NUM_LENGTH));	//	alloziere Speicherbedarf: 2^Seitennummer

	/**	Seitentabelle: 
		Modified	-	Present/Absent	-	Page Frame	*/
	vector < array <unsigned int, 3> > m_page_frame = vector < array <unsigned int, 3> >(pow(2,Page::PAGE_NUM_LENGTH)); //Indexbereich: 2^Seitennummer
};

