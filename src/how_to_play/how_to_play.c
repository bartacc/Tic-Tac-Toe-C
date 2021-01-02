#include "how_to_play.h"

static HowToPlayCallbackData callback_data;

void how_to_play_init(AppWindow *window, GtkGrid *how_to_play, GtkWidget *from) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(how_to_play));

    const gchar *from_name = gtk_widget_get_name(from);

    for (GList *elem = children; elem != NULL; elem = elem->next) {
        gpointer data = elem->data;

        const GtkWidget *widget = GTK_WIDGET(data);
        const gchar *name = gtk_widget_get_name(widget);

        if (strcmp(name, "how_to_play_back_button") == 0) {
            callback_data.window = window;
            callback_data.from_name = from_name;
            g_signal_connect(widget, "clicked", app_window_how_to_play_hide, &callback_data);
        }
    }
}
