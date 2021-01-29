
#include <gtk/gtk.h>
#include <stdlib.h>
#include "app.h"
#include "../menu/menu.h"
#include "../connection/connection.h"
#include "../lobby/lobby.h"
#include "../how_to_play/how_to_play.h"
#include "../game/game.h"
#include "../modals/modals.h"

typedef struct pages {
    GtkStack *stack;
    GtkGrid *mainMenuPage;
    GtkGrid *gameplayPage;
    GtkGrid *howToPlayPage;
    GtkGrid *lobbyPage;
    GtkGrid *lostConnectionPage;
} Pages;

static GtkBuilder *builder;
static GtkWindow *window;
static Pages pages;

static void app_show_page(GtkGrid *page);
static void init_pages();
static void app_play_as_1_with_size(int size);

void app_init(int argc, char *argv[]) {
    GError *error = NULL;

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    if (gtk_builder_add_from_resource(builder, "/com/github/bartacc/tictactoe-c/UITemplate.glade", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    /* Add CSS */
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/github/bartacc/tictactoe-c/style.css");
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    /* Connect signal handlers to the constructed widgets. */
    window = GTK_WINDOW(gtk_builder_get_object(builder, "appWindow"));
    gtk_window_set_default_size(window, 350, 350);
    g_signal_connect (window, "destroy", G_CALLBACK(app_quit), NULL);

    init_pages();

    if (argc > 1 && strcmp(argv[1], "debug") == 0) {
        putenv("G_MESSAGES_DEBUG=all");
    }

    gtk_widget_show_all(GTK_WIDGET(window));
}

static void init_pages() {
    pages.stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    pages.mainMenuPage = GTK_GRID(gtk_builder_get_object(builder, "mainMenuPage"));
    pages.gameplayPage = GTK_GRID(gtk_builder_get_object(builder, "gameplayPage"));
    pages.howToPlayPage = GTK_GRID(gtk_builder_get_object(builder, "howToPlayPage"));
    pages.lobbyPage = GTK_GRID(gtk_builder_get_object(builder, "lobbyPage"));
    pages.lostConnectionPage = GTK_GRID(gtk_builder_get_object(builder, "lostConnectionPage"));

    modals_init(window);
    menu_init(builder);
    lobby_init(builder);
    how_to_play_init(builder);
    game_init(builder);

    app_menu();
}

static void app_show_page(GtkGrid *page) {
    gtk_stack_set_visible_child(pages.stack, GTK_WIDGET(page));
}

void app_add_css_class_to_widget(GtkWidget *widget, const gchar *cssClass) {
    GtkStyleContext *styleContext = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(styleContext, cssClass);

}

void app_remove_css_class_from_widget(GtkWidget *widget, const gchar *cssClass) {
    GtkStyleContext *styleContext = gtk_widget_get_style_context(widget);
    gtk_style_context_remove_class(styleContext, cssClass);
}

void app_menu() {
    app_show_page(pages.mainMenuPage);
}

void app_play_as_1() {
    lobby_show(PLAYER_ONE);
    app_show_page(pages.lobbyPage);
}

void app_play_as_2() {
    lobby_show(PLAYER_TWO);
    app_show_page(pages.lobbyPage);
}

void app_how_to_play_show(GtkWidget *widget, const gchar *fromPage) {
    how_to_play_show(fromPage);
    app_show_page(pages.howToPlayPage);
}

void app_how_to_play_hide(GtkWidget *widget, const gchar *backToPage) {
    if (strcmp(backToPage, MENU_PAGE) == 0) {
        app_show_page(pages.mainMenuPage);
    } else if (strcmp(backToPage, GAMEPLAY_PAGE) == 0) {
        app_show_page(pages.gameplayPage);
    }
}

void app_start_game(int size, PlayerType pType) {
    game_start(size, pType);
    app_show_page(pages.gameplayPage);
}

void app_show_error(char *error) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", error);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void app_quit() {
    connection_drop();
    gtk_main_quit();
}
