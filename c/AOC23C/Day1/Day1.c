// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 10
#define MAX_WORD_LENGTH 5

#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7 
#define EIGHT 8 
#define NINE 9
#define STR0 "zero"
#define STR1 "one"
#define STR2 "two"
#define STR3 "three"
#define STR4 "four"
#define STR5 "five"
#define STR6 "six"
#define STR7 "seven"
#define STR8 "eight"
#define STR9 "nine"

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

void removeSubstring(char* s, const char* toremove)
{
    while (s = strstr(s, toremove)) {
        memmove(s, s + strlen(toremove), 1 + strlen(s + strlen(toremove)));
    }
}

int main()
{
    int score,
        ich,
        i,
        mapSize;
    FILE* ptr;
    char ch,
         digits[2];

    char strDigits[MAX_WORDS * 2][MAX_WORD_LENGTH];
    strcpy(strDigits[0], STR0);
    strcpy(strDigits[1], STR1);
    strcpy(strDigits[2], STR2);
    strcpy(strDigits[3], STR3);
    strcpy(strDigits[4], STR4);
    strcpy(strDigits[5], STR5);
    strcpy(strDigits[6], STR6);
    strcpy(strDigits[7], STR7);
    strcpy(strDigits[8], STR8);
    strcpy(strDigits[9], STR9);
    strcpy(strDigits[10], "0");
    strcpy(strDigits[11], "1");
    strcpy(strDigits[12], "2");
    strcpy(strDigits[13], "3");
    strcpy(strDigits[14], "4");
    strcpy(strDigits[15], "5");
    strcpy(strDigits[16], "6");
    strcpy(strDigits[17], "7");
    strcpy(strDigits[18], "8");
    strcpy(strDigits[19], "9");

    struct StrIntMap map[] = {
        { STR1, ONE },
        { STR2, TWO },
        { STR3, THREE },
        { STR4, FOUR },
        { STR5, FIVE },
        { STR6, SIX },
        { STR7, SEVEN },
        { STR8, EIGHT },
        { STR9, NINE }
    };
    mapSize = sizeof(map) / sizeof(map[0]);

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

    printf("Part 1 score is\n\n%i\n", score);


    printf("==============================\n\nPart 2: Finding the numbers on each line, where words are also valid numbers, adding them up, then adding up all the lines\n");
    rewind(ptr);
    digits[0] = 0;
    digits[1] = 0;
    int lastFoundDigit0 = -1;
    int lastFoundDigit1 = -1;
    int charsRemoved = 0;
    score = 0;

    // so the strategy here is, we're going to use strstr to find the index positions of known number strings, and known numbers, sort by index value
    char line[100];
    int iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        printf("---------------------------\nline: %s\n", line);
        // look for words of numbers
        for (i = 0; i < (sizeof(strDigits) / sizeof(strDigits[0])); i++) {
            for (;;) {
                // todo, need to catch all instances of repeat numbers

                char* currentStr = strDigits[i];
                size_t currentStrLen = strlen(currentStr);
                char* substrPtr = strstr(line, currentStr);

                if (substrPtr == NULL) {
                    break;
                }

                int index = substrPtr - line;
                int foundIntInMap = StrIntMapFindByKey(map, mapSize, currentStr);
                if (foundIntInMap == -1) { // non-string, just get number
                    foundIntInMap = atoi(currentStr);
                }
                index += charsRemoved;

                printf("Found %s at index %i\n", currentStr, index);
                if (lastFoundDigit0 == -1) { // havent found one yet, set both
                    digits[0] = foundIntInMap;
                    digits[1] = digits[0];
                    lastFoundDigit0 = index;
                    lastFoundDigit1 = index;
                }
                else if (index < lastFoundDigit0) { // if this was found earlier in the string than our last recorded earliest, overwirte 
                    digits[0] = foundIntInMap;
                    lastFoundDigit0 = index;
                }
                else if (index > lastFoundDigit1) { // if this was found later in the string than our last recorded furthest, overwirte
                    digits[1] = foundIntInMap;
                    lastFoundDigit1 = index;
                }

                // remove found item from string 
                removeSubstring(line, currentStr);
                charsRemoved += currentStrLen;
                printf("new removed length: %i\n", charsRemoved);
                printf("adjusted line: %s\n", line);
            }
        }
        // look for numerical numbers
        
        int combinedDigits = atoi(digits);
        printf("Line result: %i + %i = %i\n", digits[0], digits[1], combinedDigits);

        score += combinedDigits;

        //printf(line);
        iLine++;
        digits[0] = 0;
        digits[1] = 0;
        lastFoundDigit0 = -1;
        lastFoundDigit1 = -1;
    }

 
    printf("Part 2 score is\n\n%i\n\.", score);


    fclose(ptr);

    

    return 0;
}