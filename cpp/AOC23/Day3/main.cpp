#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <ExecutionTime.h>

int main()
{
	ExecutionTimer<std::chrono::milliseconds> timer;

	std::cout << "Advent of Code 2023, Day 3: Gear Ratios." << std::endl;
	std::cout << "========================================" << std::endl;

	std::cout << "Loading input.." << std::endl;

	std::ifstream file("../../../_puzzle_input/day3/input_sample.txt");


	// Phase 1: Read the data from file
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {

			std::cout << line << std::endl;

		}
	}

	file.close();

}