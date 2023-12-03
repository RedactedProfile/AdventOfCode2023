// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZERO    '0'
#define ONE     '1'
#define TWO     '2'
#define THREE   '3'
#define FOUR    '4'
#define FIVE    '5'
#define SIX     '6'
#define SEVEN   '7'
#define EIGHT   '8'
#define NINE    '9'

void applyDigit(char digits[], char ch) {
    if (digits[0] == 0) {
        digits[0] = ch;
        digits[1] = ch; // if there's only one number on this line, we add it together
    }
    else {
        digits[1] = ch;
    }
}

int main()
{
    int score,
        ich,
        i,
        iLine;
    FILE* ptr;
    char ch,
         digits[3];


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
    digits[2] = '\0';
    score = 0;
    do {
        ch = fgetc(ptr);

        // test if valid number
        ich = ch - '0';
        if (ich >= 1 && ich <= 9) { // valid integer
            applyDigit(digits, ch);
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

    printf("\n\n=======\nPart 1 score is %i\n\n", score);


    printf("==============================\n\nPart 2: Finding the numbers on each line, where words are also valid numbers, adding them up, then adding up all the lines\n");
    rewind(ptr);
    digits[0] = 0;
    digits[1] = 0;
    score = 0;

    char line[100];
    iLine = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        //printf("---------------------------\nline: %s", line);
        // look for words of numbers
        for (i = 0; i < sizeof(line); i++) {

            ch = line[i];
            if (ch == '\n' || ch == '\0') {
                break;
            }

            // test if valid number
            ich = ch - '0';
            if (ich >= 1 && ich <= 9) { // valid integer
                applyDigit(digits, ch);
            }
            else if (ch == 'z') { // zero
                if (line[i + 1] == 'e' && line[i + 2] == 'r' && line[i + 3] == 'o') {
                    applyDigit(digits, ZERO);
                }
            }
            else if (ch == 'o') { // one
                if (line[i + 1] == 'n' && line[i + 2] == 'e') {
                    applyDigit(digits, ONE);
                }
            }
            else if (ch == 't') { // two, three
                if (line[i + 1] == 'w' && line[i + 2] == 'o') {
                    applyDigit(digits, TWO);
                }
                else if (line[i + 1] == 'h' && line[i + 2] == 'r' && line[i + 3] == 'e' && line[i + 4] == 'e') {
                    applyDigit(digits, THREE);
                }
            }
            else if (ch == 'f') { // four, five
                if (line[i + 1] == 'o' && line[i + 2] == 'u' && line[i + 3] == 'r') {
                    applyDigit(digits, FOUR);
                }
                else if (line[i + 1] == 'i' && line[i + 2] == 'v' && line[i + 3] == 'e') {
                    applyDigit(digits, FIVE);
                }
            }
            else if (ch == 's') { // six, seven
                if (line[i + 1] == 'i' && line[i + 2] == 'x') {
                    applyDigit(digits, SIX);
                }
                else if (line[i + 1] == 'e' && line[i + 2] == 'v' && line[i + 3] == 'e' && line[i + 4] == 'n') {
                    applyDigit(digits, SEVEN);
                }
            }
            else if (ch == 'e') { // eight 
                if (line[i + 1] == 'i' && line[i + 2] == 'g' && line[i + 3] == 'h' && line[i + 4] == 't') {
                    applyDigit(digits, EIGHT);
                }
            }
            else if (ch == 'n') { // nine
                if (line[i + 1] == 'i' && line[i + 2] == 'n' && line[i + 3] == 'e') {
                    applyDigit(digits, NINE);
                }
            }
        }
        
        // look for numerical numbers
        
        int combinedDigits = atoi(digits);
        //printf("Digits: %c + %c = %i\n", digits[0], digits[1], combinedDigits);

        score += combinedDigits;
        iLine++;
        digits[0] = 0;
        digits[1] = 0;
    }

    if (digits[0] != 0) {
        int combinedDigits = atoi(digits);
        //printf("Digits: %c + %c = %i\n", digits[0], digits[1], combinedDigits);
        score += combinedDigits;
    }
 
    printf("\n\n=======\nPart 2 score is %i\n\n\nDone.", score);


    fclose(ptr);

    

    return 0;
}