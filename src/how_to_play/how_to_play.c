#include "how_to_play.h"
#include "../app/app.h"

static gchar previousPage[MAX_CONSTANT_LENGTH];

void how_to_play_init(GtkBuilder *builder) {
    GtkButton *backButton;
    backButton = GTK_BUTTON(gtk_builder_get_object(builder, "how_to_play_back_button"));
    g_signal_connect(backButton, "clicked", G_CALLBACK(app_how_to_play_hide), previousPage);
}

void how_to_play_show(const gchar *fromPage) {
    strcpy(previousPage, fromPage);
}
