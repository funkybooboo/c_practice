//
// Created by Nathan Stott on 28/06/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>

#define BOARDSIZE 8

char board[BOARDSIZE][BOARDSIZE];
int xNum = 12;
int oNum = 12;

void makeBoard() {
    for (int i = 0; i < BOARDSIZE; ++i) {
        for (int j = 0; j < BOARDSIZE; ++j) {
            if (i == 1) {
                if (j % 2 == 0) {
                    board[i][j] = 'O';
                } else {
                    board[i][j] = ' ';
                }
            }
            else if (i < 3) {
                if (j % 2 == 1) {
                    board[i][j] = 'O';
                } else {
                    board[i][j] = ' ';
                }
            }
            else if (i == 6) {
                if (j % 2 == 1) {
                    board[i][j] = 'X';
                } else {
                    board[i][j] = ' ';
                }
            }
            else if (i > 4) {
                if (j % 2 == 0) {
                    board[i][j] = 'X';
                } else {
                    board[i][j] = ' ';
                }
            } else {
                board[i][j] = ' ';
            }
        }
    }
}

void printBoard() {
    printf("   1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < BOARDSIZE; ++i) {
        printf("   - - - - - - - -\n%c |",i+65);
        for (int j = 0; j < BOARDSIZE; ++j) {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("   - - - - - - - -\n");
}

void checkBounds(const char *indicator, int *row, int *col) {
    bool validRow = false;
    bool validCol = false;
    while (!validRow && !validCol) {
        char move[2];
        printf("%s: ", indicator);
        scanf("%s", move);
        printf("\n");
        if (((int)move[1]) - 1 < 8 && ((int)move[1]) - 1 >= 0) {
            (*col) = ((int)move[1]) - 1;
            validCol = true;
        } else {
            printf("Invalid cell\n");
        }
        switch (move[0]) {
            case 'A':
                validRow = true;
                (*row) = 0;
                break;
            case 'B':
                validRow = true;
                (*row) = 1;
                break;
            case 'C':
                validRow = true;
                (*row) = 2;
                break;
            case 'D':
                validRow = true;
                (*row) = 3;
                break;
            case 'E':
                validRow = true;
                (*row) = 4;
                break;
            case 'F':
                validRow = true;
                (*row) = 5;
                break;
            case 'G':
                validRow = true;
                (*row) = 6;
                break;
            case 'H':
                validRow = true;
                (*row) = 7;
                break;
            default:
                printf("Invalid cell\n");
        }
    }
}

void checkMove(char indicator[], char p, int* result) {
    bool validResult = false;
    while (!validResult) {
        int row;
        int col;
        checkBounds(indicator, &row, &col);
        int value = strcmp(indicator, "from");
        if (value == 0) {
            if (board[row][col] == p) {
                validResult = true;
                result[0] = row;
                result[1] = col;
            } else {
                printf("Invalid option\n");
            }
        } else {
            if (board[row][col] == ' ') {
                validResult = true;
                result[0] = row;
                result[1] = col;
            } else {
                printf("Invalid option\n");
            }
        }
    }
}

bool checkValid(const int from[], const int to[]) {
    // TODO
    // if you can jump then have to

    // jump over

    // same piece double jump

    // move to the side one


}

void move(const int from[], const int to[]) {
    board[to[0]][to[1]] = board[from[0]][from[1]];
    board[from[0]][from[1]] = ' ';
}

void makeMove(char name[25], char p) {
    bool valid = false;
    int fromResult[2];
    int toResult[2];
    while (!valid) {
        printf("%s turn", name);
        char fromIndicator[] = "from";
        checkMove(fromIndicator, p, fromResult);
        char toIndicator[] = "to";
        checkMove(toIndicator, p, toResult);
        if (checkValid(fromResult, toResult)) {
            valid = true;
        } else {
            printf("Invalid option\n");
        }
    }
    move(fromResult, toResult);
}

bool checkWinner() {
    if (xNum == 0 || oNum == 0) {
        return true;
    }
    // TODO check if no one can move

    return 0;
}

bool go(char name[], char p) {
    printBoard();
    makeMove(name, p);
    if (checkWinner()) {
        printf("%s won!", name);
        return true;
    }
    return false;
}

void play() {
    char name1[25];
    char name2[25];
    printf("Player1 name: ");
    scanf("%s", name1);
    printf("\n");
    printf("Player2 name: ");
    scanf("%s", name2);
    printf("\n");
    makeBoard();
    while (1) {
        if (go(name1, 'X')) {
            break;
        }
        if (go(name2, 'O')) {
            break;
        }
    }
}

int main() {
    printf("Checkers!\n");
    char option;
    printf("\tSee rules (1)\n");
    printf("\tPlay (2)\n");
    printf("Enter: ");
    scanf("%c", &option);
    printf("\n");
    if (option == '1') {
        char line[255];
        FILE *fptr = fopen("Checkers/Rules.txt", "r");
        while (fgets(line, 255, fptr)) {
            printf("%s\n", line);
        }
        fclose(fptr);
        bool done = false;
        while (!done) {
            printf("Play game (p)\n");
            printf("Enter: ");
            scanf("%c", &option);
            printf("\n");
            if (option == 'p') {
                done = true;
            }
        }
    }
    play();
    return 0;
}
