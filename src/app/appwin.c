#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"
#include "../menu/menu.h"

struct _AppWindow {
    GtkApplicationWindow parent;
    GtkStack *stack;

    App *app;

    GtkGrid *mainMenuPage;
    GtkGrid *gameplayPage;
    GtkGrid *howToPlayPage;
    GtkGrid *lobbyPage;
    GtkGrid *lostConnectionPage;
};

G_DEFINE_TYPE(AppWindow, app_window, GTK_TYPE_APPLICATION_WINDOW);

static void app_window_init(AppWindow *win) {
    gtk_widget_init_template(GTK_WIDGET(win));

    menu_init(win, win->mainMenuPage);

    gtk_stack_set_visible_child(win->stack, GTK_WIDGET(win->mainMenuPage));
}

static void app_window_class_init(AppWindowClass *class) {
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/github/bartacc/tictactoe-c/UITemplate.glade");

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, stack);

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, mainMenuPage);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, gameplayPage);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, howToPlayPage);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, lobbyPage);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), AppWindow, lostConnectionPage);
}

AppWindow *app_window_new(App *app) {
    AppWindow *win = g_object_new(APP_WINDOW_TYPE, "application", app, NULL);
    win->app = app;
    return win;
}

void app_window_play_as_1(GtkWidget *widget, AppWindow *win) {
    gtk_stack_set_visible_child(win->stack, GTK_WIDGET(win->lobbyPage));
}

void app_window_play_as_2(GtkWidget *widget, AppWindow *win) {
    gtk_stack_set_visible_child(win->stack, GTK_WIDGET(win->lobbyPage));
}

void app_window_how_to_play(GtkWidget *widget, AppWindow *win) {
    gtk_stack_set_visible_child(win->stack, GTK_WIDGET(win->howToPlayPage));
}

void app_window_quit(GtkWidget *widget, AppWindow *win) {
    g_application_quit(G_APPLICATION(win->app));
}

