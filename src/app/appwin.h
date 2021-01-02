#ifndef TIC_TAC_TOE_C_APPWIN_H
#define TIC_TAC_TOE_C_APPWIN_H

#include "app.h"

#define APP_WINDOW_TYPE (app_window_get_type())

G_DECLARE_FINAL_TYPE (AppWindow, app_window, APP, WINDOW, GtkApplicationWindow)

AppWindow *app_window_new(App *app);

void app_window_play_as_1(GtkWidget *widget, AppWindow *win);

void app_window_play_as_2(GtkWidget *widget, AppWindow *win);

void app_window_how_to_play(GtkWidget *widget, AppWindow *win);

void app_window_quit(GtkWidget *widget, AppWindow *win);

#endif //TIC_TAC_TOE_C_APPWIN_H
