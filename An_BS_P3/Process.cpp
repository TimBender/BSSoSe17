#include "Process.h"
#include<iostream>

size_t Process::PAGE_COUNTER = 1;
size_t Process::ID = 1;
Process::Process()
:m_id(ID++)
{

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
