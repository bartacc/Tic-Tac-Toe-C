#include <stdio.h>
#include <gtk/gtk.h>
#include "game_utils.h"
#include "game_gtk_utils.h"

static PlayerType check_winner_sequence_linear(Element boardElements[BOARD_SIZE][BOARD_SIZE],
                                               int elementsToWin, bool checkColumns);
static PlayerType check_winner_sequence_diagonal_up(Element boardElements[BOARD_SIZE][BOARD_SIZE],
                                                    int elementsToWin, int startX, int startY, bool checkRightDiagonal);

PlayerType game_check_winner(Element boardElements[BOARD_SIZE][BOARD_SIZE], int elementsToWin) {
    PlayerType winner;

    //Columns
    winner = check_winner_sequence_linear(boardElements, elementsToWin, true);
    if (winner != PLAYER_NONE) {
        return winner;
    }

    //Rows
    winner = check_winner_sequence_linear(boardElements, elementsToWin, false);
    if (winner != PLAYER_NONE) {
        return winner;
    }

    //Diagonal
    for (int y = 0; y < BOARD_SIZE; y++) {
        winner = check_winner_sequence_diagonal_up(boardElements, elementsToWin, 0, y, true);
        if (winner != PLAYER_NONE) {
            return winner;
        }

        winner = check_winner_sequence_diagonal_up(boardElements, elementsToWin, BOARD_SIZE - 1, y, false);
        if (winner != PLAYER_NONE) {
            return winner;
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        winner = check_winner_sequence_diagonal_up(boardElements, elementsToWin, x, BOARD_SIZE - 1, true);
        if (winner != PLAYER_NONE) {
            return winner;
        }

        winner = check_winner_sequence_diagonal_up(boardElements, elementsToWin, x, BOARD_SIZE - 1, false);
        if (winner != PLAYER_NONE) {
            return winner;
        }
    }

    return PLAYER_NONE;
}

static PlayerType check_winner_sequence_linear(Element boardElements[BOARD_SIZE][BOARD_SIZE], int elementsToWin, bool checkColumns) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        Element startElem = checkColumns ? boardElements[i][0] : boardElements[0][i];
        int elemsInSequence = 1;
        for (int j = 1; j < BOARD_SIZE; j++) {
            int x = checkColumns ? i : j;
            int y = checkColumns ? j : i;

            if (boardElements[x][y] == startElem && startElem != EMPTY) {
                elemsInSequence++;
            } else {
                startElem = boardElements[x][y];
                elemsInSequence = 1;
            }

            if (elemsInSequence >= elementsToWin) {
                PlayerType winner = get_player(startElem);
                printf("Player %s wins in columns!\n", winner == PLAYER_ONE ? "1" : "2");
                return winner;
            }
        }
    }

    return PLAYER_NONE;
}

static PlayerType check_winner_sequence_diagonal_up(Element boardElements[BOARD_SIZE][BOARD_SIZE], int elementsToWin, int startX, int startY,
                                                    bool checkRightDiagonal) {
    Element startElem = boardElements[startX][startY];
    int elemsInSequence = 1;

    int x = checkRightDiagonal ? startX + 1 : startX - 1;
    int y = startY - 1;

    while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        if (boardElements[x][y] == startElem && startElem != EMPTY) {
            elemsInSequence++;
        } else {
            startElem = boardElements[x][y];
            elemsInSequence = 1;
        }

        if (elemsInSequence >= elementsToWin) {
            PlayerType winner = get_player(startElem);
            printf("Player %s wins diagonally!\n", winner == PLAYER_ONE ? "1" : "2");
            return winner;
        }

        x = checkRightDiagonal ? x + 1 : x - 1;
        y--;
    }

    return PLAYER_NONE;
}

void game_push_element_to_column(Element boardElements[BOARD_SIZE][BOARD_SIZE],
                                 GtkGrid *board, int column, Element element) {
    int y = game_get_first_empty_y_in_column(boardElements, column);

    boardElements[column][y] = element;
    game_set_element_label(board, column, y, boardElements[column][y], NOT_PLACEHOLDER);
}

bool game_is_move_possible(Element boardElements[BOARD_SIZE][BOARD_SIZE], int column) {
    return boardElements[column][0] == EMPTY;
}

bool game_are_any_moves_left(Element boardElements[BOARD_SIZE][BOARD_SIZE]) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (boardElements[x][y] == EMPTY) {
                return true;
            }
        }
    }

    return false;
}

int game_get_first_empty_y_in_column(Element boardElements[BOARD_SIZE][BOARD_SIZE], int column) {
    int y = BOARD_SIZE - 1;
    while (boardElements[column][y] != EMPTY) {
        y--;
    }
    return y;
}
