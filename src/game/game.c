#include <stdbool.h>

#include "game.h"
#include "../app/app.h"
#include "../connection/connection.h"

static const gchar *GAMEPLAY_PAGE_STRING = GAMEPLAY_PAGE;

static GtkGrid *board;
static GtkLabel *stateLabel, *wonLostCountLabel;

static Element boardElements[BOARD_SIZE][BOARD_SIZE];
static PlayerType player;
static PlayerType whoseTurn;

static void play_again();
static void get_event_box_coordinates(GtkWidget *eventBox, int *x, int *y);
static bool is_move_possible(int column);
static int get_first_empty_y_in_column(int column);
static void set_element_label(int x, int y, Element element, PlaceholderType placeholderType);
static void set_state_label();

static gboolean on_hover_enter(GtkWidget *widget);
static gboolean on_hover_leave(GtkWidget *widget);
static gboolean on_frame_click(GtkWidget *widget);

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

void game_start(int columns, PlayerType pType) {
    whoseTurn = PLAYER_ONE;
    player = pType;

    set_state_label();

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            boardElements[x][y] = EMPTY;
            set_element_label(x, y, EMPTY, NOT_PLACEHOLDER);
        }
    }
}

static void play_again() {
    printf("%s\n", "Play again clicked");
}

static gboolean on_frame_click(GtkWidget *widget) {
    int x, y;
    get_event_box_coordinates(widget, &x, &y);

    printf("Clicked x:%d  y:%d\n", x, y);

    if (whoseTurn == player && is_move_possible(x)) {
        Element element = boardElements[x][y];
        if (element == EMPTY) {
            game_move_push(x, false);
        } else {
            game_move_replace(x);
        }
    }

    return TRUE;
}

static void get_event_box_coordinates(GtkWidget *eventBox, int *x, int *y) {
    GValue xVal = G_VALUE_INIT;
    GValue yVal = G_VALUE_INIT;
    g_value_init(&xVal, G_TYPE_INT);
    g_value_init(&yVal, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(board), eventBox, "left-attach", &xVal);
    gtk_container_child_get_property(GTK_CONTAINER(board), eventBox, "top-attach", &yVal);

    if (x != NULL) {
        *x = g_value_get_int(&xVal);
    }
    if (y != NULL) {
        *y = g_value_get_int(&yVal) - 1; //We subtract 1 to account for the placeholder row (row 0)
    }
}

static gboolean on_hover_enter(GtkWidget *widget) {
    int x, y;
    get_event_box_coordinates(widget, &x, &y);

    if (is_move_possible(x) && whoseTurn == player) {
        Element elem = boardElements[x][y];
        int firstEmptyY = get_first_empty_y_in_column(x);
        if (elem == EMPTY || firstEmptyY == BOARD_SIZE - 2) {
            set_element_label(x, firstEmptyY, get_element(player), PLACEHOLDER_ADD);
        } else {
            Element bottomElem = boardElements[x][BOARD_SIZE - 1];
            set_element_label(x, BOARD_SIZE - 1, bottomElem, PLACEHOLDER_REMOVE);
            set_element_label(x, firstEmptyY, bottomElem, PLACEHOLDER_ADD);
            set_element_label(x, firstEmptyY - 1, get_element(player), PLACEHOLDER_ADD);
        }
    }
}

static gboolean on_hover_leave(GtkWidget *widget) {
    int x;
    get_event_box_coordinates(widget, &x, NULL);

    set_element_label(x, -1, EMPTY, NOT_PLACEHOLDER); //Clear row for placeholders
    for (int currentY = 0; currentY < BOARD_SIZE; currentY++) {
        set_element_label(x, currentY, boardElements[x][currentY], NOT_PLACEHOLDER);
    }
}

static bool is_move_possible(int column) {
    return boardElements[column][0] == EMPTY;
}

static int get_first_empty_y_in_column(int column) {
    int y = BOARD_SIZE - 1;
    while (boardElements[column][y] != EMPTY) {
        y--;
    }
    return y;
}

static void push_element(int column, Element element) {
    int y = get_first_empty_y_in_column(column);

    boardElements[column][y] = element;
    set_element_label(column, y, boardElements[column][y], NOT_PLACEHOLDER);
}

void game_move_push(int column, bool wasColumnReplaced) {
    Element element = (whoseTurn == PLAYER_ONE) ? X : O;
    push_element(column, element);

    if (whoseTurn == player && !wasColumnReplaced) {
        connection_send_move(MOVE_PUSH_PREFIX, column);
    }

    whoseTurn = (whoseTurn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    set_state_label();
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
        set_element_label(column, y, boardElements[column][y], NOT_PLACEHOLDER);
    }

    push_element(column, bottomElement);

    if (whoseTurn == player) {
        connection_send_move(MOVE_REPLACE_PREFIX, column);
    }

    game_move_push(column, true);
}

static void set_element_label(int x, int y, Element element, PlaceholderType placeholderType) {
    y = y + 1; //This offset is needed because row 0 is only for placeholder on hover

    GtkBin *eventBox = GTK_BIN(gtk_grid_get_child_at(board, x, y));
    GtkBin *frame = GTK_BIN(gtk_bin_get_child(eventBox));
    GtkLabel *label = GTK_LABEL(gtk_bin_get_child(frame));

    gchar *elementText = "";
    if (element == X) {
        elementText = "X";
    } else if (element == O) {
        elementText = "O";
    }

    gtk_label_set_text(label, elementText);

    switch (placeholderType) {
        case PLACEHOLDER_ADD:
            app_add_css_class_to_widget(GTK_WIDGET(label), CSS_ADD_PLACEHOLDER);
            app_remove_css_class_from_widget(GTK_WIDGET(label), CSS_REMOVE_PLACEHOLDER);
            break;
        case PLACEHOLDER_REMOVE:
            app_add_css_class_to_widget(GTK_WIDGET(label), CSS_REMOVE_PLACEHOLDER);
            app_remove_css_class_from_widget(GTK_WIDGET(label), CSS_ADD_PLACEHOLDER);
            break;
        case NOT_PLACEHOLDER:
            app_remove_css_class_from_widget(GTK_WIDGET(label), CSS_ADD_PLACEHOLDER);
            app_remove_css_class_from_widget(GTK_WIDGET(label), CSS_REMOVE_PLACEHOLDER);
            break;
    }
}

static void set_state_label() {
    gchar state[MAX_CONSTANT_LENGTH];

    if (whoseTurn == player) {
        strcpy(state, "Your move (");
        strcat(state, get_element(player) == X ? "X" : "O");
        strcat(state, ")");
        gtk_label_set_text(stateLabel, state);
    } else {
        strcpy(state, "Opponent's move (");
        strcat(state, get_element(player) == X ? "O" : "X");
        strcat(state, ")");
        gtk_label_set_text(stateLabel, state);
    }
}
