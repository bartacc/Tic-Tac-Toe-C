#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"

struct _App {
    GtkApplication parent;
};

G_DEFINE_TYPE(App, app, GTK_TYPE_APPLICATION);

static void app_init(App *app) {}

static void app_activate(GApplication *gApp) {
    AppWindow *win;

    win = app_window_new(APP_APP(gApp));
    gtk_window_present(GTK_WINDOW(win));
}

static void app_class_init(AppClass *class) {
    G_APPLICATION_CLASS(class)->activate = app_activate;
}

App *app_new(void) {
    return g_object_new(APP_TYPE, "application-id", "com.github.bartacc.tictactoe-c",
                        "flags", G_APPLICATION_NON_UNIQUE, NULL);
}
