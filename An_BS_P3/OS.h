#pragma once
#include"Page.h"

#include<list>

using namespace std;
class OS
{
public:
	/*Singleton*/
	static OS& getInstance()
	{
		static OS m_instance;
		return m_instance;
	}

	OS(const OS&) = delete;
	OS operator=(const OS&) = delete;
	~OS();
private:
	OS(){};

	/**Seitenvergabe bzw. - falls keine Seite frei ist - Seitenersetzung???*/
	list<Page> m_pages;
};

