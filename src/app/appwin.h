#ifndef TIC_TAC_TOE_C_APPWIN_H
#define TIC_TAC_TOE_C_APPWIN_H

#include <gtk/gtk.h>
#include "app.h"

#define APP_WINDOW_TYPE (app_window_get_type())

G_DECLARE_FINAL_TYPE (AppWindow, app_window, APP, WINDOW, GtkApplicationWindow)

AppWindow *app_window_new(App *app);

#endif //TIC_TAC_TOE_C_APPWIN_H
