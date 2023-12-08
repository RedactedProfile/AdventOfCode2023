#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ranges>
#include <ExecutionTime.h>


void line_parser(std::string line, int line_num)
{
	std::cout << line << std::endl;
}

void read_file(std::string filename, void (*parser)(std::string line, int line_num))
{
	std::cout << "Loading input.." << std::endl;

	std::ifstream file(filename);

	// Phase 1: Read the data from file
	int line_num = 0;
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {
			parser(line, line_num);
			line_num++;
		}
	}

	file.close();
}

void build_cache()
{
	read_file("../../../_puzzle_input/day4/input_sample.txt", line_parser);
}

int main()
{
	ExecutionTimer<std::chrono::milliseconds> timer;

	std::cout << "Advent of Code 2023, Day 4: Scratchcards." << std::endl;
	std::cout << "========================================" << std::endl;

	build_cache();

}