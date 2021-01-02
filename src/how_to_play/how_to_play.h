#ifndef TIC_TAC_TOE_C_HOW_TO_PLAY_H
#define TIC_TAC_TOE_C_HOW_TO_PLAY_H

#include "../app/appwin.h"

typedef struct HowToPlayCallbackData {
    AppWindow *window;
    const gchar *from_name;
} HowToPlayCallbackData;

void how_to_play_init(AppWindow *window, GtkGrid *how_to_play, GtkWidget *from);

#endif //TIC_TAC_TOE_C_HOW_TO_PLAY_H
