#include "Page.h"

size_t Page::PAGE_NUM_LENGTH = 4;

Page::Page(const bitset<32>& block )
:m_block(block)
{

}


Page::~Page()
{
    
}
