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


// Configuration 
#define DAY "5"
#define SLOGAN "If You Give A Seed A Fertilizer"
#define USE_SAMPLE true
#define DO_PART_1 false 
#define DO_PART_2 true

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
        return val >= start && val <= end;
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

struct PathMap
{
    uint32_t 
        seed, 
        soil, 
        fertilizer, 
        water, 
        light, 
        temperature, 
        humidity, 
        location;
};

uint32_t handle_map(std::vector<std::pair<RangeMap, RangeMap>>& map, uint32_t value) {
    uint32_t result = value; // default value that might be overridden
    for (int i = 0; i < map.size(); ++i) {
        if (map[i].second.in(value) == true) {
            result = map[i].first.start + (value - map[i].second.start);
            break;
        }
    }
    return result;
}

PathMap handle_find_seed_location(uint32_t seed) {
    // we'll brute force this for now, maybe there's a better faster way using loops or something 
    
    // seed -> Soil -> Fert -> Water -> Light -> Temp -> Hum -> Location
    PathMap result      = PathMap();
    result.seed         = seed;
    result.soil         = handle_map(Soil, result.seed);
    result.fertilizer   = handle_map(Fertilizer, result.soil);
    result.water        = handle_map(Water, result.fertilizer);
    result.light        = handle_map(Light, result.water);
    result.temperature  = handle_map(Temperature, result.light);
    result.humidity     = handle_map(Humidity, result.temperature);
    result.location     = handle_map(Location, result.humidity);


    return result;
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

    // fun note, the cache for the real input is only 64kb of memory :)

    // fun note, dont forget that the in the RangeMap pairs, the FIRST is the DESTINATION, and the SECOND is the SOURCE

#if DO_PART_1
    // Part 1, 
    {
        // We just need to loop through the seeds, find the location number of each seed, and report the lowest location number

        score = 0;
        
        std::vector<uint32_t> locations(Seeds.size());
        for (int i = 0; i < Seeds.size(); ++i) {
            auto r = handle_find_seed_location(Seeds[i]);
            locations[i] = r.location;
        }
        std::sort(locations.begin(), locations.end());

        score = locations[0];
        

        std::cout << "Part 1 Score: " << score << std::endl;
    }
#endif // DO_PART_1

#if DO_PART_2
    // Part 2
    {
        // This is the exact same puzzle, except that the seeds are actually in integer pairs denoting a range. 
        // That's a lot of traversal for every seed (could be millions) 
        // attempt 1: brute force it, just loop through the range of each pair and collect locations

        score = 0;

        // attempt 1
        uint32_t lowest_location = UINTMAX_MAX;
        int iters = 0;
        for (int i = 0; i < Seeds.size(); i += 2) { // first collect all the pairs
            for (uint32_t s = Seeds[i]; s < Seeds[i] + Seeds[i + 1]; ++s) {
                auto r = handle_find_seed_location(s);
                if (r.location < lowest_location) {
                    lowest_location = r.location;
                }
                iters++;
            }
        }

        score = lowest_location;


        std::cout << "Part 2 Score: " << score << std::endl;
    }
#endif
}