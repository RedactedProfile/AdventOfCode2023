// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int score,
        ich;
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

    fclose(ptr);

    printf("Final score is\n\n%i\n\nDone.", score);

    return 0;
}