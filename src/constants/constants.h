#ifndef TIC_TAC_TOE_C_CONSTANTS_H
#define TIC_TAC_TOE_C_CONSTANTS_H

//Connection
#define MAX_CONSTANT_LENGTH 200

#define REQUEST_CONNECTION "request_connection"
#define DROP_CONNECTION "drop_connection"
#define ACCEPT_CONNECTION "accept_connection"

#define COLUMNS_4 "columns_4"
#define COLUMNS_5 "columns_5"

//Pages
#define GAMEPLAY_PAGE "gameplay_page"
#define MENU_PAGE "menu_page"

//Gameplay
#define BOARD_SIZE 10
#define BOARD_SIZE_MAX_DIGITS 2
#define MOVE_PUSH_PREFIX "move_push_"
#define MOVE_REPLACE_PREFIX "move_replace_"

typedef enum playerType {
    PLAYER_ONE, PLAYER_TWO
} PlayerType;

typedef enum element {
    X, O, EMPTY
} Element;

#endif //TIC_TAC_TOE_C_CONSTANTS_H
