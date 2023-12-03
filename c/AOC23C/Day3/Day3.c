// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
    char ch;
    FILE* ptr;


    printf("=====================================\n==== AOC '23, C edition \n====     by Kyle Harrison\n===================\n==== Day 3 \n=================================\n\n");

    score = 0;
    /// Read Input

    printf("Reading input file\n");
    ptr = fopen("../../../_puzzle_input/day3/input_sample.txt", "r");
    if (NULL == ptr) {
        printf("File can't be opened. Aborting.\n");
        return 1;
    }

    printf("Part 1: \n");

    char line[100];
    iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        printf("---------------------------\nline: %s", line);
        for (i = 0; i < sizeof(line); i++) {

            ch = line[i];
            if (ch == '\n' || ch == '\0') {
                break;
            }
        }
        score += 0;
        iLine++;
    }

    fclose(ptr);

    printf("Done.");

    return 0;
}
