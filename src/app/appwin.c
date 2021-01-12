#include "appwin.h"

#include "../menu/menu.h"
#include "../how_to_play/how_to_play.h"
#include "../lobby/lobby.h"
#include "../connection/connection.h"


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
    lobby_init(win, win->lobbyPage);

    app_window_menu(NULL, win);
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

static void app_window_show_view(GtkGrid *view, AppWindow *win) {
    gtk_stack_set_visible_child(win->stack, GTK_WIDGET(view));
}

void app_window_menu(GtkWidget *widget, AppWindow *win) {
    connection_drop();
    app_window_show_view(win->mainMenuPage, win);
}

void app_window_play_as_1(GtkWidget *widget, AppWindow *win) {
    lobby_show(PLAYER_ONE);
    app_window_show_view(win->lobbyPage, win);
}

void app_window_play_as_2(GtkWidget *widget, AppWindow *win) {
    lobby_show(PLAYER_TWO);
    app_window_show_view(win->lobbyPage, win);
}

void app_window_how_to_play_show(GtkWidget *widget, AppWindow *win) {
    how_to_play_init(win, win->howToPlayPage, widget);
    app_window_show_view(win->howToPlayPage, win);
}

void app_window_how_to_play_hide(GtkWidget *widget, gpointer data) {
    HowToPlayCallbackData *d = data;
    const gchar *name = d->from_name;
    const AppWindow *win = d->window;

    if (strcmp(name, "menu_how_to_play_button") == 0) {
        app_window_show_view(win->mainMenuPage, win);
    } else if (strcmp(name, "gameplay_how_to_play_button") == 0) {
        app_window_show_view(win->gameplayPage, win);
    }

}

void app_window_start_game(AppWindow *win, int columns, PlayerType pType) {
    app_window_show_view(win->gameplayPage, win);
}

void app_window_show_error(AppWindow *win, char *error) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", error);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void app_window_quit(GtkWidget *widget, AppWindow *win) {
    g_application_quit(G_APPLICATION(win->app));
}

