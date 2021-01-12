#ifndef TIC_TAC_TOE_C_CONNECTION_H
#define TIC_TAC_TOE_C_CONNECTION_H

#include "../constants/constants.h"

void connection_init(PlayerType pType);
void connection_drop();
void connection_send_columns(int columns);
void connection_show_error(char *error);


#endif //TIC_TAC_TOE_C_CONNECTION_H
