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
#include <DirectXMath.h>

using namespace DirectX;


// Configuration 
#define DAY "10"
#define SLOGAN "Pipe Maze"
#define USE_SAMPLE false
#define DO_PART_1 true 
#define DO_PART_2 false


// Conditional Configuration
#if USE_SAMPLE
#define FILENAME "input_sample_simple.txt"
#else 
#define FILENAME "input.txt"
#endif


// Macros 
#ifdef NDEBUG
#define DEBUG_LOG(message) ((void)0)
#else 
#define DEBUG_LOG(message) std::cout << message << std::endl
#endif

/////////////// Notes ///////////////////////////
/*
The approach we're going to try here is \
1. instead of parsing line by line, we're going to simply load the whole text file into memory
2. create a simple struct with an ID, an enumaerated type, a 0-index coordinate pair, a 1-index coordinate pair, and a input refernce and an output reference. 
3. We need to find out what piece the starting point is, but we'll store that reference as a special global
4. Using this data, we should be able to walk from the starting coordinate following outputs all the way back to the startign coordinate. This will assert the loop is correct.
5. With the loop verified, for part 1 to be satisfied, we need to determine the furthest part of the loop.
    So my thought here is, we'll walk the loop twice, and find the spot where the highest value is equal on both directions
*/



////////////// Enums ////////////////////////////
enum TYPE : char
{
    STRAIGHT_VERT = '|',
    STRAIGHT_HORZ = '-',
    CORNER_BL = 'L',
    CORNER_BR = 'J',
    CORNER_TR = '7',
    CORNER_TL = 'F',
    GROUND = '.'
};

enum DIRECTION {
    ABOVE,
    BELOW,
    LEFT,
    RIGHT
};


////////////// Structs and Classes //////////////
struct Pipe {
    inline static int lastId = 0;

    int id;
    TYPE type = TYPE::GROUND;
    std::pair<int, int> point0, point1;
    Pipe *in, *out;

    Pipe() : in(nullptr), out(nullptr) {
        id = ++lastId;
    }
};

std::map<std::pair<int, int>, Pipe*> map;


Pipe* starter = nullptr;
////////////// Parsing Functions ////////////////

TYPE typeLookup(const char c) {
    if (TYPE::STRAIGHT_VERT == c) return TYPE::STRAIGHT_VERT;
    else if (TYPE::STRAIGHT_HORZ == c) return TYPE::STRAIGHT_HORZ;
    else if (TYPE::CORNER_BL == c) return TYPE::CORNER_BL;
    else if (TYPE::CORNER_BR == c) return TYPE::CORNER_BR;
    else if (TYPE::CORNER_TR == c) return TYPE::CORNER_TR;
    else if (TYPE::CORNER_TL == c) return TYPE::CORNER_TL;
    else return TYPE::GROUND;
}

void build_relationships() {

    for (auto pipe : map)
    {
        if (pipe.second->id == 7576) {
            DEBUG_LOG("asd");
        }
        std::pair<int, int> poss_in, poss_out;
        std::vector<TYPE> compat_in, compat_out;

        // TODO: may need to worry about direction of in vs out, and this is probably the place we'd do it
        // on the other hand, pipes really dont care about flow direction, maybe calling them in and out was wrong and confusing
        
        switch (pipe.second->type)
        {
        case STRAIGHT_HORZ: // '-' =  ::  x + 1, x - 1
            poss_in = { pipe.first.first - 1, pipe.first.second };
            poss_out = { pipe.first.first + 1, pipe.first.second };

            //                  =[=]                    ╚[=]                ╔[=]
            compat_in = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_BL, TYPE::CORNER_TL };

            //                  [=]=                    [=]╝                [=]╗
            compat_out = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_BR, TYPE::CORNER_TR };
            break;

        case STRAIGHT_VERT: // '|' ║  ::  y - 1, y + 1
            poss_in = { pipe.first.first, pipe.first.second - 1 };
            poss_out = { pipe.first.first, pipe.first.second + 1 };

            //                  ║                    ╔                   ╗
            //                 [║]                  [║]                 [║]
            compat_in = { TYPE::STRAIGHT_VERT, TYPE::CORNER_TL, TYPE::CORNER_TR };

            //                 [║]                  [║]                 [║]
            //                  ║                    ╚                   ╝
            compat_out = { TYPE::STRAIGHT_VERT, TYPE::CORNER_BL, TYPE::CORNER_BR };
            break;

        case CORNER_BL: // 'L' ╚  ::  y - 1, x + 1
            poss_in = { pipe.first.first, pipe.first.second - 1 };
            poss_out = { pipe.first.first + 1, pipe.first.second };

            //                  ║                    ╔                   ╗
            //                 [╚]                  [╚]                 [╚]
            compat_in = { TYPE::STRAIGHT_VERT, TYPE::CORNER_TL, TYPE::CORNER_TR };

            //                 [╚]=                 [╚]╝                [╚]╗                   
            compat_out = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_BR, TYPE::CORNER_TR };
            break;

        case CORNER_BR: // 'J' ╝ ::  y - 1, x - 1
            poss_in = { pipe.first.first, pipe.first.second - 1 };
            poss_out = { pipe.first.first - 1, pipe.first.second };
            
            //                =[╝]                 ╔[╝]                ╚[╝]                   
            compat_in = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_TL, TYPE::CORNER_BL };

            //                  ║                    ╔                   ╗
            //                 [╝]                  [╝]                 [╝]
            compat_out = { TYPE::STRAIGHT_VERT, TYPE::CORNER_TL, TYPE::CORNER_TR };
            break;

        case CORNER_TL: // 'F' ╔ ::  y + 1, x + 1
            poss_out = { pipe.first.first, pipe.first.second + 1 };
            poss_in = { pipe.first.first + 1, pipe.first.second };
        
            //                 [╔]                  [╔]                 [╔]
            //                  ║                    ╚                   ╝
            compat_out = { TYPE::STRAIGHT_VERT, TYPE::CORNER_BL, TYPE::CORNER_BR };

            //                 [╔]=                 [╔]╝                [╔]╗                   
            compat_in = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_BR, TYPE::CORNER_TR };
            break;

        case CORNER_TR: // '7' ╗ ::  y + 1, x - 1
            poss_out = { pipe.first.first, pipe.first.second + 1 };
            poss_in = { pipe.first.first - 1, pipe.first.second };

            //                 [╗]                  [╗]                 [╗]
            //                  ║                    ╚                   ╝
            compat_out = { TYPE::STRAIGHT_VERT, TYPE::CORNER_BL, TYPE::CORNER_BR };

            //                =[╗]                 ╔[╗]                ╚[╗]                   
            compat_in = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_TL, TYPE::CORNER_BL };
            break;

        case GROUND:
        default:
            break;

        }

        // check the in and out coordinate pairs to see if anything is there 
        if (map.contains(poss_in) && std::find(compat_in.begin(), compat_in.end(), map[poss_in]->type) != compat_in.end()) {
            pipe.second->in = map[poss_in];
            //map[poss_in]->out = pipe.second;
        }


        if (map.contains(poss_out) && std::find(compat_out.begin(), compat_out.end(), map[poss_out]->type) != compat_out.end()) {
            pipe.second->out = map[poss_out];
            //map[poss_out]->in = pipe.second;
        }
    }
}


bool checkCompatPos(std::pair<int, int> pos, DIRECTION dir)
{
    std::vector<TYPE> compat;
    switch (dir)
    {
    case ABOVE:
        compat = { TYPE::STRAIGHT_VERT, TYPE::CORNER_TL, TYPE::CORNER_TR };
        break;
    case BELOW:
        compat = { TYPE::STRAIGHT_VERT, TYPE::CORNER_BL, TYPE::CORNER_BR };
        break;
    case LEFT:
        compat = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_TR, TYPE::CORNER_BL };
        break;
    case RIGHT:
        compat = { TYPE::STRAIGHT_HORZ, TYPE::CORNER_TL, TYPE::CORNER_BR };
        break;
    default:
        break;
    }

    return map.find(pos) != map.end() && std::find(compat.begin(), compat.end(), map[pos]->type) != compat.end();
}

void convert_starter() {
    // we need to figure out what kind of pipe this is
    std::pair<int, int> pos = starter->point0,
        above = pos,
        below = pos,
        left = pos,
        right = pos;

    above.second--;
    below.second++;
    left.first--;
    right.first++;

    bool compat_up    = map.find(above) != map.end() && checkCompatPos(above, DIRECTION::ABOVE),
         compat_down  = map.find(below) != map.end() && checkCompatPos(below, DIRECTION::BELOW),
         compat_left  = map.find(left) != map.end() && checkCompatPos(left, DIRECTION::LEFT),
         compat_right = map.find(right) != map.end() && checkCompatPos(right, DIRECTION::RIGHT);

    if      (compat_up && compat_down)    { starter->type = TYPE::STRAIGHT_VERT; }
    else if (compat_left && compat_right) { starter->type = TYPE::STRAIGHT_HORZ; }
    else if (compat_up && compat_right)   { starter->type = TYPE::CORNER_BL; }
    else if (compat_up && compat_left)    { starter->type = TYPE::CORNER_BR; }
    else if (compat_left && compat_down)  { starter->type = TYPE::CORNER_TR; }
    else if (compat_down && compat_right) { starter->type = TYPE::CORNER_TL; }
}

/// <summary>
/// Handles parsing of each individual line of the puzzle input
/// </summary>
/// <param name="line"></param>
/// <param name="line_num"></param>
void line_parser(std::string line, int line_num)
{
    DEBUG_LOG(line);

    for (int i = 0; i < line.size(); i++)
    {
        Pipe *pipe = new Pipe();
        pipe->point0 = { i, line_num };
        pipe->point1 = { i + 1, line_num + 1 };

        pipe->type = typeLookup(line[i]);

        if(pipe->type != TYPE::GROUND) // ignore storing ground types
            map[pipe->point0] = pipe;

        if (line[i] == 'S') {
            starter = pipe;
            map[pipe->point0] = pipe;
        }
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
    convert_starter();
    build_relationships();
    
}

int main()
{
    ExecutionTimer<std::chrono::milliseconds> timer;
    int score, i, c, m;

    std::cout << "Advent of Code 2023, Day " << DAY << ": " << SLOGAN << "." << std::endl;
    std::cout << "========================================" << std::endl;

    build_cache();

    std::cout << "Starter Position Type: ";
    switch (starter->type)
    {
    case STRAIGHT_VERT:
        std::cout << "vertical pipe <║> ";
        break;
    case STRAIGHT_HORZ:
        std::cout << "horizontal pipe <=> ";
        break;
    case CORNER_BL:
        std::cout << "bottom left <╚> ";
        break;
    case CORNER_BR:
        std::cout << "bottom right <╝> ";
        break;
    case CORNER_TR:
        std::cout << "top right <╗> ";
        break;
    case CORNER_TL:
        std::cout << "top left <╔> ";
        break;
    case GROUND:
    default:
        std::cout << "not found";
        break;
    }
    std::cout << std::endl;

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