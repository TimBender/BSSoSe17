#include "Process.h"
#include <iostream>
#include<string>
#include <cmath>

const size_t Process::OFFSET_LENGTH = 4;			
const size_t Process::PAGENR_LENGTH = 2;
size_t Process::ID_COUNTER = 1;								// assign pages to specific processes
const size_t Process::SIZE = pow(2,PAGENR_LENGTH);			// allocated storage= 2^PAGE NUMBER


Process::Process()
:m_id(ID_COUNTER++), m_virtual_memory(SIZE), m_table(vector<array<size_t, 4>>(SIZE))
{
	const unsigned char GARBAGE_PAGE_NR = 0x3;		//	default / garbage value for the page table = 11
	const unsigned char OFFSET = 0x4;
	const unsigned NEXT_ADRESS = 0x10;

	unsigned start_adress = 0x0;

	/*	create pages/ virtual memory	*/
	for (size_t i = 0; i < m_virtual_memory.size(); i++)
	{
		bitset<6> block(start_adress + OFFSET); // die 2 vorderen Bits werden gecuttet -> 8b- 2b
		m_virtual_memory[i] = Page(block, m_id);
		
		start_adress += NEXT_ADRESS;
		// cout << m_virtual_memory[i].getVirtualAdress().to_string()<<'\t'<< m_virtual_memory[i].getContent() << '\n';
	}

	/*	create page table*/
	array <size_t, 4> content = { 0, 0 , 0, GARBAGE_PAGE_NR };	// adress: garbage;	modified: 0; absent/present=0;
	for (size_t i = 0; i < m_table.size(); i++){
		m_table[i] = content;
		//cout << std::bitset<8>(content[0]).to_string() << endl;
	}
	//cout << hex << (int) GARBAGE_PAGE_NR << '\n';
	//cout << std::bitset<8>(GARBAGE_PAGE_NR)[0] << endl;
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

/*	detect if page is assigned to this process	*/
Page Process::find(const bitset<6>& adress){
	for (Page& page : m_virtual_memory){
		if (page.getVirtualAdress().to_string() == adress.to_string()) return page;
	}
	throw exception();
}

Process::~Process()
{


}