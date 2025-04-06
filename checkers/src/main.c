#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 8

// ------------------------- Data Structures -------------------------

/**
 * @brief Structure representing a game piece.
 *
 * The Piece structure holds information about a checker piece including its position,
 * whether it is alive, its type ('n' for normal, 'k' for king), and its side ('X' or 'O').
 */
typedef struct {
    int row;       ///< Row position of the piece on the board.
    int col;       ///< Column position of the piece on the board.
    bool isAlive;  ///< Flag indicating if the piece is active.
    char type;     ///< Type of the piece ('n' for normal, 'k' for king).
    char side;     ///< Side of the piece ('X' or 'O'); a space (' ') represents an empty cell.
} Piece;

/**
 * @brief Structure representing the result of a jump validation.
 *
 * This structure is used to return whether a jump move is valid and the coordinates
 * of the piece that would be jumped over.
 */
typedef struct {
    bool valid;   ///< True if the jump is valid, false otherwise.
    int midRow;   ///< Row position of the jumped piece.
    int midCol;   ///< Column position of the jumped piece.
} JumpResult;

// ------------------------- Global Variables -------------------------

Piece board[BOARD_SIZE][BOARD_SIZE];  ///< The game board represented as a 2D array of Pieces.
int xCount = 12;                      ///< Count of active pieces for player 'X'.
int oCount = 12;                      ///< Count of active pieces for player 'O'.

// ------------------------- Function Prototypes -------------------------

// Board Setup & Display Functions
void initializeBoard(void);
void printBoard(void);
bool isWithinBounds(int row, int col);

// Input Handling Functions
void flushInputBuffer(void);
bool parseCoordinate(const char *input, int *row, int *col);
void getCoordinate(const char *prompt, int *row, int *col);

// Move Validation & Execution Functions
bool isValidSlide(int fromRow, int fromCol, int toRow, int toCol, char currentSide);
JumpResult checkJump(int fromRow, int fromCol, int toRow, int toCol, char currentSide);
void slidePiece(int fromRow, int fromCol, int toRow, int toCol);
bool performJump(int fromRow, int fromCol, int toRow, int toCol, char currentSide);
bool canJump(int row, int col, char currentSide);
bool anyForcedCaptureAvailable(char currentSide);

// Game Logic Functions
void playerMove(const char *playerName, char currentSide);
bool hasAvailableMoves(char currentSide);

// ------------------------- Utility Functions -------------------------

/**
 * @brief Clears any extra characters from the input buffer.
 *
 * This function reads and discards characters until a newline is encountered.
 */
void flushInputBuffer(void) {
    while (getchar() != '\n');
}

/**
 * @brief Checks if the given board coordinates are within valid bounds.
 *
 * @param row The row index.
 * @param col The column index.
 * @return true if (row, col) is within the board, false otherwise.
 */
bool isWithinBounds(const int row, const int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// ------------------------- Board Setup & Display Functions -------------------------

/**
 * @brief Initializes the game board.
 *
 * The board is first cleared by setting all cells to empty.
 * Then, pieces are placed on the dark squares of the board.
 * Player 'O' pieces are placed in the top three rows and player 'X'
 * pieces in the bottom three rows.
 */
void initializeBoard(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].row = i;
            board[i][j].col = j;
            board[i][j].isAlive = false;
            board[i][j].type = 'n';
            board[i][j].side = ' ';
        }
    }
    // Place Player 'O' pieces (assumed to move downward)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                board[i][j].isAlive = true;
                board[i][j].side = 'O';
                board[i][j].type = 'n';
            }
        }
    }
    // Place Player 'X' pieces (assumed to move upward)
    for (int i = BOARD_SIZE - 3; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                board[i][j].isAlive = true;
                board[i][j].side = 'X';
                board[i][j].type = 'n';
            }
        }
    }
    xCount = 12;
    oCount = 12;
}

/**
 * @brief Displays the current state of the board.
 *
 * The board is printed with row labels (A-H) and column labels (1-8).
 * Empty squares are shown as dots ('.').
 * King pieces are marked with an asterisk (*).
 */
void printBoard(void) {
    printf("\n    1  2  3  4  5  6  7  8\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("  +------------------------+\n");
        printf("%c |", 'A' + i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].side == ' ') {
                printf(" . ");
            } else if (board[i][j].type == 'k') {
                printf(" %c*", board[i][j].side);
            } else {
                printf(" %c ", board[i][j].side);
            }
        }
        printf("|\n");
    }
    printf("  +------------------------+\n\n");
}

// ------------------------- Input Handling Functions -------------------------

/**
 * @brief Converts a coordinate string (e.g., "A1") into numeric row and column indices.
 *
 * @param input The input string representing the coordinate.
 * @param row Pointer to store the resulting row index.
 * @param col Pointer to store the resulting column index.
 * @return true if the conversion is successful, false otherwise.
 */
bool parseCoordinate(const char *input, int *row, int *col) {
    if (input == NULL || strlen(input) < 2)
        return false;
    const char rowChar = (char) toupper(input[0]);
    const char colChar = input[1];
    if (rowChar < 'A' || rowChar >= 'A' + BOARD_SIZE)
        return false;
    if (colChar < '1' || colChar > '8')
        return false;
    *row = rowChar - 'A';
    *col = colChar - '1';
    return true;
}

/**
 * @brief Prompts the user to enter a coordinate until a valid one is provided.
 *
 * @param prompt A string prompt to display.
 * @param row Pointer to store the resulting row index.
 * @param col Pointer to store the resulting column index.
 */
void getCoordinate(const char *prompt, int *row, int *col) {
    while (1) {char input[10];
        printf("%s (e.g., A1): ", prompt);
        if (scanf("%9s", input) != 1) {
            printf("Invalid input. Try again.\n");
            flushInputBuffer();
            continue;
        }
        flushInputBuffer();
        if (parseCoordinate(input, row, col))
            break;
        else
            printf("Invalid coordinate. Please enter again.\n");
    }
}

// ------------------------- Move Validation & Execution Functions -------------------------

/**
 * @brief Checks whether a slide move is valid.
 *
 * A slide move moves a piece one diagonal step into an empty square.
 * For normal pieces, the move must be in the forward direction.
 *
 * @param fromRow Starting row.
 * @param fromCol Starting column.
 * @param toRow Destination row.
 * @param toCol Destination column.
 * @param currentSide The side of the piece ('X' or 'O').
 * @return true if the slide move is valid, false otherwise.
 */
bool isValidSlide(const int fromRow, const int fromCol, const int toRow, const int toCol, const char currentSide) {
    if (!isWithinBounds(toRow, toCol) || board[toRow][toCol].side != ' ')
        return false;
    if (abs(toRow - fromRow) != 1 || abs(toCol - fromCol) != 1)
        return false;
    // For normal pieces, enforce forward movement.
    if (board[fromRow][fromCol].type == 'n') {
        if (currentSide == 'O' && (toRow - fromRow) != 1)
            return false;
        if (currentSide == 'X' && (toRow - fromRow) != -1)
            return false;
    }
    return true;
}

/**
 * @brief Checks whether a jump move is valid.
 *
 * A jump move involves moving two diagonal steps over an opponent's piece.
 * For normal pieces, the jump must be forward.
 *
 * @param fromRow Starting row.
 * @param fromCol Starting column.
 * @param toRow Destination row.
 * @param toCol Destination column.
 * @param currentSide The side of the moving piece.
 * @return A JumpResult structure containing the validity and the position of the jumped piece.
 */
JumpResult checkJump(const int fromRow, const int fromCol, const int toRow, const int toCol, const char currentSide) {
    JumpResult result = { false, -1, -1 };
    if (!isWithinBounds(toRow, toCol) || abs(toRow - fromRow) != 2 || abs(toCol - fromCol) != 2)
        return result;
    if (board[toRow][toCol].side != ' ')
        return result;
    // For normal pieces, enforce forward jump.
    if (board[fromRow][fromCol].type == 'n') {
        if (currentSide == 'O' && (toRow - fromRow) != 2)
            return result;
        if (currentSide == 'X' && (toRow - fromRow) != -2)
            return result;
    }
    const int midRow = (fromRow + toRow) / 2;
    const int midCol = (fromCol + toCol) / 2;
    const char opponent = (currentSide == 'O') ? 'X' : 'O';
    if (board[midRow][midCol].side != opponent)
        return result;
    result.valid = true;
    result.midRow = midRow;
    result.midCol = midCol;
    return result;
}

/**
 * @brief Slides a piece from one position to another.
 *
 * The function moves the piece, updates its position, and clears the source cell.
 * It also handles king promotion if the piece reaches the opposite end.
 *
 * @param fromRow Starting row.
 * @param fromCol Starting column.
 * @param toRow Destination row.
 * @param toCol Destination column.
 */
void slidePiece(const int fromRow, const int fromCol, const int toRow, const int toCol) {
    board[toRow][toCol] = board[fromRow][fromCol];
    board[toRow][toCol].row = toRow;
    board[toRow][toCol].col = toCol;

    // Clear the original cell.
    board[fromRow][fromCol].isAlive = false;
    board[fromRow][fromCol].side = ' ';
    board[fromRow][fromCol].type = 'n';

    // Promote to king if applicable.
    if ((board[toRow][toCol].side == 'O' && toRow == BOARD_SIZE - 1) ||
        (board[toRow][toCol].side == 'X' && toRow == 0)) {
        board[toRow][toCol].type = 'k';
    }
}

/**
 * @brief Executes a jump move.
 *
 * The function validates the jump move, removes the opponent's piece,
 * and then moves the jumping piece to the destination.
 *
 * @param fromRow Starting row.
 * @param fromCol Starting column.
 * @param toRow Destination row.
 * @param toCol Destination column.
 * @param currentSide The side of the jumping piece.
 * @return true if the jump was successfully performed, false otherwise.
 */
bool performJump(const int fromRow, const int fromCol, const int toRow, const int toCol, const char currentSide) {
    const JumpResult jr = checkJump(fromRow, fromCol, toRow, toCol, currentSide);
    if (!jr.valid) {
        printf("Invalid jump move.\n");
        return false;
    }
    // Remove the opponent's piece.
    if (board[jr.midRow][jr.midCol].side == 'O')
        oCount--;
    else if (board[jr.midRow][jr.midCol].side == 'X')
        xCount--;

    board[jr.midRow][jr.midCol].isAlive = false;
    board[jr.midRow][jr.midCol].side = ' ';
    board[jr.midRow][jr.midCol].type = 'n';

    slidePiece(fromRow, fromCol, toRow, toCol);
    return true;
}

/**
 * @brief Checks if the piece at a given position can perform any jump.
 *
 * This function tests all four diagonal directions for a valid jump move.
 *
 * @param row The row of the piece.
 * @param col The column of the piece.
 * @param currentSide The side of the piece.
 * @return true if at least one jump is possible, false otherwise.
 */
bool canJump(const int row, const int col, const char currentSide) {
    const int directions[4][2] = { {2, 2}, {2, -2}, {-2, 2}, {-2, -2} };
    for (int i = 0; i < 4; i++) {
        const int newRow = row + directions[i][0];
        const int newCol = col + directions[i][1];
        const JumpResult jr = checkJump(row, col, newRow, newCol, currentSide);
        if (jr.valid)
            return true;
    }
    return false;
}

/**
 * @brief Determines if any forced capture (jump move) is available for the current side.
 *
 * @param currentSide The side to check ('X' or 'O').
 * @return true if a forced capture is available, false otherwise.
 */
bool anyForcedCaptureAvailable(const char currentSide) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].side == currentSide && canJump(i, j, currentSide))
                return true;
        }
    }
    return false;
}

// ------------------------- Game Logic Functions -------------------------

/**
 * @brief Checks if the current side has any legal moves available.
 *
 * This includes both slide moves and jump moves.
 *
 * @param currentSide The side to check ('X' or 'O').
 * @return true if there is at least one legal move, false otherwise.
 */
bool hasAvailableMoves(const char currentSide) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].side == currentSide) {
                if (canJump(i, j, currentSide))
                    return true;
                const int slideDirs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
                for (int d = 0; d < 4; d++) {
                    const int newRow = i + slideDirs[d][0];
                    const int newCol = j + slideDirs[d][1];
                    if (isWithinBounds(newRow, newCol) && board[newRow][newCol].side == ' ') {
                        if (board[i][j].type == 'n') {
                            if (currentSide == 'O' && (newRow - i) != 1)
                                continue;
                            if (currentSide == 'X' && (newRow - i) != -1)
                                continue;
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * @brief Processes a single player's move.
 *
 * This function repeatedly prompts the player to select a piece and a destination until
 * a valid move is executed. It enforces forced capture rules and handles both slide and jump moves.
 *
 * @param playerName The name of the player.
 * @param currentSide The side of the player ('X' or 'O').
 */
void playerMove(const char *playerName, const char currentSide) {
    while (1) {
        printBoard();
        printf("%s's turn (%c)\n", playerName, currentSide);
        int fromRow, fromCol, toRow, toCol;
        const bool forcedCapture = anyForcedCaptureAvailable(currentSide);

        // Prompt the player to select a valid piece.
        while (1) {
            getCoordinate("Select piece to move", &fromRow, &fromCol);
            if (board[fromRow][fromCol].side != currentSide) {
                printf("That is not your piece. Try again.\n");
                continue;
            }
            if (forcedCapture && !canJump(fromRow, fromCol, currentSide)) {
                printf("A capture is available; you must select a piece that can jump.\n");
                continue;
            }
            break;
        }

        // Prompt for destination.
        getCoordinate("Select destination", &toRow, &toCol);

        // Attempt to execute the move.
        if (abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 2) {
            if (!performJump(fromRow, fromCol, toRow, toCol, currentSide)) {
                printf("Invalid jump. Please try again.\n");
                continue;
            }
            // Enforce multiple jumps if available.
            while (canJump(toRow, toCol, currentSide)) {
                printBoard();
                char choice;
                printf("You must continue capturing. Jump again? (y/n): ");
                scanf(" %c", &choice);
                flushInputBuffer();
                if (choice != 'y' && choice != 'Y')
                    break;
                fromRow = toRow;
                fromCol = toCol;
                getCoordinate("Select next jump destination", &toRow, &toCol);
                if (!performJump(fromRow, fromCol, toRow, toCol, currentSide)) {
                    printf("Invalid jump. Please try again.\n");
                    break;
                }
            }
            break; // Move executed.
        } else if (abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 1) {
            if (forcedCapture) {
                printf("A capture is available; you must jump. Please try again.\n");
                continue;
            }
            if (!isValidSlide(fromRow, fromCol, toRow, toCol, currentSide)) {
                printf("Invalid slide move. Please try again.\n");
                continue;
            }
            slidePiece(fromRow, fromCol, toRow, toCol);
            break; // Move executed.
        } else {
            printf("Invalid move. Moves must be diagonal. Please try again.\n");
        }
    }
}

// ------------------------- Main Function -------------------------

/**
 * @brief Entry point of the Checkers game.
 *
 * This function handles game setup, displays the rules, and processes the main game loop.
 *
 * @return int Exit status of the program.
 */
int main(void) {
    char player1[50], player2[50];

    printf("=== Checkers Game ===\n");
    printf("--- Game Rules ---\n");
    printf("Setup:\n");
    printf("  - An 8x8 board with pieces on dark squares only.\n");
    printf("  - Each player starts with 12 pieces on the 12 dark squares closest to them.\n");
    printf("Gameplay:\n");
    printf("  - Pieces move diagonally. A slide is one step into an empty square.\n");
    printf("  - To capture, jump over an opponent's piece into an empty square (forced capture).\n");
    printf("  - Non-king pieces move and jump only forward.\n");
    printf("  - Upon reaching the opposite end, a piece is promoted to king (indicated with *),\n");
    printf("    and can move both forward and backward.\n");
    printf("Winning:\n");
    printf("  - The game ends when a player has no legal moves.\n");

    printf("Enter Player 1 name (X): ");
    scanf("%s", player1);
    flushInputBuffer();
    printf("Enter Player 2 name (O): ");
    scanf("%s", player2);
    flushInputBuffer();

    initializeBoard();

    // Main game loop.
    while (true) {
        if (!hasAvailableMoves('X')) {
            printf("%s has no moves left. %s wins!\n", player1, player2);
            break;
        }
        playerMove(player1, 'X');
        if (xCount <= 0 || oCount <= 0)
            break;

        if (!hasAvailableMoves('O')) {
            printf("%s has no moves left. %s wins!\n", player2, player1);
            break;
        }
        playerMove(player2, 'O');
        if (xCount <= 0 || oCount <= 0)
            break;
    }

    if (xCount <= 0)
        printf("%s wins!\n", player2);
    else if (oCount <= 0)
        printf("%s wins!\n", player1);
    else
        printf("Game over.\n");

    return 0;
}
