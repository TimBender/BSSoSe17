#pragma once

#include<vector>

using namespace std;
class Page
{
public:
	static size_t BLOCK_SIZE;
	friend class Process;
	Page(const size_t& id);
	size_t getId()const { return m_id; }
	vector<char> getBlock() const { return m_block; }
	~Page();
private:
	vector<char> m_block;
	size_t m_id;
};

