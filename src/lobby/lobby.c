#include <stdbool.h>
#include "lobby.h"
#include "../connection/connection.h"

static AppWindow *window;
static GtkGrid *lobbyGrid;
static bool connectionEstablished;
static int chosenColumns;

static GtkGrid *player1Controls;
static GtkLabel *waitingForPlayerLabel;
static PlayerType playerType;

static void init_player_1_controls(GtkGrid *controls);

void lobby_init(AppWindow *win, GtkGrid *lobby) {
    window = win;
    lobbyGrid = lobby;

    GList *children = gtk_container_get_children(GTK_CONTAINER(lobby));

    for (GList *elem = children; elem != NULL; elem = elem->next) {
        gpointer data = elem->data;

        GtkWidget *widget = GTK_WIDGET(data);
        const gchar *name = gtk_widget_get_name(widget);

        if (strcmp(name, "lobby_player_1_controls") == 0) {
            player1Controls = GTK_GRID(widget);
            init_player_1_controls(GTK_GRID(widget));
        } else if (strcmp(name, "lobby_waiting_for_player_label") == 0) {
            waitingForPlayerLabel = GTK_LABEL(widget);
        }
    }
}

static void choose_columns(GtkWidget *widget) {
    const gchar *name = gtk_widget_get_name(widget);

    chosenColumns = 4;
    if (strcmp(name, "lobby_choose_columns_5_button") == 0) {
        chosenColumns = 5;
    }

    gtk_widget_hide(GTK_WIDGET(player1Controls));
    gtk_widget_show(GTK_WIDGET(waitingForPlayerLabel));
    connection_init(window, PLAYER_ONE);
}

static void init_player_1_controls(GtkGrid *controls) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(controls));

    for (GList *elem = children; elem != NULL; elem = elem->next) {
        gpointer data = elem->data;

        GtkWidget *widget = GTK_WIDGET(data);
        const gchar *name = gtk_widget_get_name(widget);

        if (strcmp(name, "lobby_choose_columns_4_button") == 0) {
            g_signal_connect(widget, "clicked", G_CALLBACK(choose_columns), NULL);
        } else if (strcmp(name, "lobby_choose_columns_5_button") == 0) {
            g_signal_connect(widget, "clicked", G_CALLBACK(choose_columns), NULL);
        }
    }
}

void lobby_show(PlayerType pType) {
    app_window_show_error(window, "This is an error");

    playerType = pType;
    connectionEstablished = false;
    chosenColumns = 0;

    if (playerType == PLAYER_ONE) {
        gtk_widget_show(GTK_WIDGET(player1Controls));
        gtk_widget_hide(GTK_WIDGET(waitingForPlayerLabel));
    } else {
        gtk_widget_hide(GTK_WIDGET(player1Controls));
        gtk_widget_show(GTK_WIDGET(waitingForPlayerLabel));
    }

    gchar *text;
    if (playerType == PLAYER_ONE) {
        text = "Waiting for Player 2";
    } else {
        text = "Waiting for Player 1";
    }
    gtk_label_set_text(GTK_LABEL(waitingForPlayerLabel), text);

    if (playerType == PLAYER_TWO) {
        connection_init(window, PLAYER_TWO);
    }
}

//For player 1
void lobby_connection_established() {
    if (playerType == PLAYER_ONE) {
        connection_send_columns(chosenColumns);
        app_window_start_game(window, chosenColumns, playerType);
    }
}

//For player 2
void lobby_columns_received(int columns) {
    chosenColumns = columns;
    app_window_start_game(window, chosenColumns, playerType);
}
