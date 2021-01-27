#include "modals.h"
#include "../app/app.h"

static GtkWindow *window;

static void modal_play_again_response(GtkDialog *dialog, int responseID, PlayerType *pType);

void modals_init(GtkWindow *win) {
    window = win;
}

static void setup_modal(GtkWidget *dialog) {
    app_add_css_class_to_widget(dialog, "dialog-widget");
}

void modal_end_game(PlayerType *currentPlayer, PlayerType winner) {
    gchar *labelText;
    if (winner == PLAYER_NONE) {
        labelText = "A tie :/ Want to play again?";
    } else if (winner == *currentPlayer) {
        labelText = "You won! Want to play again?";
    } else {
        labelText = "You lost :( Want to play again?";
    }

    GtkWidget *dialog = gtk_message_dialog_new(window, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, "%s", labelText);
    setup_modal(dialog);
    g_signal_connect(dialog, "response", G_CALLBACK(modal_play_again_response), currentPlayer);
    gtk_widget_show_all(GTK_WIDGET(dialog));
}

static void modal_play_again_response(GtkDialog *dialog, int responseID, PlayerType *pType) {
    if (responseID == GTK_RESPONSE_YES) {
        if (*pType == PLAYER_ONE) {
            app_play_as_1();
        } else {
            app_play_as_2();
        }
    } else {
        app_quit();
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void modal_play_again(PlayerType *currentPlayer) {
    GtkWidget *dialog = gtk_message_dialog_new(window, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, "%s", "Your opponent concedes. Want to play again?");
    setup_modal(dialog);
    g_signal_connect(dialog, "response", G_CALLBACK(modal_play_again_response), currentPlayer);
    gtk_widget_show_all(GTK_WIDGET(dialog));
}
