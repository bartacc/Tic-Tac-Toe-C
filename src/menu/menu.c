#include "menu.h"
#include "../app/app.h"

static const gchar *MENU_PAGE_STRING = MENU_PAGE;

void menu_init(GtkBuilder *builder) {
    GtkButton *playAs1Button, *playAs2Button, *howToPlayButton, *quitButton;

    playAs1Button = GTK_BUTTON(gtk_builder_get_object(builder, "menu_play_as_1_button"));
    playAs2Button = GTK_BUTTON(gtk_builder_get_object(builder, "menu_play_as_2_button"));
    howToPlayButton = GTK_BUTTON(gtk_builder_get_object(builder, "menu_how_to_play_button"));
    quitButton = GTK_BUTTON(gtk_builder_get_object(builder, "menu_quit_button"));

    g_signal_connect(playAs1Button, "clicked", G_CALLBACK(app_play_as_1), NULL);
    g_signal_connect(playAs2Button, "clicked", G_CALLBACK(app_play_as_2), NULL);
    g_signal_connect(howToPlayButton, "clicked", G_CALLBACK(app_how_to_play_show), MENU_PAGE_STRING);
    g_signal_connect(quitButton, "clicked", G_CALLBACK(app_quit), NULL);
}
