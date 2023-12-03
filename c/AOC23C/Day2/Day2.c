// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROUNDS 10
#define ID_SEPARATOR ':'
#define ROUND_SEPARATOR ';'
#define GAME_DELIMINATOR ','

#define SLOT_RED 0
#define SLOT_GREEN 1
#define SLOT_BLUE 2

#define COLOR_ID 'i'
#define COLOR_RED 'r'
#define COLOR_GREEN 'g'
#define COLOR_BLUE 'b'

#define LEN_RED 2
#define LEN_GREEN 4
#define LEN_BLUE 3

#define STATE_ID 0
#define STATE_NUMERAL 1
#define STATE_COLOR 2

#define P1_MAX_RED 12
#define P1_MAX_GREEN 13
#define P1_MAX_BLUE 14

typedef struct round_s {
    char unassignedNumeral[4];
    int id;
    int rounds;
    int red[MAX_ROUNDS];
    int green[MAX_ROUNDS];
    int blue[MAX_ROUNDS];
} round_t;

// Reset a round object to default values
void RoundIdentity(round_t *rounds) {
    memset(rounds->unassignedNumeral, 0, sizeof(rounds->unassignedNumeral));
    rounds->id = 0;
    rounds->rounds = 0;
    for (int i = 0; i < MAX_ROUNDS; i++)
    {
        rounds->red[i] = 0;
        rounds->green[i] = 0;
        rounds->blue[i] = 0;
    }
}

// Assigns a value inside a round object based on a key
void RoundAssign(char color, round_t *round, int index) {
    int value = atoi(round->unassignedNumeral);
    if (color == COLOR_RED) round->red[index] = value;
    else if (color == COLOR_GREEN) round->green[index] = value;
    else if (color == COLOR_BLUE) round->blue[index] = value;
    else if (color == COLOR_ID) round->id = value;
    memset(round->unassignedNumeral, 0, sizeof(round->unassignedNumeral));
}

// Determine if round would have been possible without going over these thresholds
bool RoundQualify(round_t* round, int max_red, int max_green, int max_blue) {
    for (int i = 0; i < round->rounds; i++) {
        if (round->red[i] > max_red || round->blue[i] > max_blue || round->green[i] > max_green) {
            return false;
        }
    }
    return true;
}


// The power of a set of cubes is equal to the numbers of red, green, and blue cubes multiplied together.
int RoundFewest(round_t* round) {
    int highestValues[3] = {0,0,0};
    for (int i = 0; i < round->rounds; i++)
    {
        if (highestValues[SLOT_RED] < round->red[i]) highestValues[SLOT_RED] = round->red[i];
        if (highestValues[SLOT_GREEN] < round->green[i]) highestValues[SLOT_GREEN] = round->green[i];
        if (highestValues[SLOT_BLUE] < round->blue[i]) highestValues[SLOT_BLUE] = round->blue[i];
    }

    return highestValues[SLOT_RED] * highestValues[SLOT_GREEN] * highestValues[SLOT_BLUE];
}

int main()
{
    int score, score2,
        ich,
        i,
        iLine,
        state;
    char ch,
         line[200];
    FILE* ptr;

    
    printf("=====================================\n==== AOC '23, C edition \n====     by Kyle Harrison\n===================\n==== Day 2 \n=================================\n\n");

    score = 0;
    score2 = 0;
    /// Read Input

    printf("Reading input file\n");
    ptr = fopen("../../../_puzzle_input/day2/input.txt", "r");
    if (NULL == ptr) {
        printf("File can't be opened. Aborting.\n");
        return 1;
    }

    printf("Part 1: Cube Number Threshold \n");

    iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        printf("---------------------------\nline: %s", line);

        // Always Start off with collecting the game ID
        state = STATE_ID;
        round_t round;
        
        RoundIdentity(&round);

        ich = 0;

             // 👇 start at 5 to skip past "Game: "
        for (i = 5; i < sizeof(line); i++) {

            if (round.rounds > MAX_ROUNDS) {
                printf("Exceeded maximum rounds (%i) for this game", MAX_ROUNDS);
                exit(EXIT_FAILURE); // maybe this will tell me if I need to increase the limit or not
                break;
            }

            ch = line[i];
            
            if (ch == '\n' || ch == '\0') {
                state = STATE_ID;
                ich = 0;
                break;
            }

            if (state == STATE_ID) 
            {
                if (ch == ID_SEPARATOR) {
                    state = STATE_NUMERAL;
                    if (line[i + 1] == ' ') {
                        i++; // skip trailing whitespace if present
                    }
                    //round[iRound].id[ich] = '\0'; // end the char array for ID
                    RoundAssign(COLOR_ID, &round, round.rounds);
                    ich = 0;
                    continue;
                }

                if (ch == ' ') {
                    printf("A whitespace during ID parsing? This shouldn't be happening");
                    exit(EXIT_FAILURE);
                    continue; // skip whitespace
                }

                // collect ID info
                round.unassignedNumeral[ich] = ch;
                ich++;
            }
            else if (state == STATE_NUMERAL)
            {
                if (ch == ' ') { // a whitespace during numerical parsing is the delimiter between NUM and COL parsing
                    state = STATE_COLOR;
                    round.unassignedNumeral[ich] = '\0'; // end the char array for the unknown number
                    ich = 0;
                    continue; // skip ahead
                }
                
                round.unassignedNumeral[ich] = ch;
                ich++;
            }
            else if (state == STATE_COLOR)
            {
                if (ch == GAME_DELIMINATOR) {
                    state = STATE_NUMERAL;
                    if (line[i + 1] == ' ') {
                        i++; // skip trailing whitespace if present
                    }
                    ich = 0;
                    continue;
                }
                else if (ch == ROUND_SEPARATOR) {
                    // clean up

                    
                    state = STATE_NUMERAL;
                    if (line[i + 1] == ' ') {
                        i++; // skip trailing whitespace if present
                    }
                    ich = 0;
                    round.rounds++;
                    continue;
                }
                else if (ch == COLOR_RED) {
                    i += LEN_RED;
                    RoundAssign(ch, &round, round.rounds);
                }
                else if (ch == COLOR_GREEN) {
                    i += LEN_GREEN;
                    RoundAssign(ch, &round, round.rounds);
                }
                else if (ch == COLOR_BLUE) {
                    i += LEN_BLUE;
                    RoundAssign(ch, &round, round.rounds);
                }
                else if (ch == ' ') {
                    printf("A whitespace during COLOR parsing? This shouldn't be happening");
                    exit(EXIT_FAILURE);
                    continue; // skip ahead
                }
            }
        }

        round.rounds++; // stop 0-indexing, make actual count for reference later

        // Run round parser for Phase 1
        if (RoundQualify(&round, P1_MAX_RED, P1_MAX_GREEN, P1_MAX_BLUE)) {
            score += round.id;
        }

        score2 += RoundFewest(&round);

        // Run round parser for Phase 2?
        iLine++;
    }

    printf("====================\nPart 1 Score is: %i\n====================\n", score);
    printf("====================\nPart 2 Score is: %i\n====================\n", score2);

    fclose(ptr);

    printf("Done.");

    return 0;
}