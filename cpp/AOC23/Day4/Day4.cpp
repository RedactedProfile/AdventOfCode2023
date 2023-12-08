#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ranges>
#include <ExecutionTime.h>


#define USE_SAMPLE2

#ifdef USE_SAMPLE
    #define FILENAME        "input_sample.txt"
    #define MAX_WINNERS     5
    #define MAX_PLAYS       8
    #define MAX_ID_SIZE     2
    #define MAX_VALUE_SIZE  2
#endif // USE_SAMPLE
#ifndef USE_SAMPLE
    #define FILENAME        "input.txt"
    #define MAX_WINNERS     10
    #define MAX_PLAYS       25
    #define MAX_ID_SIZE     4
    #define MAX_VALUE_SIZE  2
#endif // !USE_SAMPLE



class Game
{
public:
    int id;
    std::vector<int> winners,
        plays;

    Game() : winners(MAX_WINNERS), plays(MAX_PLAYS) {
        id = 0;
    }

    int total_matches() {
        int score = 0;
        for (int i = 0; i < plays.size(); ++i) {
            if (std::find(winners.begin(), winners.end(), plays[i]) != winners.end()) {
                score++;
            }
        }
        return score;
    }

    int score() {
        int score = 0;
        for (int i = 0; i < plays.size(); ++i) {
            if (std::find(winners.begin(), winners.end(), plays[i]) != winners.end()) {
                if (0 == score) score = 1;
                else if (score >= 1) score *= 2;
            }
        }
        return score;
    }
};

std::vector<Game> games = std::vector<Game>();

void line_parser(std::string line, int line_num)
{
    std::cout << line << std::endl;

    // Format
    // Card   #: ## ## ## ## ## | ## ## ## ## ## ##
    // Card  ##:  # ## ## ## ## | ## ## ##  # ##  #
    // Card ###: ##  # ## ##  # | ##  # ## ##  # ##
    // we can ignore Card and jump the cursor ahead to the card number because we don't care

    // parsing state
    enum STATE
    {
        ID, WINNERS, PLAYS
    };
    STATE state = STATE::ID;
    auto game = Game();
    int val_cursor = 0;

    for (int i = 4; i < line.length(); ++i) {
        char ch = line[i];

        if (ch == ':') {
            state = STATE::WINNERS;
            i++;
            val_cursor = 0;
            continue;
        }
        else if (ch == '|') {
            state = STATE::PLAYS;
            i++;
            val_cursor = 0;
            continue;
        }

        switch (state)
        {
        case ID: {

            // in ID mode we can straight up capture the next three characters and jump the cursor
#ifdef USE_SAMPLE
            char val[] = { line[i], line[i + 1], '\0' };
#else
            char val[] = { line[i], line[i + 1], line[i + 2], line[i + 3], '\0' };
#endif // USE_SAMPLE

            game.id = atoi(val);
            i += MAX_ID_SIZE - 1;

            break;
        }
        case WINNERS: {

            char val[] = { line[i], line[i + 1], '\0' };
            game.winners[val_cursor] = atoi(val);
            i += MAX_VALUE_SIZE;
            val_cursor++;

            break;
        }
        case PLAYS: {

            char val[] = { line[i], line[i + 1], '\0' };
            game.plays[val_cursor] = atoi(val);
            i += MAX_VALUE_SIZE;
            val_cursor++;

            break;
        }
        default:
            break;
        }

    }

    games.push_back(game);

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
    std::string filepath = "../../../_puzzle_input/day4/";
    filepath += FILENAME;
    read_file(filepath, line_parser);
}

int main()
{
    ExecutionTimer<std::chrono::milliseconds> timer;
    int score, i, c, m;

    std::cout << "Advent of Code 2023, Day 4: Scratchcards." << std::endl;
    std::cout << "========================================" << std::endl;

    build_cache();

    // Part 1, sum up the calculated score of each card
    {
        score = 0;
        for (i = 0; i < games.size(); ++i) {
            score += games[i].score();
        }

        std::cout << "Part 1 Score: " << score << std::endl;
    }

    // Part 2, find out how many winning numbers there were per card, clone that many cards into their own iterable container separated by card ID
    {
        score = 0;
        std::vector<std::vector<Game>> buckets = std::vector<std::vector<Game>>(games.size()); // pre-allocated a bucket-per-game 
        // populate the buckets first element with each game
        for (i = 0; i < games.size(); ++i) {
            Game game = games[i];
            int id0 = game.id - 1;
            buckets[id0].push_back(game);
        }

        // with the buckets pre-allocated with a copy of each original game 
        // now we iterate over the buckets themselves, keeping in mind a pointer to the actual game in the original games manifest. 
        // This is important because we'll need to clone n amount of games into their respective buckets based on total matches of the game 
        for (i = 0; i < buckets.size(); ++i) {
            for (c = 0; c < buckets[i].size(); ++c) {
                Game game = buckets[i][c];
                int matches = game.total_matches();

                for (m = 0; m < matches; ++m) {
                    // actually we can just clone the first element of that respective bucket
                    int id1 = i + m + 1;
                    buckets[id1].push_back(buckets[id1][0]);
                }
            }
        }

        // run through one more time to count the number of cards
        for (i = 0; i < buckets.size(); ++i) {
            score += buckets[i].size();
        }

        std::cout << "Part 2 Score: " << score << std::endl;
    }
}