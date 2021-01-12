#ifndef TIC_TAC_TOE_C_LOBBY_H
#define TIC_TAC_TOE_C_LOBBY_H

#include "../app/appwin.h"
#include "../constants/constants.h"

void lobby_init(AppWindow *window, GtkGrid *lobby);
void lobby_show(PlayerType playerType);
void lobby_connection_established();
void lobby_columns_received(int columns);

#endif //TIC_TAC_TOE_C_LOBBY_H
