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
#define DAY "7"
#define SLOGAN "Camel Cards"
#define USE_SAMPLE false
#define DO_PART_1 true 
#define DO_PART_2 false


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


enum HAND_TYPE {
    FIVE,
    FOUR,
    FULL,
    THREE,
    TWO,
    ONE,
    HIGH,
    NONE
};
struct Hand
{   
    std::string cards;
    int bid;

    HAND_TYPE result = HAND_TYPE::NONE;

    void eval_hand() {
        // update result with the type of hand this is
        std::unordered_map<char, int> collection;
        for (auto card : cards) {
            if (!collection.contains(card)) collection[card] = 1;
            else collection[card]++;
        }

        if (collection.size() == 1) {
            result = HAND_TYPE::FIVE;
        }
        else if (collection.size() == 5) {
            result = HAND_TYPE::HIGH;
        }
        else if (collection.size() == 2) {
            result = HAND_TYPE::FULL;
        }
        else if (collection.size() == 3) {
            // two pair or three of a kind?
            for (const auto& pair : collection) {
                if (pair.second == 3) {
                    result = HAND_TYPE::THREE;
                    break;
                }
            }
            if (result == HAND_TYPE::NONE) {
                result = HAND_TYPE::TWO;
            }
        }
        else if (collection.size() == 4) {
            result = HAND_TYPE::ONE;
        }
    }
};


std::vector<Hand> hands;
////////////// Parsing Functions ////////////////

/// <summary>
/// Handles parsing of each individual line of the puzzle input
/// </summary>
/// <param name="line"></param>
/// <param name="line_num"></param>
void line_parser(std::string line, int line_num)
{
    DEBUG_LOG(line);

    auto parts = str_split(str_normalize_whitespace(str_trim(line)), ' ');
    Hand hand = { parts[0], std::stoi(parts[1]) };
    hand.eval_hand();

    hands.push_back(hand);
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
        // for part 1 
        score = 0;

        std::cout << "Part 1 Score: " << score << std::endl;
    }
#endif // DO_PART_1

#if DO_PART_2
    // Part 2
    {
        // for part 2 
        score = 0;

        std::cout << "Part 2 Score: " << score << std::endl;
    }
#endif
}