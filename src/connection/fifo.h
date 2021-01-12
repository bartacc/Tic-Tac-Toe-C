#ifndef TIC_TAC_TOE_C_FIFO_H
#define TIC_TAC_TOE_C_FIFO_H

#include <stdbool.h>
#include "../constants/constants.h"

typedef struct pipes *PipesPtr;
PipesPtr init_pipes(PlayerType playerType);
void send_string_to_pipe(PipesPtr channel, const char *data);
bool get_string_from_pipe(PipesPtr channel, char *buffer, size_t size);
void close_pipes(PipesPtr channel);

#endif //TIC_TAC_TOE_C_FIFO_H
