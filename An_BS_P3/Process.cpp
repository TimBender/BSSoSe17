#include "Process.h"
#include<iostream>
#include<sstream>
#include<bitset>
#include<cstdlib>

size_t Process::ID = 1;

/**	bei der Erzeugung eines Prozess wird der Speicherbereich alloziert 
	+ Seiten eingespeichert
	+ jeder Prozess bekommt eine ID
 */

Process::Process()
:m_id(ID++)
{
	unsigned int count_pagenumber = 0;
	unsigned int offset = 0;

	/*	Speicherbereich bereits alloziert: 64B / 64 Adressraum	*/
	for (int i = 0; i < m_pages.size(); i++)
	{

		bitset<4> pagenumber(count_pagenumber++);	// Seitennummer
		bitset<28> block_offset(offset);			// Offset
		offset += 4;

		m_pages[i]= Page(bitset<32>(pagenumber.to_string() + block_offset.to_string()));	// fuege Seite hinzu (32b gross)	
	}
	array <unsigned int, 3> info = { 0, 0, NULL };
	m_page_frame.push_back(info);
}

Process::Process(const unsigned int& id)
:m_id(id)
{
}

bool Process::operator==(const Process& right) const
{
	return m_id == right.m_id;
}

bool Process::operator<(const Process& right) const
{
	return m_id < right.m_id;
}
Process::~Process()
{
}
