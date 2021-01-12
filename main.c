#include <gtk/gtk.h>
#include "src/app/app.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    app_init(argc, argv);
    gtk_main();

    return 0;
}
