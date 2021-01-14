#ifndef TIC_TAC_TOE_C_MODALS_H
#define TIC_TAC_TOE_C_MODALS_H

#include <gtk/gtk.h>
#include "../constants/constants.h"

void modals_init(GtkWindow *win);
void modal_end_game(PlayerType *currentPlayer, PlayerType winner);
void modal_play_again(PlayerType *currentPlayer);

#endif //TIC_TAC_TOE_C_MODALS_H
