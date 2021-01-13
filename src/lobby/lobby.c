#include <stdbool.h>
#include "lobby.h"
#include "../connection/connection.h"
#include "../app/app.h"

static GtkGrid *player1Controls;
static GtkLabel *waitingForPlayerLabel;
static PlayerType playerType;
static int chosenSize;

static void choose_size(int size);
static void choose_size_4();
static void choose_size_5();

void lobby_init(GtkBuilder *builder) {
    GtkButton *choose4, *choose5;

    player1Controls = GTK_GRID(gtk_builder_get_object(builder, "lobby_player_1_controls"));
    choose4 = GTK_BUTTON(gtk_builder_get_object(builder, "lobby_choose_size_4_button"));
    choose5 = GTK_BUTTON(gtk_builder_get_object(builder, "lobby_choose_size_5_button"));

    waitingForPlayerLabel = GTK_LABEL(gtk_builder_get_object(builder, "lobby_waiting_for_player_label"));

    g_signal_connect(choose4, "clicked", G_CALLBACK(choose_size_4), NULL);
    g_signal_connect(choose5, "clicked", G_CALLBACK(choose_size_5), NULL);

}

static void choose_size_4() {
    choose_size(4);
}

static void choose_size_5() {
    choose_size(5);
}

static void choose_size(int size) {
    chosenSize = size;

    gtk_widget_hide(GTK_WIDGET(player1Controls));
    gtk_widget_show(GTK_WIDGET(waitingForPlayerLabel));
    connection_init(PLAYER_ONE);
}

void lobby_show(PlayerType pType) {
    playerType = pType;
    chosenSize = 0;

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
        connection_init(PLAYER_TWO);
    }
}

//For player 1
void lobby_connection_established() {
    if (playerType == PLAYER_ONE) {
        connection_send_columns(chosenSize);
        app_start_game(chosenSize, playerType);
    }
}

//For player 2
void lobby_columns_received(int columns) {
    chosenSize = columns;
    app_start_game(chosenSize, playerType);
}

//Used for starting the game from command-line
void lobby_show_with_size(PlayerType pType, int size) {
    lobby_show(pType);
    choose_size(size);
}
