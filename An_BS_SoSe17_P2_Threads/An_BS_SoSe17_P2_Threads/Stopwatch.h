#pragma once

#include <chrono>


class Stopwatch
{
private:
	// note usage of new C++11 { initializations }
	typedef std::chrono::steady_clock		clock;

	long long int		numberOfMeasurements{ 0 };
	clock::duration		totalTimeMeasured{ 0 };
	clock::time_point	startPoint;

public:
	// constructor
	Stopwatch() {
		startMeasuring(); // just to be sure lastPoint has a meaningful value
	};

	// main methods
	void startMeasuring() 		{
		startPoint = clock::now();
	}

	std::chrono::nanoseconds takeMeasurement() 	{
		totalTimeMeasured += (clock::now() - startPoint);
		numberOfMeasurements++;
		return(totalTimeMeasured);
	};

	// return value is in seconds
	double averageMeasurement() const {
		// protect against division by zero
		if (0 == numberOfMeasurements) return(0.0);

		std::chrono::duration< double >		totalSeconds(totalTimeMeasured);
		return (totalSeconds.count() / (double)numberOfMeasurements);
	};

	long long int count() const { return numberOfMeasurements; }

}; // end class stopwatch


