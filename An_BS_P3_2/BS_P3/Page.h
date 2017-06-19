#pragma once
#include<bitset>
#include<string>

using namespace std;
class Page
{
public:
	friend class Process;
	Page(){};
	Page(const bitset<6>& virtual_adress, const size_t& content);
	bitset<6> getVirtualAdress() const{ return m_virtual_adress; }
	size_t getContent()const{ return m_content; }
	~Page(){};
private:
	bitset<6> m_virtual_adress;
	size_t m_content;
};

