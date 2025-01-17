#ifndef TIC_TAC_TOE_C_GAME_GTK_UTILS_H
#define TIC_TAC_TOE_C_GAME_GTK_UTILS_H

#include <gtk/gtk.h>
#include "../../constants/constants.h"

void game_get_event_box_coordinates(GtkGrid *board, GtkWidget *eventBox, int *x, int *y);
void game_set_winning_elements_background(GtkGrid *board, const int winnerSequence[], int elementsInSequence, bool clear);
void game_set_element_label(GtkGrid *board, int x, int y, Element element, PlaceholderType placeholderType);
void game_set_state_label(GtkLabel *stateLabel, PlayerType whoseTurn, PlayerType player);
void game_set_won_lost_count_label(GtkLabel *wonLostCountLabel, int gamesWon, int gamesLost);

#endif //TIC_TAC_TOE_C_GAME_GTK_UTILS_H
