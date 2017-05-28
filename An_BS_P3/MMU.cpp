#include "MMU.h"

size_t MMU::PAGE_ERROR_COUNTER = 0;

MMU::MMU()
:m_OS(OS::getInstance())
{

}

/**Uebersetzt virtuelle Adressen in physische Adressen ???
   + verwendet die aktuelle Seitentabelle (= ST des aktuellen Prozesses)*/
void MMU::translateAddress(Page& page)
{
	/*Versuche Adresse zu uebersetzen*/

	/*Uebersetzung fehlgeschlagen: Rufe OS auf*/
	PAGE_ERROR_COUNTER++;
}

void MMU::setCurrentPageChart(const multimap < Page, bool>& table)
{
	m_current_page_table = table;
}

MMU::~MMU()
{
}
