// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int score,
        ich,
        i,
        iLine;
    FILE* ptr;
    char ch,
         digits[2];


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
    score = 0;

    // so the strategy here is, we're going to use strstr to find the index positions of known number strings, and known numbers, sort by index value
    char line[100];
    int iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        printf("---------------------------\nline: %s\n", line);
        // look for words of numbers
        for (i = 0; i < sizeof(line); i++) {

        }
        
        // look for numerical numbers
        
        int combinedDigits = atoi(digits);
        printf("Line result: %i + %i = %i\n", digits[0], digits[1], combinedDigits);

        score += combinedDigits;
        iLine++;
        digits[0] = 0;
        digits[1] = 0;
    }

 
    printf("Part 2 score is\n\n%i\n\.", score);


    fclose(ptr);

    

    return 0;
}