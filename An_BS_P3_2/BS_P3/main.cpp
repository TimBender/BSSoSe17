#include"Process.h"
#include<vector>

using namespace std;
int main(int argc, char** argv)
{
	array<unsigned char, 1000> RAM;	//	capacity: 1KB
	vector<Process> m_processes(2);

	for (size_t i = 0; i < m_processes.size(); i++){
		m_processes[i] = Process();
	}




	return 0;
}