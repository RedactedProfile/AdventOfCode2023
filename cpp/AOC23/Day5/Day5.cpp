#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
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
#define MAP_PREALLOCATE 5
#else 
#define FILENAME "input.txt"
#define MAP_PREALLOCATE 15
#endif


// Macros 
#ifdef NDEBUG
#define DEBUG_LOG(message) ((void)0)
#else 
#define DEBUG_LOG(message) std::cout << message << std::endl
#endif


//////////////////////////////////////////////////////////////

class RangeMap {
public:
    
    uint32_t start, end;

    RangeMap() {}

    RangeMap(uint32_t _start, uint32_t _length) {
        start = _start;
        end = _start + _length - 1;
    }

    bool in(uint32_t val) {
        return start >= val && end <= val;
    }
};


enum MapParseMode
{
    NONE,
    SEED,
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION
};

std::vector<uint32_t> Seeds;
std::vector<std::pair<RangeMap, RangeMap>> Soil;
std::vector<std::pair<RangeMap, RangeMap>> Fertilizer;
std::vector<std::pair<RangeMap, RangeMap>> Water;
std::vector<std::pair<RangeMap, RangeMap>> Light;
std::vector<std::pair<RangeMap, RangeMap>> Temperature;
std::vector<std::pair<RangeMap, RangeMap>> Humidity;
std::vector<std::pair<RangeMap, RangeMap>> Location;

MapParseMode current_parse_mode = MapParseMode::NONE;

void handle_feed_map(std::vector<std::pair<RangeMap, RangeMap>>& map, std::vector<uint32_t> values)
{
    map.push_back({ {values[0], values[2]}, {values[1], values[2]} });
}

std::vector<uint32_t> handle_collect_values(std::string line) {
    std::istringstream iss(line);
    std::vector<uint32_t> values;

    std::string token;
    while (std::getline(iss, token, ' ')) {
        uint32_t num = std::stoul(token);
        values.push_back(num);
    }

    return values;
}

// The input ALWAYS starts with the seed line that awkwardly doesn't follow the same format as everything else by keeping the mode identifier in the same line as the values
void handle_seeds(std::string line) {
    auto token = std::string("seeds: ");
    line = line.substr(token.size(), line.size() - token.size());
    // we should have just a line of numbers now
    auto values = handle_collect_values(line);
    DEBUG_LOG(line);
    Seeds = values;
}

void line_parser(std::string line, int line_num)
{
    DEBUG_LOG(line);

    if (line.size() <= 0) {
        return; // line is empty, ignore
    }

    if (line.starts_with("seeds:")) {
        current_parse_mode = MapParseMode::SEED;
    }
    else if (line.starts_with("seed-to-soil")) {
        current_parse_mode = MapParseMode::SOIL;
        return;
    }
    else if (line.starts_with("soil-to-fertilizer")) {
        current_parse_mode = MapParseMode::FERTILIZER;
        return;
    }
    else if (line.starts_with("fertilizer-to-water")) {
        current_parse_mode = MapParseMode::WATER;
        return;
    }
    else if (line.starts_with("water-to-light")) {
        current_parse_mode = MapParseMode::LIGHT;
        return;
    }
    else if (line.starts_with("light-to-temperature")) {
        current_parse_mode = MapParseMode::TEMPERATURE;
        return;
    }
    else if (line.starts_with("temperature-to-humidity")) {
        current_parse_mode = MapParseMode::HUMIDITY;
        return;
    }
    else if (line.starts_with("humidity-to-location")) {
        current_parse_mode = MapParseMode::LOCATION;
        return;
    }
    

    switch (current_parse_mode)
    {
    case NONE:
        break;
    case SEED:
        handle_seeds(line);
        break;
    case SOIL:
        handle_feed_map(Soil, handle_collect_values(line));
        break;
    case FERTILIZER:
        handle_feed_map(Fertilizer, handle_collect_values(line));
        break;
    case WATER:
        handle_feed_map(Water, handle_collect_values(line));
        break;
    case LIGHT:
        handle_feed_map(Light, handle_collect_values(line));
        break;
    case TEMPERATURE:
        handle_feed_map(Temperature, handle_collect_values(line));
        break;
    case HUMIDITY:
        handle_feed_map(Humidity, handle_collect_values(line));
        break;
    case LOCATION:
        handle_feed_map(Location, handle_collect_values(line));
        break;
    default:
        break;
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
    //Seeds = std::vector<uint32_t>(MAP_PREALLOCATE);
    //Soil = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Fertilizer = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Water = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Light = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Temperature = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Humidity = std::vector<RangeMap>(MAP_PREALLOCATE);
    //Location = std::vector<RangeMap>(MAP_PREALLOCATE);

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