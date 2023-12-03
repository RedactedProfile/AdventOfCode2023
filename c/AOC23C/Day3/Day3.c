// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GRID_WIDTH 150
#define GRID_HEIGHT 150

#define SYMBOL_BLANK    '.'

#define SYMBOL_STAR     '*'
#define SYMBOL_DOLLAR   '$'
#define SYMBOL_AMP      '&'
#define SYMBOL_SLASH    '/'
#define SYMBOL_AT       '@'
#define SYMBOL_PERCENT  '%'
#define SYMBOL_EQ       '='
#define SYMBOL_ADD      '+'
#define SYMBOL_SUB      '-'
#define SYMBOL_HASH     '#'
#define SLOT_STAR       0
#define SLOT_DOLLAR     1
#define SLOT_AMP        2
#define SLOT_SLASH      3
#define SLOT_AT         4
#define SLOT_PERCENT    5
#define SLOT_EQ         6
#define SLOT_ADD        7
#define SLOT_SUB        8
#define SLOT_HASH       9

typedef struct vec2_s {
    int x, y;
} vec2_t;

typedef struct cached_value_s {
    vec2_t points[4];
    int value;
} cached_value_t;

int score, score2, // First part and second part score
    i, r, c;        // loop indexes; i = generic index, r = row, c = column
char grid[GRID_HEIGHT][GRID_WIDTH];

cached_value_t cache[GRID_HEIGHT][GRID_WIDTH];

int cache_ptr,
    xcache_ptr,
    ycache_ptr;

char symbols[] = {
    SYMBOL_STAR,
    SYMBOL_DOLLAR,
    SYMBOL_AMP,
    SYMBOL_SLASH,
    SYMBOL_AT,
    SYMBOL_PERCENT,
    SYMBOL_EQ,
    SYMBOL_ADD,
    SYMBOL_SUB,
    SYMBOL_HASH
};

void CacheValueIdentity(cached_value_t* item)
{
    for (int v = 0; v < 4; v++) {
        item->points[v].x = -1;
        item->points[v].y = -1;
    }
    item->value = 0;
}

int CursorScan(vec2_t cursor, char symbol) 
{
    int consideredValues[8] = { '\0', '\0', '\0' , '\0' , '\0' , '\0' , '\0' , '\0' },
        iConsidered = 0,
        sum = 0;
    printf("======= Scanning around %c at %i,%i \n", symbol, cursor.x, cursor.y);
    // only consider each discovered number once
    
    // look through cache index 
    vec2_t points[] = {
        { cursor.x - 1, cursor.y     }, // LEFT
        { cursor.x - 1, cursor.y - 1 }, // TOP LEFT
        { cursor.x    , cursor.y - 1 }, // TOP
        { cursor.x + 1, cursor.y - 1 }, // TOP RIGHT 
        { cursor.x + 1, cursor.y     }, // RIGHT 
        { cursor.x + 1, cursor.y + 1 }, // BOTTOM RIGHT 
        { cursor.x    , cursor.y + 1 }, // BOTTOM
        { cursor.x - 1, cursor.y + 1 }  // BOTTOM LEFT
    };
    
    // cycle through each of the possible points in sequence around the cursor
    for (int p = 0; p < sizeof(points) / sizeof(vec2_t); p++)
    {
        // clamp value if neeed to avoid OOI errors
        //if (points[p].x < 0) points[p].x = 0;
        //else if (points[p].x > GRID_WIDTH) points[p].x = GRID_WIDTH;
        //else if (points[p].y < 0) points[p].y = 0;
        //else if (points[p].y > GRID_HEIGHT) points[p].y = GRID_HEIGHT;

        int value = cache[points[p].y][points[p].x].value;
        if (value > 0) {
            //printf("Found value: %i as {x:%i:y:%i}\n", value, points[p].x, points[p].y);
            
            
            // loop through considered values to see if it's already there
            bool found = false;
            for (int v = 0; v < sizeof(consideredValues) / sizeof(int); v++) {
                if (consideredValues[v] == value) {
                    found = true;
                    break; // out of the for loop
                }
            }
            if (found) continue;
            consideredValues[iConsidered] = value;
            iConsidered++;
        }
        

    }

    printf("Considered values: ");
    for (int v = 0; v < sizeof(consideredValues) / sizeof(int); v++) {
        if (consideredValues[v] > 0) {
            printf("%i, ", consideredValues[v]);
            sum += consideredValues[v];
        }
        
    }
    printf("= \033[0;31m%i\033[0m\n\n", sum);
    
    return sum;
}

int main()
{
    int ich,            // character index
        iLine;          // line index 

    char ch;            // character container

    vec2_t cursor;
    FILE* ptr;

    printf("=====================================\n==== AOC '23, C edition \n====     by Kyle Harrison\n===================\n==== Day 3 \n=================================\n\n");

    cursor.x = 0;
    cursor.y = 0;
    cache_ptr = 0;
    xcache_ptr = 0;
    ycache_ptr = 0;
    score = 0;


    /// Read Input

    printf("Reading input file\n");
    ptr = fopen("../../../_puzzle_input/day3/c_input.txt", "r");
    if (NULL == ptr) {
        printf("File can't be opened. Aborting.\n");
        return 1;
    }

    printf("Part 1: \n");

    char line[GRID_WIDTH];
    iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        unsigned long long l = sizeof(line);
        char new_number[4] = { '\0', '\0', '\0', '\0' };
        cached_value_t new_item;
        CacheValueIdentity(&new_item);
        //printf("---------------------------\nline: %s", line);
        ich = 0;
        for (i = 0; i < sizeof(line); i++) {

            ch = line[i];
            if (ch == '\n' || ch == '\0') {
                break;
            }

            grid[iLine][i] = ch;

            //find whole numbers and store them as cached values
            int int_test = ch - '0';
            if (int_test >= 0 && int_test <= 9) {
                new_number[ich] = ch;
                new_item.points[ich].x = i;
                new_item.points[ich].y = iLine;
                ich++;
            }
            else if (ich > 0) {
                new_item.value = atoi(new_number);

                for (int p = 0; p < 4; p++) {
                    if (new_item.points[p].y >= 0 && new_item.points[p].x >= 0) {
                        cache[new_item.points[p].y][new_item.points[p].x] = new_item;
                    }
                }

                // reset values properly
                memset(new_number, 0, sizeof(new_number));
                CacheValueIdentity(&new_item);
                ich = 0;
            }
        }
        
        iLine++;
    }

    // loop through and find part numbers to do things with 
    for (r = 0; r < GRID_HEIGHT; r++)
    {
        cursor.y = r;
        for (c = 0; c < GRID_WIDTH; c++) {
            cursor.x = c;
            ch = grid[r][c];

            if (ch == '\0') {
                break; // end of the line bucko
            }

            // move on if `.` or digit
            int int_test = ch - '0';
            if (ch == SYMBOL_BLANK || (int_test >= 1 && int_test <= 9)) {
                continue;
            }

            // clearly one of the symbols, do a scan
            switch (ch)
            {
            case SYMBOL_STAR:
            case SYMBOL_DOLLAR:
            case SYMBOL_AMP:
            case SYMBOL_SLASH:
            case SYMBOL_AT:
            case SYMBOL_PERCENT:
            case SYMBOL_EQ:
            case SYMBOL_ADD:
            case SYMBOL_SUB:
            case SYMBOL_HASH:
                score += CursorScan(cursor, ch);
                printf("New Score: \033[0;32m%i\033[0m\n\n", score);
                break;
            default:
                continue;
                break;
            }
        }
    }

    fclose(ptr);

    
    int desired_score = 544664;
    printf("==========================================\n====== Score Part 1: %i\n==============================================\n====== Desired:      %i\n\n", score, desired_score);
    
    
    if (score > desired_score) {
        printf("Too high by %i \n\n", score - desired_score);
    }
    else if (score < desired_score) {
        printf("Too low by %i \n\n", desired_score - score);
    }
    else if (score == desired_score) {
        printf("- Congratulations 🥳 - ");
    }

    printf("Done.");

    return 0;
}
