#include "Process.h"
#include <iostream>
#include<string>

size_t Process::OFFSET = 4;
size_t Process::ID_COUNTER = 1;
Process::Process()
:m_id(ID_COUNTER++)
{
	unsigned char garbage_pageNr = 0xF;

	array <unsigned char, 2> content= {garbage_pageNr, 0 };	// adress: garbage;	modified: 0; absent/present=0;
	for (size_t i = 0; i < m_table.size(); i++){
		m_table[i] = content;
		//cout << std::bitset<8>(content[0]).to_string() << endl;
	}
	//cout << hex << (int) garbage_pageNr << '\n';
	//cout << std::bitset<8>(garbage_pageNr)[0] << endl;
}

int Process::getModifiedBit(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return m_table[pageNr][1];
}
int Process::getPresentBit(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return m_table[pageNr][2];
}
bitset<8> Process::getPageFrame(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return bitset<8>(m_table[pageNr][0]);
}

Process::~Process()
{


}