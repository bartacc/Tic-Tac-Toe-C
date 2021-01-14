#include <stdbool.h>

#include "game.h"
#include "utils/game_utils.h"
#include "utils/game_gtk_utils.h"
#include "../app/app.h"
#include "../connection/connection.h"
#include "../modals/modals.h"

static const gchar *GAMEPLAY_PAGE_STRING = GAMEPLAY_PAGE;

static GtkGrid *board;
static GtkLabel *stateLabel, *wonLostCountLabel;

static Element boardElements[BOARD_SIZE][BOARD_SIZE];
static PlayerType player;
static PlayerType whoseTurn;
static int elementsToWin;

//Gtk callback functions
static gboolean on_hover_enter(GtkWidget *widget);
static gboolean on_hover_leave(GtkWidget *widget);
static gboolean on_frame_click(GtkWidget *widget);
static void play_again();

void game_init(GtkBuilder *builder) {
    GtkButton *quitButton, *howToPlayButton, *playAgainButton;

    board = GTK_GRID(gtk_builder_get_object(builder, "gameplay_board"));
    quitButton = GTK_BUTTON(gtk_builder_get_object(builder, "gameplay_quit_button"));
    howToPlayButton = GTK_BUTTON(gtk_builder_get_object(builder, "gameplay_how_to_play_button"));
    playAgainButton = GTK_BUTTON(gtk_builder_get_object(builder, "gameplay-play-again-button"));
    stateLabel = GTK_LABEL(gtk_builder_get_object(builder, "gameplay_state_label"));
    wonLostCountLabel = GTK_LABEL(gtk_builder_get_object(builder, "gameplay-won-lost-count-label"));

    g_signal_connect(quitButton, "clicked", G_CALLBACK(app_quit), NULL);
    g_signal_connect(howToPlayButton, "clicked", G_CALLBACK(app_how_to_play_show), GAMEPLAY_PAGE_STRING);
    g_signal_connect(playAgainButton, "clicked", G_CALLBACK(play_again), NULL);


    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE + 1; y++) {
            GtkEventBox *eventBox = GTK_EVENT_BOX(gtk_event_box_new());

            GtkFrame *frame = GTK_FRAME(gtk_frame_new(NULL));
            gtk_frame_set_shadow_type(frame, GTK_SHADOW_NONE);

            GtkLabel *elementLabel = GTK_LABEL(gtk_label_new(""));
            gtk_widget_set_halign(GTK_WIDGET(elementLabel), GTK_ALIGN_CENTER);
            gtk_widget_set_valign(GTK_WIDGET(elementLabel), GTK_ALIGN_CENTER);

            gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(elementLabel));
            gtk_container_add(GTK_CONTAINER(eventBox), GTK_WIDGET(frame));

            gtk_grid_attach(board, GTK_WIDGET(eventBox), x, y, 1, 1);

            if (y == 0) {
                app_add_css_class_to_widget(GTK_WIDGET(frame), CSS_GAMEPLAY_FRAME_FOR_PLACEHOLDER);
            } else {
                app_add_css_class_to_widget(GTK_WIDGET(frame), CSS_GAMEPLAY_FRAME);
                g_signal_connect(GTK_WIDGET(eventBox), "button-press-event", G_CALLBACK(on_frame_click), NULL);
                g_signal_connect(GTK_WIDGET(eventBox), "enter-notify-event", G_CALLBACK(on_hover_enter), NULL);
                g_signal_connect(GTK_WIDGET(eventBox), "leave-notify-event", G_CALLBACK(on_hover_leave), NULL);
            }
        }
    }
}

void game_start(int elementsInSequenceToWin, PlayerType pType) {
    whoseTurn = PLAYER_ONE;
    player = pType;
    elementsToWin = elementsInSequenceToWin;

    game_set_state_label(stateLabel, whoseTurn, player);

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            boardElements[x][y] = EMPTY;
            game_set_element_label(board, x, y, EMPTY, NOT_PLACEHOLDER);
        }
    }
}

void game_move_push(int column, bool wasColumnReplaced) {
    Element element = get_element(whoseTurn);
    game_push_element_to_column(boardElements, board, column, element);

    if (whoseTurn == player && !wasColumnReplaced) {
        connection_send_move(MOVE_PUSH_PREFIX, column);
    }

    if (!game_are_any_moves_left(boardElements)) {
        printf("A tie!");
        whoseTurn = PLAYER_NONE;
        modal_end_game(&player, PLAYER_NONE);
        return;
    }

    PlayerType winner = game_check_winner(boardElements, elementsToWin);
    if (winner != PLAYER_NONE) {
        printf("Player %s wins!\n", winner == PLAYER_ONE ? "1" : "2");
        whoseTurn = PLAYER_NONE;
        modal_end_game(&player, winner);
        return;
    }

    whoseTurn = (whoseTurn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    game_set_state_label(stateLabel, whoseTurn, player);
}

void game_move_replace(int column) {
    //If there is only one element in this column
    if (boardElements[column][BOARD_SIZE - 2] == EMPTY) {
        game_move_push(column, false);
        return;
    }

    Element bottomElement = boardElements[column][BOARD_SIZE - 1];
    for (int y = BOARD_SIZE - 1; y > 0; y--) {
        boardElements[column][y] = boardElements[column][y - 1];
        game_set_element_label(board, column, y, boardElements[column][y], NOT_PLACEHOLDER);
    }

    game_push_element_to_column(boardElements, board, column, bottomElement);

    if (whoseTurn == player) {
        connection_send_move(MOVE_REPLACE_PREFIX, column);
    }

    game_move_push(column, true);
}

static void play_again() {
    connection_send_play_again();
    if (player == PLAYER_ONE) {
        app_play_as_1();
    } else {
        app_play_as_2();
    }
}

static gboolean on_hover_enter(GtkWidget *widget) {
    int x, y;
    game_get_event_box_coordinates(board, widget, &x, &y);

    if (game_is_move_possible(boardElements, x) && whoseTurn == player) {
        Element elem = boardElements[x][y];
        int firstEmptyY = game_get_first_empty_y_in_column(boardElements, x);
        if (elem == EMPTY || firstEmptyY == BOARD_SIZE - 2) {
            game_set_element_label(board, x, firstEmptyY, get_element(player), PLACEHOLDER_ADD);
        } else {
            Element bottomElem = boardElements[x][BOARD_SIZE - 1];
            game_set_element_label(board, x, BOARD_SIZE - 1, bottomElem, PLACEHOLDER_REMOVE);
            game_set_element_label(board, x, firstEmptyY, bottomElem, PLACEHOLDER_ADD);
            game_set_element_label(board, x, firstEmptyY - 1, get_element(player), PLACEHOLDER_ADD);
        }
    }

    return TRUE;
}

static gboolean on_hover_leave(GtkWidget *widget) {
    int x;
    game_get_event_box_coordinates(board, widget, &x, NULL);

    game_set_element_label(board, x, -1, EMPTY, NOT_PLACEHOLDER); //Clear row for placeholders
    for (int currentY = 0; currentY < BOARD_SIZE; currentY++) {
        game_set_element_label(board, x, currentY, boardElements[x][currentY], NOT_PLACEHOLDER);
    }

    return TRUE;
}

static gboolean on_frame_click(GtkWidget *widget) {
    int x, y;
    game_get_event_box_coordinates(board, widget, &x, &y);

    printf("Clicked x:%d  y:%d\n", x, y);

    if (whoseTurn == player && game_is_move_possible(boardElements, x)) {
        Element element = boardElements[x][y];
        if (element == EMPTY) {
            game_move_push(x, false);
        } else {
            game_move_replace(x);
        }
    }

    return TRUE;
}
