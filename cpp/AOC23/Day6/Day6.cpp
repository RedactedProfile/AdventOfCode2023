#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <ranges>
#include <ExecutionTime.h>
#include <StringTools.hpp>


// Configuration 
#define DAY "6"
#define SLOGAN "Wait For It"
#define USE_SAMPLE false
#define DO_PART_1 true 
#define DO_PART_2 false

#define TOKEN_JUMP 11

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

////////////// Enums ////////////////////////////


////////////// Structs and Classes //////////////
struct Race
{
    int time, distance;

    int simulate_winners() {
        DEBUG_LOG("--------------------------");
        DEBUG_LOG("     Total Time: " << time << "ms");
        DEBUG_LOG("Record Distance: " << distance << "mm");
        int winners = 0;
        for (int held = 1; held < time; ++held) {
            int simulated_distance = 0;

            int speed = held; // our speed is 1mm per ms for each ms the button is held
            simulated_distance = speed * (time - held);

            DEBUG_LOG("-- " << held << "ms held = " << simulated_distance << "mm traveled");

            if (simulated_distance > distance) winners++;
        }

        return winners;
    }
};

std::vector<Race> races;

////////////// Parsing Functions ////////////////

/// <summary>
/// Handles parsing of each individual line of the puzzle input
/// </summary>
/// <param name="line"></param>
/// <param name="line_num"></param>
void line_parser(std::string line, int line_num)
{
    DEBUG_LOG(line);

    int cursor = 0;
    char mode = line.at(0);
    auto values = str_split_to_int(str_normalize_whitespace(str_trim(line.substr(TOKEN_JUMP, line.size() - TOKEN_JUMP))), ' ');
    for (int val : values) {
        if (mode == 'T') {
            races.push_back({ val, 0 });
        }
        else if (mode == 'D') {
            races[cursor].distance = val;
        }
        cursor++;
    }

    
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

    std::cout << "Advent of Code 2023, Day " << DAY << ": " << SLOGAN << "." << std::endl;
    std::cout << "========================================" << std::endl;

    build_cache();

#if DO_PART_1
    // Part 1, 
    {
        // for part 1 we simply find how many winning moves there are and multiply them all together
        score = 1;

        for (auto race : races)
        {
            score *= race.simulate_winners();
        }

        std::cout << "Part 1 Score: " << score << std::endl;
    }
#endif // DO_PART_1

#if DO_PART_2
    // Part 2
    {
        score = 0;

        std::cout << "Part 2 Score after considering " << iters << " seeds: " << score << std::endl;
    }
#endif
}