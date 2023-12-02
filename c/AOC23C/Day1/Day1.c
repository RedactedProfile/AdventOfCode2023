// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StrIntMap {
    char key[20];
    int value;
};

static char** StrIntMapGetKeys(struct StrIntMap map[], int size)
{
    int i;
    char** strArr = (char**)malloc(size * sizeof(char*));

    if (strArr == NULL) {
        printf("Memory allocation of strings failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < size; i++) {
        strArr[i] = _strdup(map[0].key);
    }

    return strArr;
}

static int StrIntMapFindByKey(struct StrIntMap map[], int size, const char* key) 
{
    int i;

    for (i = 0; i < size; i++) {
        if (strcmp(map[i].key, key) == 0) {
            return map[i].value;
        }
    }

    return -1;
}

int main()
{
    int score,
        ich;
    FILE* ptr;
    char ch,
         digits[2];

    struct StrIntMap map[] = {
        { "one", 1 },
        { "two", 2 },
        { "three", 3 },
        { "four", 4 },
        { "five", 5 },
        { "six", 6 },
        { "seven", 7 },
        { "eight", 8 },
        { "nine", 9 }
    };
    int mapSize = sizeof(map) / sizeof(map[0]);

    auto value = StrIntMapFindByKey(map, mapSize, "two");
    auto value2 = StrIntMapGetKeys(map, mapSize);

    printf("=====================================\n==== AOC '23, C edition \n====     by Kyle Harrison\n===================\n==== Day 1 \n=================================\n\n");

    /// Read Input

    printf("Reading input file\n");
    ptr = fopen("../../../_puzzle_input/day1/input.txt", "r");
    if (NULL == ptr) {
        printf("File can't be opened. Aborting.\n");
        return 1;
    }

    printf("Part 1: Finding the numbers on each line, adding them up, then adding up all the lines\n");
    
    digits[0] = 0; 
    digits[1] = 0;
    score = 0;
    do {
        ch = fgetc(ptr);

        // test if valid number
        ich = ch - '0';
        if (ich >= 1 && ich <= 9) { // valid integer
            if (digits[0] == 0) {
                digits[0] = ch;
                digits[1] = ch; // if there's only one number on this line, we add it together
            }
            else {
                digits[1] = ch;
            }
        }
       
        if (ch == '\n') {
            // combine the digits to make a number, add them to the score, clear them and restart 
            score += atoi(digits);
            digits[0] = 0; 
            digits[1] = 0;
        }

    } while (ch != EOF);
    if (digits[0] != 0) {
        score += atoi(digits);
    }

    fclose(ptr);

    printf("Final score is\n\n%i\n\nDone.", score);

    return 0;
}