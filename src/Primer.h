#pragma once

#include <chrono>
#include <vector>

class Primer
{
private:
	const static unsigned long MAX_PRIME_NUM = 203280221UL;

	std::chrono::high_resolution_clock::time_point startTime;
	std::vector<unsigned long> foundPrimes;
	unsigned long currNum;

	bool runOnce();
	long readLong();
	long calculatePrime(unsigned long num);
	void calculateNextPrime(unsigned long num);
	void printPrime(unsigned long num, unsigned long prime, double rate);
	unsigned long nextPrime();

public:
	Primer();
	void run();
};
