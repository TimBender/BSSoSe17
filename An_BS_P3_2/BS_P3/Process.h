#pragma once
#include"Page.h"
#include<bitset>
#include<array>

using namespace std;
class Process
{
public:
	static size_t ID_COUNTER;
	static size_t OFFSET;
	Process();
	bitset<8> getPage() const{ return m_page; }
	size_t getId() const { return m_id; }
	int getModifiedBit(const int& pageNr)const;
	int getPresentBit(const int& pageNr)const;
	bitset<8> getPageFrame(const int& pageNr)const;
	~Process();
private:
	/**	information:
	Page Frame		-	Present / Absent*/
	array<array<unsigned char, 2>, 16> m_table;
	bitset<8> m_page;
	size_t m_id;
};

