#pragma once
#include<bitset>
#include<vector>

using namespace std;
class Page
{
public:
	static size_t PAGE_NUM_LENGTH;
	static size_t BLOCK_SIZE;
	friend class Process;
	Page(){};
	Page(const bitset<32>& block);
	size_t getId()const { return m_id; }
	bitset<32> getBlock() const { return m_block; }
	~Page();
private:
	bitset<32> m_block;
	size_t m_id;
};

