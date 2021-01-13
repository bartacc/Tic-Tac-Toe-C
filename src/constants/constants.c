#include <gtk/gtk.h>
#include "constants.h"

Element get_element(PlayerType pType) {
    switch (pType) {
        case PLAYER_ONE:
            return X;
        case PLAYER_TWO:
            return O;
        default:
            return EMPTY;
    }
}

PlayerType get_player(Element element) {
    switch (element) {
        case X:
            return PLAYER_ONE;
        case O:
            return PLAYER_TWO;
        default:
            return PLAYER_NONE;
    }
}
