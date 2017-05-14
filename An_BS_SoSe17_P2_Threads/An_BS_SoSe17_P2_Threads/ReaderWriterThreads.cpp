#include "ReaderWriterThreads.h"
#include "Database.h"
#include <stdio.h>
#include <iostream>			// for std::cin, cerr, cout ...
#include <thread>			// for std::this_thread
#include <chrono>			// for std::chrono... 
#include<mutex>

typedef int semaphore;

std::mutex shared_mutex_db;
std::mutex rc_mutex;
//semaphore rc_mutex = 1;
//semaphore db = 1;
int rc = 0;
//QJPBG-JCN2K-X2YGV-C9HXY-7MJQF
// ******** reader & writer threads ******** 

// The writer thread
void writer(int writerID, int numSeconds) {

	std::cout << "Writer " << writerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);

	//timeslot not exhausted 
	while ((std::chrono::steady_clock::now() - startTime) < maxTime) 
	{ 
		sem_wait(&db);
		shared_mutex_db.lock(); //writer braucht exklusiven Zugriff
		bool result = theDatabase.write(writerID);
		shared_mutex_db.unlock(); //exklusiven Zugriff wieder freigeben

		++tests;

		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "WRITER " << writerID << '\n';
		std::cout << "_________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result == true ? "correct" : "incorrect") << '\n'
			<< "sleeping " << numSeconds2sleep 
			<< " seconds " << "\n_________________________________\n\n";
		//if (numSeconds2sleep) std::this_thread::sleep_for(randomSeconds);
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));
	} // repeat until time used is up

	std::cout << "\n== WRITER " << writerID 
		<< " finished == \n" <<"Returning after: " << tests
		<< " tests." << std::endl << std::endl;

} // end writer function

// The reader thread
void reader(int readerID, int numSeconds) {

	std::cout << "Reader " << readerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);
	while ((std::chrono::steady_clock::now() - startTime) < maxTime) {
		rc_mutex.lock(); //exklusiver Zugriff fuer reader counter/rc
		rc++;
		if (rc == 1) shared_mutex_db.lock(); //1.Reader => lock DB
		rc_mutex.unlock();

		//Read from database
		bool result = theDatabase.read(readerID);
		++tests;
	
		rc_mutex.lock(); //exklusiver Zugriff auf reader counter
		rc--;
		if (rc == 0) shared_mutex_db.unlock(); //unlock DB, when last reader thread has finished
		rc_mutex.unlock();
		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "READER " << readerID << "\n________________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result==true ? "correct" : "incorrect" )<< '\n'
			<< "sleeping " << numSeconds2sleep 
			<< " seconds " <<"\n________________________________________\n\n";
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));

	} // repeat until time is used up

	std::cout << "\n== READER " << readerID
		<< " finished. ==\nReturning after " << tests
		<< " tests." << std::endl << std::endl;

} // end reader function


