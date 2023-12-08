#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ranges>
#include <ExecutionTime.h>


// Configuration 
#define DAY "5"
#define SLOGAN "If You Give A Seed A Fertilizer"
#define USE_SAMPLE true


// Conditional Configuration
#if USE_SAMPLE
#define FILENAME "input_sample.txt"
#else 
#define FILENAME "input.txt"
#endif


// Macros 
#ifdef NDEBUG
#define DEBUG_LOG(message) ((void)0)
#else 
#define DEBUG_LOG(message) std::cout << message << std::endl
#endif


//////////////////////////////////////////////////////////////


void line_parser(std::string line, int line_num)
{
    std::cout << line << std::endl;

}

void read_file(std::string filename, void (*parser)(std::string line, int line_num))
{
    std::cout << "Loading input.." << filename << std::endl;

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
    std::string filepath = "../../../_puzzle_input/day";
    filepath += DAY;
    filepath += "/";
    filepath += FILENAME;
    read_file(filepath, line_parser);
}

int main()
{
    ExecutionTimer<std::chrono::milliseconds> timer;
    int score, i, c, m;

    std::cout << "Advent of Code 2023, Day " << DAY << ": " << SLOGAN <<"." << std::endl;
    std::cout << "========================================" << std::endl;

    build_cache();

    // Part 1, 
    {
        score = 0;
        

        std::cout << "Part 1 Score: " << score << std::endl;
    }

    // Part 2
    {
        score = 0;

        std::cout << "Part 2 Score: " << score << std::endl;
    }
}