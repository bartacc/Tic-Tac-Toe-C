#include <gtk/gtk.h>
#include "src/app/app.h"

int main(int argc, char *argv[]) {
    GApplication *a = G_APPLICATION(app_new());
    int status = g_application_run(a, argc, argv);
    g_object_unref(a);
    return status;
}
