#ifndef TIC_TAC_TOE_C_APP_H
#define TIC_TAC_TOE_C_APP_H

#include <gtk/gtk.h>

#define APP_TYPE (app_get_type())

G_DECLARE_FINAL_TYPE (App, app, APP, APP, GtkApplication)

App *app_new(void);

#endif //TIC_TAC_TOE_C_APP_H
