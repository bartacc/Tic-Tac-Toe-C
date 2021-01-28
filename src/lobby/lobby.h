#ifndef TIC_TAC_TOE_C_LOBBY_H
#define TIC_TAC_TOE_C_LOBBY_H

#include <gtk/gtk.h>
#include "../constants/constants.h"

void lobby_init(GtkBuilder *builder);
void lobby_show(PlayerType playerType);
void lobby_connection_established();
void lobby_size_received(int size);

#endif //TIC_TAC_TOE_C_LOBBY_H
