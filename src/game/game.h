#ifndef TIC_TAC_TOE_C_GAME_H
#define TIC_TAC_TOE_C_GAME_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../constants/constants.h"

void game_init(GtkBuilder *builder);
void game_start(int elementsToWin, PlayerType pType);
void game_move_push(int column, bool wasColumnReplaced);
void game_move_replace(int column);
void game_opponent_concede();

#endif //TIC_TAC_TOE_C_GAME_H
