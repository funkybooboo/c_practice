//
// Created by Nathan Stott on 28/06/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>

#define BOARDSIZE 8

typedef char Name[25];

typedef struct {
    int row;
    int col;
    bool isAlive;
    char type; // n = normal, k = king, b = blank
    char side;
} Piece;

Piece board[BOARDSIZE][BOARDSIZE];
Piece list[24];
int xNum = 12;
int oNum = 12;

int count = 0;
void makePiece(int row, int col, char side) {
    Piece piece;
    piece.row = row;
    piece.col = col;
    if (side == ' ') {
        piece.isAlive = false;
        piece.side = 'b';
    } else {
        piece.isAlive = true;
        piece.side = side;
    }
    board[row][col] = piece;
    list[count] = piece;
}

void makeBoard() {
    for (int i = 0; i < BOARDSIZE; ++i) {
        for (int j = 0; j < BOARDSIZE; ++j) {
            if (i == 1) {
                if (j % 2 == 0) {
                    makePiece(i, j, 'O');
                } else {
                    makePiece(i, j, ' ');
                }
            }
            else if (i < 3) {
                if (j % 2 == 1) {
                    makePiece(i, j, 'O');
                } else {
                    makePiece(i, j, ' ');
                }
            }
            else if (i == 6) {
                if (j % 2 == 1) {
                    makePiece(i, j, 'X');
                } else {
                    makePiece(i, j, ' ');
                }
            }
            else if (i > 4) {
                if (j % 2 == 0) {
                    makePiece(i, j, 'X');
                } else {
                    makePiece(i, j, ' ');
                }
            } else {
                makePiece(i, j, ' ');
            }
        }
    }
}

void printBoard() {
    printf("   1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < BOARDSIZE; ++i) {
        printf("   - - - - - - - -\n%c |",i+65);
        for (int j = 0; j < BOARDSIZE; ++j) {
            printf("%c|", board[i][j].side);
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
            if (board[row][col].side == p) {
                validResult = true;
                result[0] = row;
                result[1] = col;
            } else {
                printf("Invalid option\n");
            }
        } else {
            if (board[row][col].side == ' ') {
                validResult = true;
                result[0] = row;
                result[1] = col;
            } else {
                printf("Invalid option\n");
            }
        }
    }
}

void moveToSide(Piece fromPiece, Piece toPiece) {
    toPiece.side = fromPiece.side;
    fromPiece.side = ' ';
    toPiece.isAlive = true;
    fromPiece.isAlive = false;
    toPiece.type = fromPiece.type;
    fromPiece.type = 'b';
    if ((toPiece.side == 'O' && toPiece.row == BOARDSIZE-1) || (toPiece.side == 'X' && toPiece.row == 0)) {
        toPiece.type = 'k';
    }
}

void jumpMove(Piece fromPiece, Piece toPiece, Piece middlePiece) {
    if (middlePiece.side == 'O') {
        oNum--;
    } else if (middlePiece.side == 'X') {
        xNum--;
    }
    moveToSide(fromPiece, toPiece);
    middlePiece.side = ' ';
    middlePiece.isAlive = false;
    middlePiece.type = 'b';

}

bool checkValid(const int from[], const int to[], char currentSide) {
    bool jump = false;
    Piece fromPiece = board[from[0]][from[1]];
    Piece toPiece = board[to[0]][to[1]];
    char otherSide;

    if (currentSide == 'O') {
        otherSide = 'X';
    }
    else if (currentSide == 'X') {
        otherSide = 'O';
    } else {
        return false;
    }

    if (fromPiece.side == ' ' || toPiece.side != ' ') {
        return false;
    }

    // move to the side one
    if ((fromPiece.row+1 == toPiece.row && fromPiece.col+1 == toPiece.col) || (fromPiece.row+1 == toPiece.row && fromPiece.col-1 == toPiece.col)) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            moveToSide(fromPiece, toPiece);
            return true;
        }
        return false;
    }
    else if ((fromPiece.row-1 == toPiece.row && fromPiece.col+1 == toPiece.col) || (fromPiece.row-1 == toPiece.row && fromPiece.col-1 == toPiece.col)) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            moveToSide(fromPiece, toPiece);
            return true;
        }
        return false;
    }

    // jump over
    if (fromPiece.row+2 == toPiece.row && fromPiece.col+2 == toPiece.col) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            Piece middlePiece = board[fromPiece.row+1][fromPiece.col+1];
            if (middlePiece.side != otherSide) {
                return false;
            }
            jumpMove(fromPiece, toPiece, middlePiece);
            jump = true;
        }
        return false;
    }
    else if (fromPiece.row-2 == toPiece.row && fromPiece.col+2 == toPiece.col) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            Piece middlePiece = board[fromPiece.row-1][fromPiece.col+1];
            if (middlePiece.side != otherSide) {
                return false;
            }
            jumpMove(fromPiece, toPiece, middlePiece);
            jump = true;
        }
        return false;
    }
    else if (fromPiece.row+2 == toPiece.row && fromPiece.col-2 == toPiece.col) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            Piece middlePiece = board[fromPiece.row+1][fromPiece.col-1];
            if (middlePiece.side != otherSide) {
                return false;
            }
            jumpMove(fromPiece, toPiece, middlePiece);
            jump = true;
        }
        return false;
    }
    else if (fromPiece.row-2 == toPiece.row && fromPiece.col-2 == toPiece.col) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            Piece middlePiece = board[fromPiece.row-1][fromPiece.col-1];
            if (middlePiece.side != otherSide) {
                return false;
            }
            jumpMove(fromPiece, toPiece, middlePiece);
            jump = true;
        }
        return false;
    }

    if (jump) {

        return true;
    }

    return false;
}

void makeMove(Name name, char currentSide) {
    bool valid = false;
    int fromResult[2];
    int toResult[2];
    while (!valid) {
        printf("%s turn", name);
        char fromIndicator[] = "from";
        checkMove(fromIndicator, currentSide, fromResult);
        char toIndicator[] = "to";
        checkMove(toIndicator, currentSide, toResult);
        if (checkValid(fromResult, toResult, currentSide)) {
            valid = true;
        } else {
            printf("Invalid option\n");
        }
    }
}

bool checkWinner() {
    if (xNum == 0 || oNum == 0) {
        return true;
    }
    // TODO check if no one can move

    return 0;
}

bool go(char name[], char currentSide) {
    printBoard();
    makeMove(name, currentSide);
    if (checkWinner()) {
        printf("%s won!", name);
        return true;
    }
    return false;
}

void play() {
    Name name1;
    Name name2;
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
