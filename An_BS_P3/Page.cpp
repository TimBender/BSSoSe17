#include "Page.h"

size_t Page::BLOCK_SIZE = 8;

Page::Page(const size_t& id)
:m_id(id), m_block(vector<char>(BLOCK_SIZE))
{
}


Page::~Page()
{
}
