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

typedef enum playerType {
    PLAYER_ONE, PLAYER_TWO
} PlayerType;

#endif //TIC_TAC_TOE_C_CONSTANTS_H
