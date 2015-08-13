#include "Primer.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

bool Primer::runOnce()
{
	long num = 0;

	while (true)
	{
		std::cout << "Enter target prime (0 to exit, -1 for max): ";

		try
		{
			num = readLong();

			if (num < -1)
			{
				std::cout << "Negative numbers invalid, try again" << std::endl;
				continue;
			}

			break;
		}
		catch (std::exception&)
		{
			std::cout << "Invalid number, try again" << std::endl;
		}
	}

	if (num == 0)
	{
		return true;
	}
	else if (num == -1)
	{
		num = MAX_PRIME_NUM;
	}

	long prime = calculatePrime(num - 1);
	if (prime == 0)
	{
		std::cout << "Prime to large, try smaller number" << std::endl;
	}
	else
	{
		std::cout << "Prime #" << std::setw(10) << num << ": " << std::setw(10) << prime << std::endl;
	}

	return false;
}

long Primer::readLong()
{
	std::string input;
	std::getline(std::cin, input);

	return std::stoul(input);
}

long Primer::calculatePrime(unsigned long num)
{
	if (num > MAX_PRIME_NUM)
	{
		std::cout << "Number to big, max is " << MAX_PRIME_NUM << std::endl;
		return 0;
	}

	if (num >= foundPrimes.size())
	{
		std::cout << "Prime #" << std::setw(10) << num + 1 << " not known. Calculating from #" << std::setw(10) << foundPrimes.size() << std::endl;
		try
		{
			if (num >= MAX_PRIME_NUM / 2)
			{
				foundPrimes.reserve(MAX_PRIME_NUM);
			}
			else
			{
				foundPrimes.reserve(num + 1);
			}
		}
		catch (std::bad_alloc& ex)
		{
			fprintf(stderr, "Failed to allocate more memory: %s\n", ex.what());
			return 0;
		}

		startTime = std::chrono::high_resolution_clock::now();

		try
		{
			for (unsigned long i = foundPrimes.size(); i <= num; i++)
			{
				calculateNextPrime(i);
			}
		}
		catch (std::exception& ex)
		{
			fprintf(stderr, "Failed to calculate prime: %s\n", ex.what());
			return 0;
		}
	}

	return foundPrimes[num];
}

void Primer::calculateNextPrime(unsigned long num)
{
	unsigned long prime = nextPrime();
	const static unsigned int PRINT_PERIOD = 100000;
	if (num % PRINT_PERIOD == PRINT_PERIOD - 1)
	{
		std::chrono::high_resolution_clock::time_point stopTime = std::chrono::high_resolution_clock::now();
		double rate = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(stopTime - startTime).count() / (double)PRINT_PERIOD;
		startTime = stopTime;

		printPrime(num, prime, rate);
	}
}

void Primer::printPrime(unsigned long num, unsigned long prime, double rate)
{
	std::cout << "Prime #" << std::setw(10) << num + 1 << ": " << std::setw(10) << prime << " at " << std::setw(5) << std::fixed << std::setprecision(1) << rate << " us/prime" << std::endl;
}

unsigned long Primer::nextPrime()
{
	bool primeFound = false;
	while (!primeFound)
	{
		if (currNum == std::numeric_limits<unsigned long>::max())
		{
			std::ostringstream ss;
			ss << "Max reached (" << currNum << ")." << std::endl;

			throw std::exception(ss.str().c_str());
		}

		currNum++;

		primeFound = true;
		for (unsigned long n : foundPrimes)
		{
			if (currNum % n == 0)
			{
				primeFound = false;
				break;
			}

			if (n * n > currNum)
			{
				break;
			}
		}
	}

	foundPrimes.push_back(currNum);

	return currNum;
}

Primer::Primer()
	: currNum(1)
{
}

void Primer::run()
{
	bool end = false;
	while (!end)
	{
		end = runOnce();
	}
}
