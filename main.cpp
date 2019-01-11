#include <iostream>
#include <array>
#include <ctime> // for time()
#include <cstdlib> // for rand() and srand()
#include <cassert> // for assert()
#include "wumpus.h"

using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // If using Visual Studio, discard first random value
	/*
	try {
		wumpus::hunt_wumpus();
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << "\n";
		std::cin.get();
	}
	catch (...) {
		std::cerr << "unknown error\n";
		std::cin.get();
	}
	*/
	
	try
	{
		wumpus::hunt_wumpus();
	}
	catch (std::exception&)
	{
		std::cout << "Something went wrong!!!.\n";
		std::cin.get();
	}
	
	return 0;
}