#pragma once

#include <semaphore.h>
/*semaphores for fair treatment*/
extern sem_t fair_mutex;							//for fair_rc  
extern sem_t fair_db;								//access control on database
extern sem_t fifoQueue;							//preserves right order
extern int fair_rc;								//reader counter

/* READER WRITER PROBLEM: reader preference
=========================================*/

// The writer threads
void writer(int writerID, int numSeconds);

// The reader thread
void reader(int readerID, int numSeconds);


/* READER WRITER PROBLEM: fair treatment
=========================================*/

// The writer thread
void fairWriter(int writerID, int numSeconds);

// The reader thread
void fairReader(int readerID, int numSeconds);


