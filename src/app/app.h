#ifndef TIC_TAC_TOE_C_APP_H
#define TIC_TAC_TOE_C_APP_H

#include "../constants/constants.h"
#include "../how_to_play/how_to_play.h"

void app_init();
void app_activate(GtkApplication *app, gpointer user_data);

void app_add_css_class_to_widget(GtkWidget *widget, const gchar *cssClass);
void app_remove_css_class_from_widget(GtkWidget *widget, const gchar *cssClass);

void app_menu();
void app_play_as_1();
void app_play_as_2();
void app_how_to_play_show(GtkWidget *widget, const gchar *fromPage);
void app_how_to_play_hide(GtkWidget *widget, const gchar *backToPage);
void app_start_game(int columns, PlayerType pType);
void app_show_error(char *error);
void app_quit();

#endif //TIC_TAC_TOE_C_APP_H
