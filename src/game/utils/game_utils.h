#ifndef TIC_TAC_TOE_C_GAME_UTILS_H
#define TIC_TAC_TOE_C_GAME_UTILS_H

#include <stdbool.h>
#include "../../constants/constants.h"

extern int game_winner_sequence[MAX_WINNING_SEQUENCE * 2]; //Array of MAX_WINNING_SEQUENCE (x,y) pairs

PlayerType game_check_winner(Element boardElements[BOARD_SIZE][BOARD_SIZE], int elementsToWin);
void game_clear_winner_sequence_array();
void game_push_element_to_column(Element boardElements[BOARD_SIZE][BOARD_SIZE],
                                 GtkGrid *board, int column, Element element);
bool game_is_move_possible(Element boardElements[BOARD_SIZE][BOARD_SIZE], int column);
bool game_are_any_moves_left(Element boardElements[BOARD_SIZE][BOARD_SIZE]);
int game_get_first_empty_y_in_column(Element boardElements[BOARD_SIZE][BOARD_SIZE], int column);

#endif //TIC_TAC_TOE_C_GAME_UTILS_H
