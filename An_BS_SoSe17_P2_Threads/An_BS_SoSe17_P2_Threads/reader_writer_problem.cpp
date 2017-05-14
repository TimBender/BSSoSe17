#include"ReaderWriterThreads.h"
#include "Database.h"

#include<thread> //thread bib
#include<vector>
#include<iostream>
#include<stdexcept>

using namespace std;


int main(int argc, char** argv)
{
	const size_t NUM_OF_THREADS = 4; //number of writing threads
	int r_counter = 1, w_counter = 1, MAX_TIME = 20;

	/*first test round: 9:1 ratio (r_threads:w_threads)*/

	vector<thread> r_threads(NUM_OF_THREADS * 9); //reading function
	vector<thread> w_threads = vector<thread>(NUM_OF_THREADS); //writing function

	/*second test round: 1:1 ratio (r_threads:w_threads)*/

	//vector<thread> r_thread = vector<thread>(NUM_OF_THREADS, thread(reader)); //reading function
	//vector<thread> w_thread = vector<thread>(NUM_OF_THREADS, thread(writer)); //writing function

	/*third test round: 1:9 ratio (r_threads:w_threads)*/

	//vector<thread> r_thread = vector<thread>((NUM_OF_THREADS), thread(reader)); //reading function
	//vector<thread> w_thread = vector<thread>((NUM_OF_THREADS* 9), thread(writer)); //writing function


	//push reader threads into verctor
	for (auto &r_thread : r_threads){
		r_thread = thread(reader, r_counter++, MAX_TIME);
	}

	//push writer threads into verctor
	for (auto &w_thread : w_threads){
		thread(writer, w_counter++, MAX_TIME);
	}




	// wait for all threads to terminate
	try{
		for (auto &current_thread : r_threads){
			current_thread.join();
		}

		/*for (auto &current_thread : w_thread){
			current_thread.join();
		}*/
		cout << "All thread executions are completed.\n";
	}
	catch (const invalid_argument & eo){
		cerr << "thread object is not joinable: " << eo.what() << "\n";
	}
	return 0;
}