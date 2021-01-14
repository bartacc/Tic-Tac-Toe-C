#include "game_gtk_utils.h"
#include "../../app/app.h"

void game_get_event_box_coordinates(GtkGrid *board, GtkWidget *eventBox, int *x, int *y) {
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

void game_set_element_label(GtkGrid *board, int x, int y, Element element, PlaceholderType placeholderType) {
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

void game_set_state_label(GtkLabel *stateLabel, PlayerType whoseTurn, PlayerType player) {
    gchar state[MAX_STRING_LENGTH];

    if (whoseTurn == player) {
        strcpy(state, "Your move (");
        strcat(state, get_element(player) == X ? "X" : "O");
        strcat(state, ")");
        gtk_label_set_text(stateLabel, state);
    } else if (whoseTurn != PLAYER_NONE) {
        strcpy(state, "Opponent's move (");
        strcat(state, get_element(player) == X ? "O" : "X");
        strcat(state, ")");
        gtk_label_set_text(stateLabel, state);
    }
}

void game_set_won_lost_count_label(GtkLabel *wonLostCountLabel, int gamesWon, int gamesLost) {
    gchar text[MAX_STRING_LENGTH];
    sprintf(text, "Games won: %d\nGames lost: %d", gamesWon, gamesLost);

    gtk_label_set_text(wonLostCountLabel, text);
}
