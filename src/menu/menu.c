#include "menu.h"

void menu_init(AppWindow *window, GtkGrid *menu) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(menu));

    for (GList *elem = children; elem != NULL; elem = elem->next) {
        gpointer data = elem->data;

        const GtkWidget *widget = GTK_WIDGET(data);
        const gchar *name = gtk_widget_get_name(widget);

        if (strcmp(name, "menu_play_as_1_button") == 0) {
            g_signal_connect(widget, "clicked", app_window_play_as_1, window);
            continue;
        }
        if (strcmp(name, "menu_play_as_2_button") == 0) {
            g_signal_connect(widget, "clicked", app_window_play_as_2, window);
            continue;
        }
        if (strcmp(name, "menu_how_to_play_button") == 0) {
            g_signal_connect(widget, "clicked", app_window_how_to_play_show, window);
            continue;
        }
        if (strcmp(name, "menu_quit_button") == 0) {
            g_signal_connect(widget, "clicked", app_window_quit, window);
            continue;
        }
    }
}
