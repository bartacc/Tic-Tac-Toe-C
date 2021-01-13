#include "constants.h"

Element get_element(PlayerType pType) {
    if (pType == PLAYER_ONE) {
        return X;
    } else {
        return O;
    }
}
