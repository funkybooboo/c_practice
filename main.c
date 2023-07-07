//
// Created by Nathan Stott on 28/06/2023.
//
#include <stdio.h>
#include <stdbool.h>
#include<string.h>

#define BOARDSIZE 8
#define NUMPIECES 24

typedef char Name[25];

typedef struct {
    int row;
    int col;
    bool isAlive;
    char type; // n = normal, k = king, b = blank
    char side; // O, X, ' '
} Piece;

typedef struct {
    bool valid;
    Piece middle;
} Pair;

Piece board[BOARDSIZE][BOARDSIZE];
Piece list[NUMPIECES];
int xNum = 12;
int oNum = 12;

void makePiece(int row, int col, char side, int count) {
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
    int count = 0;
    for (int i = 0; i < BOARDSIZE; ++i) {
        for (int j = 0; j < BOARDSIZE; ++j) {
            if (i == 1) {
                if (j % 2 == 0) {
                    makePiece(i, j, 'O', count);
                } else {
                    makePiece(i, j, ' ', count);
                }
            }
            else if (i < 3) {
                if (j % 2 == 1) {
                    makePiece(i, j, 'O', count);
                } else {
                    makePiece(i, j, ' ', count);
                }
            }
            else if (i == 6) {
                if (j % 2 == 1) {
                    makePiece(i, j, 'X', count);
                } else {
                    makePiece(i, j, ' ', count);
                }
            }
            else if (i > 4) {
                if (j % 2 == 0) {
                    makePiece(i, j, 'X', count);
                } else {
                    makePiece(i, j, ' ', count);
                }
            } else {
                makePiece(i, j, ' ', count);
            }
            count++;
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

Piece checkBounds(const char *indicator) {
    int row;
    int col;
    bool validRow = false;
    bool validCol = false;
    while (!validRow && !validCol) {
        char move[2];
        printf("%s: ", indicator);
        scanf("%s", move);
        printf("\n");
        switch (move[0]) {
            case 'A':
                validRow = true;
                row = 0;
                break;
            case 'B':
                validRow = true;
                row = 1;
                break;
            case 'C':
                validRow = true;
                row = 2;
                break;
            case 'D':
                validRow = true;
                row = 3;
                break;
            case 'E':
                validRow = true;
                row = 4;
                break;
            case 'F':
                validRow = true;
                row = 5;
                break;
            case 'G':
                validRow = true;
                row = 6;
                break;
            case 'H':
                validRow = true;
                row = 7;
                break;
            default:
                printf("Invalid cell\n");
        }
        switch (move[1]) {
            case '1':
                validCol = true;
                col = 0;
                break;
            case '2':
                validCol = true;
                col = 1;
                break;
            case '3':
                validCol = true;
                col = 2;
                break;
            case '4':
                validCol = true;
                col = 3;
                break;
            case '5':
                validCol = true;
                col = 4;
                break;
            case '6':
                validCol = true;
                col = 5;
                break;
            case '7':
                validCol = true;
                col = 6;
                break;
            case '8':
                validCol = true;
                col = 7;
                break;
            default:
                printf("Invalid cell\n");
        }
    }
    return board[row][col];
}

void slide(Piece fromPiece, Piece toPiece) {
    Piece temp = toPiece;
    toPiece = fromPiece;
    fromPiece = temp;
    if ((toPiece.side == 'O' && toPiece.row == BOARDSIZE-1) || (toPiece.side == 'X' && toPiece.row == 0)) {
        toPiece.type = 'k';
    }
}

void jump(Piece fromPiece, Piece toPiece, Piece middlePiece) {
    if (middlePiece.side == 'O') {
        oNum--;
    } else if (middlePiece.side == 'X') {
        xNum--;
    }
    slide(fromPiece, toPiece);
    middlePiece.side = ' ';
    middlePiece.isAlive = false;
    middlePiece.type = 'b';

}

Pair isJump(Piece fromPiece, Piece toPiece, char otherSide) {
    Pair pair;
    pair.valid = false;
    if (fromPiece.side == ' ' || toPiece.side != ' ') {
        return pair;
    }
    if (fromPiece.row+2 == toPiece.row && fromPiece.col+2 == toPiece.col) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            Piece middlePiece = board[fromPiece.row+1][fromPiece.col+1];
            pair.middle = middlePiece;
            if (middlePiece.side != otherSide) {
                pair.valid = false;
                return pair;
            }
            pair.valid = true;
            return pair;
        }
    }
    else if (fromPiece.row-2 == toPiece.row && fromPiece.col+2 == toPiece.col) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            Piece middlePiece = board[fromPiece.row-1][fromPiece.col+1];
            if (middlePiece.side != otherSide) {
                pair.valid = false;
                return pair;
            }
            pair.valid = true;
            return pair;
        }
    }
    else if (fromPiece.row+2 == toPiece.row && fromPiece.col-2 == toPiece.col) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            Piece middlePiece = board[fromPiece.row+1][fromPiece.col-1];
            if (middlePiece.side != otherSide) {
                pair.valid = false;
                return pair;
            }
            pair.valid = true;
            return pair;
        }
    }
    else if (fromPiece.row-2 == toPiece.row && fromPiece.col-2 == toPiece.col) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            Piece middlePiece = board[fromPiece.row-1][fromPiece.col-1];
            if (middlePiece.side != otherSide) {
                pair.valid = false;
                return pair;
            }
            pair.valid = true;
            return pair;
        }
    }
    return pair;
}

bool isSlide(Piece fromPiece, Piece toPiece) {
    if (fromPiece.side == ' ' || toPiece.side != ' ') {
        return false;
    }
    if ((fromPiece.row+1 == toPiece.row && fromPiece.col+1 == toPiece.col) || (fromPiece.row+1 == toPiece.row && fromPiece.col-1 == toPiece.col)) {
        if ((fromPiece.side == 'O' && (fromPiece.type == 'n' || fromPiece.type == 'k')) || (fromPiece.side == 'X' && fromPiece.type == 'k')) {
            return true;
        }
        return false;
    }
    else if ((fromPiece.row-1 == toPiece.row && fromPiece.col+1 == toPiece.col) || (fromPiece.row-1 == toPiece.row && fromPiece.col-1 == toPiece.col)) {
        if ((fromPiece.side == 'O' && fromPiece.type == 'k') || (fromPiece.side == 'X' && (fromPiece.type == 'k' || fromPiece.type == 'n'))) {
            return true;
        }
        return false;
    }
    return false;
}

Pair canJump(Piece piece, char otherSide) {

    Pair bottomRight = isJump(piece, board[piece.row+2][piece.col+2], otherSide);
    if (bottomRight.valid) {
        return bottomRight;
    }

    Pair topRight = isJump(piece, board[piece.row-2][piece.col+2], otherSide);
    if (topRight.valid) {
        return topRight;
    }

    Pair bottomLeft = isJump(piece, board[piece.row+2][piece.col-2], otherSide);
    if (bottomLeft.valid) {
        return bottomLeft;
    }

    Pair topLeft = isJump(piece, board[piece.row-2][piece.col-2], otherSide);
    if (topLeft.valid) {
        return topLeft;
    }

    Pair none;
    none.valid = false;
    return none;
}

bool canSlide(Piece piece) {
    Piece topRight = board[piece.row-1][piece.col+1];
    Piece bottomRight = board[piece.row+1][piece.col+1];
    Piece topLeft = board[piece.row-1][piece.col-1];
    Piece bottomLeft = board[piece.row+1][piece.col-1];
    return topRight.side == ' ' || bottomRight.side == ' ' || topLeft.side == ' ' || bottomLeft.side == ' ';
}

void getUserCoordinates(Piece piece, char cord[]) {
    switch (piece.row) {
        case 0:
            cord[0] = 'A';
            break;
        case 1:
            cord[0] = 'B';
            break;
        case 2:
            cord[0] = 'C';
            break;
        case 3:
            cord[0] = 'D';
            break;
        case 4:
            cord[0] = 'E';
            break;
        case 5:
            cord[0] = 'F';
            break;
        case 6:
            cord[0] = 'G';
            break;
        case 7:
            cord[0] = 'H';
            break;
    }
    switch (piece.col) {
        case 0:
            cord[1] = '1';
            break;
        case 1:
            cord[1] = '2';
            break;
        case 2:
            cord[1] = '3';
            break;
        case 3:
            cord[1] = '4';
            break;
        case 4:
            cord[1] = '5';
            break;
        case 5:
            cord[1] = '6';
            break;
        case 6:
            cord[1] = '7';
            break;
        case 7:
            cord[1] = '8';
            break;
    }
}

void jumping(Piece fromPiece, Piece toPiece, char currentSide, char otherSide) {
    while (true) {
        Pair pair = isJump(fromPiece, toPiece, otherSide);
        if (pair.valid) {
            jump(fromPiece, toPiece, pair.middle);
            fromPiece = toPiece;
            while (canJump(fromPiece, currentSide).valid) {
                printBoard();
                char option;
                printf("Jump Again? (y/n): \n");
                scanf("%c", &option);
                printf("\n");
                if (option == 'y') {
                    char cords[2];
                    getUserCoordinates(fromPiece, cords);
                    printf("Start %s\n", cords);
                    toPiece = checkBounds("to");
                } else {
                    break;
                }
            }
        } else {
            char option;
            printf("Invalid jump\n");
            printf("stop (y/n): ");
            scanf("%c", &option);
            printf("\n");
            if (option == 'y') {
                break;
            }
        }
    }
}

bool move(Piece fromPiece, Piece toPiece, char currentSide) {

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

    if (!isSlide(fromPiece, toPiece)) {
        jumping(fromPiece, toPiece, currentSide, otherSide);
    } else {
        slide(fromPiece, toPiece);
    }
    return true;
}

Piece getChoice(char indicator[], char currentSide) {
    bool validResult = false;
    Piece piece;
    while (!validResult) {
        piece = checkBounds(indicator);
        int value = strcmp(indicator, "from");
        if (value == 0) {
            if (piece.side == currentSide) {
                validResult = true;
            } else {
                printf("Invalid option\n");
            }
        } else {
            if (piece.side == ' ') {
                validResult = true;
            } else {
                printf("Invalid option\n");
            }
        }
    }
    return piece;
}

void makeMove(Name name, char currentSide) {
    bool valid = false;
    while (!valid) {
        printf("%s turn", name);
        if (move(getChoice("from", currentSide), getChoice("to", currentSide), currentSide)) {
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
    // check if no one can move
    for (int i = 0; i < NUMPIECES; i++) {
        Piece piece = list[i];
        if (piece.isAlive && canSlide(piece)) {
            return false;
        }
    }
    return true;
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
