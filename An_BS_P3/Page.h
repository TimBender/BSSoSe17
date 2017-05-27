#pragma once

#include<array>

using namespace std;
class Page
{
public:
	friend class Prozess;
	Page(const size_t& id);
	size_t getId()const{ return m_id; }
	~Page();
private:
	array<char, 8> m_block;
	size_t m_id;
};

