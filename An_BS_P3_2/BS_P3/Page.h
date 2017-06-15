#pragma once
#include<bitset>
#include<string>

using namespace std;
class Page
{
public:
	Page(){};
	Page(const bitset<8> virtual_adress, const string& content);
	bitset<8> getVirtualAdress() const{ return m_virtual_adress; }
	string getContent()const{ return m_content; }
	~Page();

private:
	bitset<8> m_virtual_adress;
	string m_content;
};

