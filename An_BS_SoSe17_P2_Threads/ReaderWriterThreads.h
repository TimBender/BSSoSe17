#pragma once


/* READER WRITER PROBLEM: reader preference
=========================================*/

// The writer thread
void writer(int writerID, int numSeconds);

// The reader thread
void reader(int readerID, int numSeconds);


/* READER WRITER PROBLEM: fair treatment
=========================================*/

// The writer thread
void fairWriter(int writerID, int numSeconds);

// The reader thread
void fairReader(int readerID, int numSeconds);


